#!/usr/bin/env python3
import csv
import os
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

def write_tsv(path, fields, data):
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader()
        w.writerows(data)

exports = read_tsv(ROOT / "progress/R16/tables/libgcc_export_classification.tsv")
version_rows = read_tsv(ROOT / "progress/R16/tables/libgcc_symbol_versions.tsv")
families = read_tsv(ROOT / "progress/R17/tables/missing_helper_family_members.tsv")
runtime = [r for r in read_tsv(ROOT / "progress/R11/tables/elf_inventory.tsv") if r["runtime_elf"] == "YES"]
vendor_names = {r["vendor_package_name"] for r in read_tsv(ROOT / "progress/R13/tables/vendor_prebuilt_closure_assignment.tsv")}

versions = defaultdict(set)
for r in version_rows:
    versions[(r["arch"], r["symbol"])].add(r["version_node"])
export_keys = {(r["arch"], r["symbol"]) for r in exports}
fixed_r17 = {(r["arch"], r["symbol"]) for r in families if r["family"] == "FIXED_POINT"}
fixed = fixed_r17 & export_keys
fixed_by_arch = {a: {s for aa, s in fixed if aa == a} for a in ARCHES}

write_tsv(OUT / "libgcc_exports_full.tsv", ["arch", "symbol", "category", "version_nodes"], [
    {"arch": r["arch"], "symbol": r["symbol"], "category": r["category"],
     "version_nodes": ",".join(sorted(versions[(r["arch"], r["symbol"])]))}
    for r in sorted(exports, key=lambda x: (x["arch"], x["symbol"]))
])
write_tsv(OUT / "fixed_point_symbols.tsv", ["arch", "symbol", "version_nodes", "classification_basis"], [
    {"arch": a, "symbol": s, "version_nodes": ",".join(sorted(versions[(a, s)])),
     "classification_basis": "R17_FIXED_POINT_MEMBER_AND_R16_ACTUAL_LIBGCC_EXPORT"}
    for a, s in sorted(fixed)
])

def local_path(r):
    sha = r["rpm_sha256"]
    return ROOT / "tmp/R11/extracted" / sha[:2] / sha / r["path"].lstrip("/")

def nevra(r):
    epoch = "" if r["epoch"] in ("", "0") else r["epoch"] + ":"
    return f'{r["name"]}-{epoch}{r["version"]}-{r["release"]}.{r["arch"]}'

path_to_index = {str(local_path(r)): i for i, r in enumerate(runtime)}
outputs = {i: [] for i in range(len(runtime))}
statuses = {i: ("MISSING_LOCAL_ELF" if not local_path(r).is_file() else "PENDING", 66 if not local_path(r).is_file() else None, "") for i, r in enumerate(runtime)}
command_rows = []
sequence = 0
batch_size = 160

def run(paths, label):
    global sequence
    cmd = ["readelf", "--dyn-syms", "--string-dump=.comment", "-W", *paths]
    cp = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                        text=True, errors="replace", check=False)
    sequence += 1
    command_rows.append({"sequence": sequence, "scope": label,
                         "command": " ".join(shlex.quote(x) for x in cmd), "exit_code": cp.returncode})
    return cp

existing = [str(local_path(r)) for r in runtime if local_path(r).is_file()]
for start in range(0, len(existing), batch_size):
    paths = existing[start:start + batch_size]
    cp = run(paths, f"BATCH_{start // batch_size + 1:04d}")
    if cp.returncode != 0:
        # Preserve fail-closed per-file status if an otherwise valid batch fails.
        for path in paths:
            one = run([path], "FAILED_BATCH_INDIVIDUAL_FALLBACK")
            idx = path_to_index[path]
            statuses[idx] = ("OK" if one.returncode == 0 else "READELF_FAILED", one.returncode, one.stdout)
            outputs[idx] = one.stdout.splitlines()
        continue
    current = None
    for line in cp.stdout.splitlines():
        if line.startswith("File: "):
            current = path_to_index.get(line[6:])
            continue
        if current is not None:
            outputs[current].append(line)
    for path in paths:
        idx = path_to_index[path]
        statuses[idx] = ("OK", 0, "")

