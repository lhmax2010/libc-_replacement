#!/usr/bin/env python3
import csv
import struct
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path.cwd(); OUT = ROOT / "progress/R23/tables"; OUT.mkdir(parents=True, exist_ok=True)
ARCHES = ("armv7l", "aarch64", "x86_64")

def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as f: return list(csv.DictReader(f, delimiter="\t"))

def write_tsv(name, fields, records):
    with (OUT / name).open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader(); w.writerows(records)

exports = read_tsv(ROOT / "progress/R16/tables/libgcc_export_classification.tsv")
version_rows = read_tsv(ROOT / "progress/R16/tables/libgcc_symbol_versions.tsv")
families = read_tsv(ROOT / "progress/R17/tables/missing_helper_family_members.tsv")
runtime = [r for r in read_tsv(ROOT / "progress/R11/tables/elf_inventory.tsv") if r["runtime_elf"] == "YES"]
binary_packages = read_tsv(ROOT / "progress/R11/tables/binary_package_records.tsv")
vendor_names = {r["vendor_package_name"] for r in read_tsv(ROOT / "progress/R13/tables/vendor_prebuilt_closure_assignment.tsv")}
versions = defaultdict(set)
for r in version_rows: versions[(r["arch"], r["symbol"])].add(r["version_node"])
fixed = ({(r["arch"], r["symbol"]) for r in families if r["family"] == "FIXED_POINT"}
         & {(r["arch"], r["symbol"]) for r in exports})
fixed_by_arch = {a: {s for aa, s in fixed if aa == a} for a in ARCHES}

write_tsv("libgcc_exports_full.tsv", ["arch", "symbol", "category", "version_nodes"], [
    {"arch": r["arch"], "symbol": r["symbol"], "category": r["category"],
     "version_nodes": ",".join(sorted(versions[(r["arch"], r["symbol"])]))}
    for r in sorted(exports, key=lambda x: (x["arch"], x["symbol"]))])
write_tsv("fixed_point_symbols.tsv", ["arch", "symbol", "version_nodes", "classification_basis"], [
    {"arch": a, "symbol": s, "version_nodes": ",".join(sorted(versions[(a, s)])),
     "classification_basis": "R17_FIXED_POINT_MEMBER_AND_R16_ACTUAL_LIBGCC_EXPORT"}
    for a, s in sorted(fixed)])

def local_path(r):
    sha = r["rpm_sha256"]
    return ROOT / "tmp/R11/extracted" / sha[:2] / sha / r["path"].lstrip("/")

def cstr(blob, offset):
    if offset >= len(blob): return ""
    end = blob.find(b"\0", offset)
    if end < 0: end = len(blob)
    return blob[offset:end].decode("utf-8", "replace")

