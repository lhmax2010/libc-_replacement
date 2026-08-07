#!/usr/bin/env python3
import csv
import re
import shlex
import subprocess
from collections import defaultdict
from pathlib import Path

ROOT = Path.cwd()
OUT = ROOT / "progress/R23/tables"
OUT.mkdir(parents=True, exist_ok=True)
ARCHES = ("armv7l", "aarch64", "x86_64")

def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def write_tsv(name, fields, records):
    with (OUT / name).open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader(); w.writerows(records)

exports = read_tsv(ROOT / "progress/R16/tables/libgcc_export_classification.tsv")
version_rows = read_tsv(ROOT / "progress/R16/tables/libgcc_symbol_versions.tsv")
families = read_tsv(ROOT / "progress/R17/tables/missing_helper_family_members.tsv")
runtime = [r for r in read_tsv(ROOT / "progress/R11/tables/elf_inventory.tsv") if r["runtime_elf"] == "YES"]
vendor_names = {r["vendor_package_name"] for r in read_tsv(ROOT / "progress/R13/tables/vendor_prebuilt_closure_assignment.tsv")}

versions = defaultdict(set)
for r in version_rows:
    versions[(r["arch"], r["symbol"])].add(r["version_node"])
export_keys = {(r["arch"], r["symbol"]) for r in exports}
fixed = {(r["arch"], r["symbol"]) for r in families if r["family"] == "FIXED_POINT"} & export_keys
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

def nevra(r):
    epoch = "" if r["epoch"] in ("", "0") else r["epoch"] + ":"
    return f'{r["name"]}-{epoch}{r["version"]}-{r["release"]}.{r["arch"]}'

paths = [str(local_path(r)) for r in runtime]
path_index = {p: i for i, p in enumerate(paths)}
facts = [{"status": "PENDING", "rc": None, "hits": set(),
          "producer": "NOT_IDENTIFIABLE_FROM_ELF_COMMENT", "diagnostic": ""}
         for _ in runtime]
# Do not perform a separate filesystem-stat pass over 36,829 paths: readelf is
# the measurement and will report a missing/unreadable cached ELF itself.  A
# failed batch is split into individual probes below, preserving per-ELF status.
existing_indices = list(range(len(runtime)))

dynsym_re = re.compile(r"^\s*\d+:\s+[0-9a-fA-F]+\s+\d+\s+\S+\s+\S+\s+\S+\s+UND\s+(.+?)\s*$")
commands = []
seq = 0

def invoke(args, scope):
    global seq
    cp = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                        text=True, errors="replace", check=False)
    seq += 1
    commands.append({"sequence": seq, "scope": scope,
                     "command": " ".join(shlex.quote(x) for x in args), "exit_code": cp.returncode})
    return cp

def split_by_file(text):
    current = None
    sections = defaultdict(list)
    for line in text.splitlines():
        if line.startswith("File: "):
            current = path_index.get(line[6:])
        elif current is not None:
            sections[current].append(line)
    return sections

batch_size = 160
for start in range(0, len(existing_indices), batch_size):
    indices = existing_indices[start:start + batch_size]
    ps = [paths[i] for i in indices]
    cp = invoke(["readelf", "--dyn-syms", "-W", *ps], f"DYNSYM_BATCH_{start // batch_size + 1:04d}")
    if cp.returncode == 0:
        sections = split_by_file(cp.stdout)
        for i in indices:
            facts[i]["status"] = "OK"
            facts[i]["rc"] = 0
            for line in sections.get(i, []):
                m = dynsym_re.match(line)
                if m:
                    raw = m.group(1).split()[0]
                    base = raw.split("@", 1)[0]
                    if base in fixed_by_arch.get(runtime[i]["arch"], set()):
                        facts[i]["hits"].add((base, raw))
    else:
        for i in indices:
            one = invoke(["readelf", "--dyn-syms", "-W", paths[i]], "FAILED_DYNSYM_BATCH_INDIVIDUAL_FALLBACK")
            facts[i]["status"] = "OK" if one.returncode == 0 else "READELF_FAILED"
            facts[i]["rc"] = one.returncode
            facts[i]["diagnostic"] = one.stdout[:2000]
            for line in one.stdout.splitlines():
                m = dynsym_re.match(line)
                if m:
                    raw = m.group(1).split()[0]
                    base = raw.split("@", 1)[0]
                    if base in fixed_by_arch.get(runtime[i]["arch"], set()):
                        facts[i]["hits"].add((base, raw))
    # Compiler-producer evidence is a separate, information-only probe; its
    # availability never converts a successful UND scan into a failure.
    comment = invoke(["readelf", "--string-dump=.comment", "-W", *ps], f"COMMENT_BATCH_{start // batch_size + 1:04d}")
    comment_sections = split_by_file(comment.stdout)
    for i in indices:
        t = "\n".join(comment_sections.get(i, []))
        has_gcc = bool(re.search(r"\bGCC:\s*\(|\bGCC:\s", t))
        has_clang = "clang version" in t or ("Android (" in t and "clang" in t.lower())
        facts[i]["producer"] = ("MIXED_GCC_AND_CLANG_COMMENT" if has_gcc and has_clang else
                                "GCC_COMMENT_IDENTIFIED" if has_gcc else
                                "CLANG_COMMENT_IDENTIFIED" if has_clang else
                                "NOT_IDENTIFIABLE_FROM_ELF_COMMENT")