dynsym_re = re.compile(r"^\s*\d+:\s+[0-9a-fA-F]+\s+\d+\s+\S+\s+\S+\s+\S+\s+UND\s+(.+?)\s*$")
scan_rows, hit_rows, vendor_rows, gcc_rows, fail_rows = [], [], [], [], []
for idx, r in enumerate(runtime):
    status, rc, diagnostic = statuses[idx]
    text = "\n".join(outputs[idx])
    und = set()
    for line in outputs[idx]:
        m = dynsym_re.match(line)
        if m:
            raw = m.group(1).split()[0]
            base = raw.split("@", 1)[0]
            if base in fixed_by_arch.get(r["arch"], set()):
                und.add((base, raw))
    has_gcc = bool(re.search(r"\bGCC:\s*\(|\bGCC:\s", text))
    has_clang = "clang version" in text or ("Android (" in text and "clang" in text.lower())
    producer = ("MIXED_GCC_AND_CLANG_COMMENT" if has_gcc and has_clang else
                "GCC_COMMENT_IDENTIFIED" if has_gcc else
                "CLANG_COMMENT_IDENTIFIED" if has_clang else
                "NOT_IDENTIFIABLE_FROM_ELF_COMMENT")
    hit_symbols = sorted({s for s, _ in und})
    common = {"repo_id": r["repo_id"], "package_nevra": nevra(r), "package_name": r["name"],
              "source_rpm": r["sourcerpm"], "arch": r["arch"], "elf_path": r["path"],
              "rpm_sha256": r["rpm_sha256"]}
    scan_rows.append({**common, "local_elf_path": str(local_path(r)), "probe_status": status,
                      "readelf_exit_code": rc, "fixed_point_und_count": len(hit_symbols),
                      "fixed_point_und_symbols": ",".join(hit_symbols) if hit_symbols else "NONE",
                      "compiler_producer_evidence": producer})
    for base, raw in sorted(und):
        hit_rows.append({"symbol": base, "raw_dynamic_symbol": raw, **common})
    if r["name"] in vendor_names:
        vendor_rows.append({"vendor_package": r["name"], "package_nevra": nevra(r), "arch": r["arch"],
                            "elf_path": r["path"], "probe_status": status,
                            "fixed_point_und_count": len(hit_symbols),
                            "fixed_point_und_symbols": ",".join(hit_symbols) if hit_symbols else "NONE",
                            "compiler_producer_evidence": producer})
    if producer in ("GCC_COMMENT_IDENTIFIED", "MIXED_GCC_AND_CLANG_COMMENT"):
        gcc_rows.append({"package_nevra": nevra(r), "package_name": r["name"], "source_rpm": r["sourcerpm"],
                         "arch": r["arch"], "elf_path": r["path"], "producer_class": producer,
                         "third_party_or_prebuilt_status": "NOT_DETERMINABLE_FROM_ELF_COMMENT",
                         "fixed_point_und_count": len(hit_symbols),
                         "fixed_point_und_symbols": ",".join(hit_symbols) if hit_symbols else "NONE"})
    if status != "OK":
        fail_rows.append({"package_nevra": nevra(r), "arch": r["arch"], "elf_path": r["path"],
                          "local_elf_path": str(local_path(r)), "status": status, "exit_code": rc,
                          "diagnostic": diagnostic.replace("\t", " ").replace("\n", " | ")[:2000] or "NONE"})

write_tsv(OUT / "elf_probe_commands.tsv", ["sequence", "scope", "command", "exit_code"], command_rows)
write_tsv(OUT / "runtime_elf_scan_results.tsv", list(scan_rows[0]), scan_rows)
write_tsv(OUT / "fixed_point_undefined_references.tsv", ["symbol", "raw_dynamic_symbol", "repo_id", "package_nevra", "package_name", "source_rpm", "arch", "elf_path", "rpm_sha256"], hit_rows)
write_tsv(OUT / "vendor_prebuilt_elf_scan.tsv", list(vendor_rows[0]) if vendor_rows else ["vendor_package"], vendor_rows)
write_tsv(OUT / "gcc_comment_identified_elfs.tsv", list(gcc_rows[0]) if gcc_rows else ["package_nevra"], gcc_rows)
write_tsv(OUT / "elf_probe_failures.tsv", ["package_nevra", "arch", "elf_path", "local_elf_path", "status", "exit_code", "diagnostic"], fail_rows)

summary = []
for arch in (*ARCHES, "ALL"):
    subset = [x for x in scan_rows if arch == "ALL" or x["arch"] == arch]
    ok = [x for x in subset if x["probe_status"] == "OK"]
    h = [x for x in ok if x["fixed_point_und_count"]]
    ah = [x for x in hit_rows if arch == "ALL" or x["arch"] == arch]
    result = "ZERO_REFERENCES_CONFIRMED" if len(ok) == len(subset) and not h else ("REFERENCES_FOUND" if h else "UNDETERMINED")
    summary.append({"arch": arch, "runtime_elf_denominator": len(subset),
                    "successfully_scanned_elf": len(ok), "unavailable_or_failed_elf": len(subset) - len(ok),
                    "fixed_point_export_symbol_count": sum(1 for a, _ in fixed if arch == "ALL" or a == arch),
                    "referenced_fixed_point_symbol_count": len({x["symbol"] for x in ah}),
                    "referencing_elf_count": len(h),
                    "referencing_binary_package_count": len({x["package_nevra"] for x in h}), "result": result})
write_tsv(OUT / "fixed_point_scan_summary.tsv", list(summary[0]), summary)

original = {"armv7l": 986, "aarch64": 25, "x86_64": 27}
corrected = []
for arch in ARCHES:
    s = next(x for x in summary if x["arch"] == arch)
    if s["result"] == "ZERO_REFERENCES_CONFIRMED":
        count, rule = original[arch] - s["fixed_point_export_symbol_count"], "ORIGINAL_GAP_MINUS_MEASURED_ZERO_REFERENCE_FIXED_POINT_FAMILY"
    elif s["result"] == "REFERENCES_FOUND":
        count, rule = original[arch], "ORIGINAL_GAP_RETAINED_FIXED_POINT_FAMILY_HAS_REFERENCES"
    else:
        count, rule = "UNDETERMINED", "SCAN_INCOMPLETE_NO_NUMERIC_CORRECTION"
    corrected.append({"arch": arch, "original_missing_helper_count": original[arch],
                      "fixed_point_family_count": s["fixed_point_export_symbol_count"],
                      "referenced_fixed_point_symbol_count": s["referenced_fixed_point_symbol_count"],
                      "corrected_missing_helper_count": count, "correction_rule": rule})
write_tsv(OUT / "corrected_gap_summary.tsv", list(corrected[0]), corrected)

print(f"runtime_elf={len(runtime)} success={sum(x['probe_status']=='OK' for x in scan_rows)} failures={len(fail_rows)} batches={len(command_rows)}")
print(f"fixed_symbols={len(fixed)} hit_rows={len(hit_rows)} vendor_elf={len(vendor_rows)} gcc_comment_elf={len(gcc_rows)}")
for r in summary:
    print("SUMMARY", *(f"{k}={v}" for k, v in r.items()))