def parse_elf(item):
    index, r = item; path = local_path(r); targets = fixed_by_arch.get(r["arch"], set())
    result = {"index": index, "status": "PASS", "exit_code": 0, "hits": set(),
              "producer": "NOT_IDENTIFIABLE_FROM_ELF_COMMENT", "diagnostic": "", "dynsym_count": 0}
    try:
        with path.open("rb") as f:
            ident = f.read(64)
            if len(ident) < 16 or ident[:4] != b"\x7fELF": raise ValueError("ELF_MAGIC_OR_HEADER_INVALID")
            cls, data = ident[4], ident[5]
            endian = "<" if data == 1 else ">" if data == 2 else None
            if endian is None or cls not in (1, 2): raise ValueError("ELF_CLASS_OR_ENDIAN_UNSUPPORTED")
            if cls == 1:
                shoff = struct.unpack_from(endian + "I", ident, 32)[0]
                shentsize, shnum, shstrndx = struct.unpack_from(endian + "HHH", ident, 46)
                shfmt, shsize = endian + "IIIIIIIIII", 40
                symfmt, symsize = endian + "IIIBBH", 16
            else:
                shoff = struct.unpack_from(endian + "Q", ident, 40)[0]
                shentsize, shnum, shstrndx = struct.unpack_from(endian + "HHH", ident, 58)
                shfmt, shsize = endian + "IIQQQQIIQQ", 64
                symfmt, symsize = endian + "IBBHQQ", 24
            if shentsize < shsize or not shoff: raise ValueError("SECTION_HEADER_TABLE_INVALID")
            f.seek(shoff); raw = f.read(shentsize * shnum)
            if len(raw) != shentsize * shnum: raise ValueError("SECTION_HEADER_TABLE_TRUNCATED")
            sections = [struct.unpack_from(shfmt, raw, i * shentsize) for i in range(shnum)]
            if shstrndx >= len(sections): raise ValueError("SHSTRTAB_INDEX_INVALID")
            shstr = sections[shstrndx]
            f.seek(shstr[4]); shstr_blob = f.read(shstr[5])
            names = [cstr(shstr_blob, s[0]) for s in sections]
            for si, sec in enumerate(sections):
                stype, offset, size, link, entsize = sec[1], sec[4], sec[5], sec[6], sec[9]
                if stype == 11:  # SHT_DYNSYM
                    if link >= len(sections): raise ValueError("DYNSYM_LINK_INVALID")
                    strsec = sections[link]
                    f.seek(strsec[4]); dynstr = f.read(strsec[5])
                    f.seek(offset); dynsym = f.read(size)
                    step = entsize or symsize
                    if step < symsize: raise ValueError("DYNSYM_ENTSIZE_INVALID")
                    for off in range(0, len(dynsym) - symsize + 1, step):
                        fields = struct.unpack_from(symfmt, dynsym, off)
                        if cls == 1: st_name, st_shndx = fields[0], fields[5]
                        else: st_name, st_shndx = fields[0], fields[3]
                        result["dynsym_count"] += 1
                        if st_shndx == 0:
                            name = cstr(dynstr, st_name)
                            base = name.split("@", 1)[0]
                            if base in targets: result["hits"].add((base, name))
                if names[si] == ".comment" and size:
                    f.seek(offset); comment = f.read(size).decode("utf-8", "replace")
                    gcc = "GCC: (" in comment or "GCC: " in comment
                    clang = "clang version" in comment or ("Android (" in comment and "clang" in comment.lower())
                    result["producer"] = ("MIXED_GCC_AND_CLANG_COMMENT" if gcc and clang else
                                          "GCC_COMMENT_IDENTIFIED" if gcc else
                                          "CLANG_COMMENT_IDENTIFIED" if clang else
                                          "NOT_IDENTIFIABLE_FROM_ELF_COMMENT")
    except Exception as exc:
        result["status"] = "FAIL"; result["exit_code"] = 1
        result["diagnostic"] = f"{type(exc).__name__}:{exc}"
    return result

with ThreadPoolExecutor(max_workers=8) as pool:
    parsed = list(pool.map(parse_elf, enumerate(runtime), chunksize=64))
parsed.sort(key=lambda x: x["index"])

def nevra(r):
    epoch = "" if r["epoch"] in ("", "0") else r["epoch"] + ":"
    return f'{r["name"]}-{epoch}{r["version"]}-{r["release"]}.{r["arch"]}'

scan, hits, vendors, producers, failures = [], [], [], [], []
for r, f in zip(runtime, parsed):
    symbols = sorted({s for s, _ in f["hits"]})
    common = {"repo_id": r["repo_id"], "package_nevra": nevra(r), "package_name": r["name"],
              "source_rpm": r["sourcerpm"], "arch": r["arch"], "elf_path": r["path"], "rpm_sha256": r["rpm_sha256"]}
    scan.append({**common, "local_elf_path": str(local_path(r)), "parser_status": f["status"],
                 "parser_exit_code": f["exit_code"], "dynamic_symbol_count": f["dynsym_count"],
                 "fixed_point_und_count": len(symbols), "fixed_point_und_symbols": ",".join(symbols) if symbols else "NONE",
                 "compiler_producer_evidence": f["producer"]})
    for base, raw in sorted(f["hits"]): hits.append({"symbol": base, "raw_dynamic_symbol": raw, **common})
    if r["name"] in vendor_names:
        vendors.append({"vendor_package": r["name"], "package_nevra": nevra(r), "arch": r["arch"], "elf_path": r["path"],
                        "parser_status": f["status"], "fixed_point_und_count": len(symbols),
                        "fixed_point_und_symbols": ",".join(symbols) if symbols else "NONE", "compiler_producer_evidence": f["producer"]})
    if f["producer"] in ("GCC_COMMENT_IDENTIFIED", "MIXED_GCC_AND_CLANG_COMMENT"):
        producers.append({"package_nevra": nevra(r), "package_name": r["name"], "source_rpm": r["sourcerpm"],
                          "arch": r["arch"], "elf_path": r["path"], "producer_class": f["producer"],
                          "third_party_or_prebuilt_status": "VENDOR_PREBUILT_ADJUDICATED" if r["name"] in vendor_names else "NOT_DETERMINABLE_FROM_ELF_COMMENT"})
    if f["status"] != "PASS":
        failures.append({"package_nevra": nevra(r), "arch": r["arch"], "elf_path": r["path"],
                         "local_elf_path": str(local_path(r)), "status": f["status"], "exit_code": f["exit_code"], "diagnostic": f["diagnostic"]})