scan_rows, hit_rows, vendor_rows, gcc_rows, failures = [], [], [], [], []
for r, p, f in zip(runtime, paths, facts):
    symbols = sorted({s for s, _ in f["hits"]})
    common = {"repo_id": r["repo_id"], "package_nevra": nevra(r), "package_name": r["name"],
              "source_rpm": r["sourcerpm"], "arch": r["arch"], "elf_path": r["path"], "rpm_sha256": r["rpm_sha256"]}
    scan_rows.append({**common, "local_elf_path": p, "probe_status": f["status"], "readelf_exit_code": f["rc"],
                      "fixed_point_und_count": len(symbols), "fixed_point_und_symbols": ",".join(symbols) if symbols else "NONE",
                      "compiler_producer_evidence": f["producer"]})
    for base, raw in sorted(f["hits"]):
        hit_rows.append({"symbol": base, "raw_dynamic_symbol": raw, **common})
    if r["name"] in vendor_names:
        vendor_rows.append({"vendor_package": r["name"], "package_nevra": nevra(r), "arch": r["arch"],
                            "elf_path": r["path"], "probe_status": f["status"], "fixed_point_und_count": len(symbols),
                            "fixed_point_und_symbols": ",".join(symbols) if symbols else "NONE",
                            "compiler_producer_evidence": f["producer"]})
    if f["producer"] in ("GCC_COMMENT_IDENTIFIED", "MIXED_GCC_AND_CLANG_COMMENT"):
        gcc_rows.append({"package_nevra": nevra(r), "package_name": r["name"], "source_rpm": r["sourcerpm"],
                         "arch": r["arch"], "elf_path": r["path"], "producer_class": f["producer"],
                         "third_party_or_prebuilt_status": "NOT_DETERMINABLE_FROM_ELF_COMMENT",
                         "fixed_point_und_count": len(symbols), "fixed_point_und_symbols": ",".join(symbols) if symbols else "NONE"})
    if f["status"] != "OK":
        failures.append({"package_nevra": nevra(r), "arch": r["arch"], "elf_path": r["path"], "local_elf_path": p,
                         "status": f["status"], "exit_code": f["rc"],
                         "diagnostic": f["diagnostic"].replace("\t", " ").replace("\n", " | ")})

write_tsv("elf_probe_commands.tsv", ["sequence", "scope", "command", "exit_code"], commands)
write_tsv("runtime_elf_scan_results.tsv", list(scan_rows[0]), scan_rows)
write_tsv("fixed_point_undefined_references.tsv", ["symbol", "raw_dynamic_symbol", "repo_id", "package_nevra", "package_name", "source_rpm", "arch", "elf_path", "rpm_sha256"], hit_rows)
write_tsv("vendor_prebuilt_elf_scan.tsv", list(vendor_rows[0]) if vendor_rows else ["vendor_package"], vendor_rows)
write_tsv("gcc_comment_identified_elfs.tsv", list(gcc_rows[0]) if gcc_rows else ["package_nevra"], gcc_rows)
write_tsv("elf_probe_failures.tsv", ["package_nevra", "arch", "elf_path", "local_elf_path", "status", "exit_code", "diagnostic"], failures)

summary = []
for arch in (*ARCHES, "ALL"):
    subset = [x for x in scan_rows if arch == "ALL" or x["arch"] == arch]
    ok = [x for x in subset if x["probe_status"] == "OK"]
    h = [x for x in ok if x["fixed_point_und_count"]]
    ah = [x for x in hit_rows if arch == "ALL" or x["arch"] == arch]
    result = "ZERO_REFERENCES_CONFIRMED" if len(ok) == len(subset) and not h else ("REFERENCES_FOUND" if h else "UNDETERMINED")
    summary.append({"arch": arch, "runtime_elf_denominator": len(subset), "successfully_scanned_elf": len(ok),
                    "unavailable_or_failed_elf": len(subset) - len(ok),
                    "fixed_point_export_symbol_count": sum(1 for a, _ in fixed if arch == "ALL" or a == arch),
                    "referenced_fixed_point_symbol_count": len({x["symbol"] for x in ah}),
                    "referencing_elf_count": len(h), "referencing_binary_package_count": len({x["package_nevra"] for x in h}),
                    "result": result})
write_tsv("fixed_point_scan_summary.tsv", list(summary[0]), summary)

original = {"armv7l": 986, "aarch64": 25, "x86_64": 27}
corrected = []
for arch in ARCHES:
    s = next(x for x in summary if x["arch"] == arch)
    if s["result"] == "ZERO_REFERENCES_CONFIRMED":
        value, rule = original[arch] - s["fixed_point_export_symbol_count"], "ORIGINAL_GAP_MINUS_MEASURED_ZERO_REFERENCE_FIXED_POINT_FAMILY"
    elif s["result"] == "REFERENCES_FOUND":
        value, rule = original[arch], "ORIGINAL_GAP_RETAINED_FIXED_POINT_FAMILY_HAS_REFERENCES"
    else:
        value, rule = "UNDETERMINED", "SCAN_INCOMPLETE_NO_NUMERIC_CORRECTION"
    corrected.append({"arch": arch, "original_missing_helper_count": original[arch],
                      "fixed_point_family_count": s["fixed_point_export_symbol_count"],
                      "referenced_fixed_point_symbol_count": s["referenced_fixed_point_symbol_count"],
                      "corrected_missing_helper_count": value, "correction_rule": rule})
write_tsv("corrected_gap_summary.tsv", list(corrected[0]), corrected)

print(f"runtime_elf={len(runtime)} success={sum(x['probe_status']=='OK' for x in scan_rows)} failures={len(failures)} commands={len(commands)}")
print(f"fixed_symbols={len(fixed)} hit_rows={len(hit_rows)} vendor_elf={len(vendor_rows)} gcc_comment_elf={len(gcc_rows)}")
for r in summary:
    print("SUMMARY", *(f"{k}={v}" for k, v in r.items()))