write_tsv("runtime_elf_scan_results.tsv", list(scan[0]), scan)
write_tsv("fixed_point_undefined_references.tsv", ["symbol", "raw_dynamic_symbol", "repo_id", "package_nevra", "package_name", "source_rpm", "arch", "elf_path", "rpm_sha256"], hits)
write_tsv("vendor_prebuilt_elf_scan.tsv", list(vendors[0]) if vendors else ["vendor_package"], vendors)
write_tsv("gcc_comment_identified_elfs.tsv", list(producers[0]) if producers else ["package_nevra"], producers)
write_tsv("elf_probe_failures.tsv", ["package_nevra", "arch", "elf_path", "local_elf_path", "status", "exit_code", "diagnostic"], failures)

summary = []
for arch in (*ARCHES, "ALL"):
    subset = [x for x in scan if arch == "ALL" or x["arch"] == arch]; ok = [x for x in subset if x["parser_status"] == "PASS"]
    h = [x for x in ok if x["fixed_point_und_count"]]; ah = [x for x in hits if arch == "ALL" or x["arch"] == arch]
    result = "ZERO_REFERENCES_CONFIRMED" if len(ok) == len(subset) and not h else ("REFERENCES_FOUND" if h else "UNDETERMINED")
    summary.append({"arch": arch, "runtime_elf_denominator": len(subset), "successfully_scanned_elf": len(ok),
                    "unavailable_or_failed_elf": len(subset)-len(ok),
                    "fixed_point_export_symbol_count": sum(1 for a, _ in fixed if arch == "ALL" or a == arch),
                    "referenced_fixed_point_symbol_count": len({x["symbol"] for x in ah}), "referencing_elf_count": len(h),
                    "referencing_binary_package_count": len({x["package_nevra"] for x in h}), "result": result})
write_tsv("fixed_point_scan_summary.tsv", list(summary[0]), summary)

original = {"armv7l": 986, "aarch64": 25, "x86_64": 27}; corrected = []
for arch in ARCHES:
    s = next(x for x in summary if x["arch"] == arch)
    if s["result"] == "ZERO_REFERENCES_CONFIRMED": value, rule = original[arch]-s["fixed_point_export_symbol_count"], "ORIGINAL_GAP_MINUS_MEASURED_ZERO_REFERENCE_FIXED_POINT_FAMILY"
    elif s["result"] == "REFERENCES_FOUND": value, rule = original[arch], "ORIGINAL_GAP_RETAINED_FIXED_POINT_FAMILY_HAS_REFERENCES"
    else: value, rule = "UNDETERMINED", "SCAN_INCOMPLETE_NO_NUMERIC_CORRECTION"
    corrected.append({"arch": arch, "original_missing_helper_count": original[arch], "fixed_point_family_count": s["fixed_point_export_symbol_count"],
                      "referenced_fixed_point_symbol_count": s["referenced_fixed_point_symbol_count"], "corrected_missing_helper_count": value, "correction_rule": rule})
write_tsv("corrected_gap_summary.tsv", list(corrected[0]), corrected)
scope = [{"counting_unit":"binary_package_instance","denominator":len(binary_packages),"source":"R11 binary_package_records.tsv"},
         {"counting_unit":"runtime_ELF","denominator":len(runtime),"source":"R11 elf_inventory.tsv runtime_elf=YES"},
         {"counting_unit":"source_package","denominator":"NOT_USED_IN_REFERENCE_RATIO","source":"no source-package ratio in this scan"}]
write_tsv("scan_scope_denominators.tsv", list(scope[0]), scope)
print(f"runtime_elf={len(runtime)} binary_packages={len(binary_packages)} success={len(runtime)-len(failures)} failures={len(failures)}")
print(f"fixed_symbols={len(fixed)} hit_rows={len(hits)} vendor_elf={len(vendors)} gcc_comment_elf={len(producers)}")
for r in summary: print("SUMMARY", *(f"{k}={v}" for k,v in r.items()))
