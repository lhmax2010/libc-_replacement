#!/usr/bin/env python3
import csv
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path.cwd()
OUT = ROOT / "progress/R23/tables"
OUT.mkdir(parents=True, exist_ok=True)

EXPORTS = ROOT / "progress/R16/tables/libgcc_export_classification.tsv"
VERSIONS = ROOT / "progress/R16/tables/libgcc_symbol_versions.tsv"
FAMILIES = ROOT / "progress/R17/tables/missing_helper_family_members.tsv"
ELFS = ROOT / "progress/R11/tables/elf_inventory.tsv"
VENDORS = ROOT / "progress/R13/tables/vendor_prebuilt_closure_assignment.tsv"
EXTRACTED = ROOT / "tmp/R11/extracted"

ARCHES = ("armv7l", "aarch64", "x86_64")

def rows(path):
    with path.open(newline="", encoding="utf-8") as f:
        yield from csv.DictReader(f, delimiter="\t")

def write_tsv(path, fields, data):
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader()
        w.writerows(data)

exports = list(rows(EXPORTS))
versions = defaultdict(set)
for r in rows(VERSIONS):
    versions[(r["arch"], r["symbol"])].add(r["version_node"])

# R17's FIXED_POINT membership was derived from the actual libgcc_s export set,
# then intersected with the compiler-rt missing set.  Rejoin it to R16 exports
# so every listed member has a measured export as its basis.
fixed_r17 = {(r["arch"], r["symbol"]) for r in rows(FAMILIES) if r["family"] == "FIXED_POINT"}
export_keys = {(r["arch"], r["symbol"]) for r in exports}
fixed = fixed_r17 & export_keys

export_out = []
for r in sorted(exports, key=lambda x: (x["arch"], x["symbol"])):
    export_out.append({
        "arch": r["arch"], "symbol": r["symbol"], "category": r["category"],
        "version_nodes": ",".join(sorted(versions[(r["arch"], r["symbol"])], key=str)),
    })
write_tsv(OUT / "libgcc_exports_full.tsv", ["arch", "symbol", "category", "version_nodes"], export_out)

fixed_out = []
for arch, symbol in sorted(fixed):
    fixed_out.append({
        "arch": arch,
        "symbol": symbol,
        "version_nodes": ",".join(sorted(versions[(arch, symbol)])),
        "classification_basis": "R17_FIXED_POINT_MEMBER_AND_R16_ACTUAL_LIBGCC_EXPORT",
    })
write_tsv(OUT / "fixed_point_symbols.tsv", ["arch", "symbol", "version_nodes", "classification_basis"], fixed_out)

fixed_by_arch = {a: {s for aa, s in fixed if aa == a} for a in ARCHES}
vendor_names = {r["vendor_package_name"] for r in rows(VENDORS)}
runtime = [r for r in rows(ELFS) if r["runtime_elf"] == "YES"]

def local_path(r):
    sha = r["rpm_sha256"]
    return EXTRACTED / sha[:2] / sha / r["path"].lstrip("/")

def nevra(r):
    epoch = "" if r["epoch"] in ("", "0") else r["epoch"] + ":"
    return f'{r["name"]}-{epoch}{r["version"]}-{r["release"]}.{r["arch"]}'

dynsym_re = re.compile(r"^\s*\d+:\s+[0-9a-fA-F]+\s+\d+\s+\S+\s+\S+\s+\S+\s+UND\s+(.+?)\s*$")

def probe(item):
    idx, r = item
    p = local_path(r)
    cmd = ["readelf", "--dyn-syms", "--string-dump=.comment", "-W", str(p)]
    if not p.is_file():
        return idx, r, p, cmd, 66, "", "MISSING_LOCAL_ELF"
    cp = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                        text=True, errors="replace", check=False)
    return idx, r, p, cmd, cp.returncode, cp.stdout, "OK" if cp.returncode == 0 else "READELF_FAILED"

results = []
with ThreadPoolExecutor(max_workers=min(8, os.cpu_count() or 1)) as pool:
    for result in pool.map(probe, enumerate(runtime), chunksize=32):
        results.append(result)
results.sort(key=lambda x: x[0])

command_rows = []
scan_rows = []
hit_rows = []
vendor_rows = []
nonclang_rows = []
fail_rows = []
for idx, r, p, cmd, rc, output, status in results:
    und = set()
    for line in output.splitlines():
        m = dynsym_re.match(line)
        if not m:
            continue
        raw = m.group(1).split()[0]
        base = raw.split("@", 1)[0]
        if base in fixed_by_arch.get(r["arch"], set()):
            und.add((base, raw))
    has_gcc = bool(re.search(r"\bGCC:\s*\(|\bGCC:\s", output))
    has_clang = "clang version" in output or "Android (" in output and "clang" in output.lower()
    if has_gcc and has_clang:
        producer = "MIXED_GCC_AND_CLANG_COMMENT"
    elif has_gcc:
        producer = "GCC_COMMENT_IDENTIFIED"
    elif has_clang:
        producer = "CLANG_COMMENT_IDENTIFIED"
    else:
        producer = "NOT_IDENTIFIABLE_FROM_ELF_COMMENT"
    command_rows.append({
        "sequence": idx + 1, "command": " ".join(cmd), "exit_code": rc,
        "status": status,
    })
    hits = sorted({x[0] for x in und})
    scan_rows.append({
        "repo_id": r["repo_id"], "package_nevra": nevra(r), "package_name": r["name"],
        "source_rpm": r["sourcerpm"], "arch": r["arch"], "elf_path": r["path"],
        "rpm_sha256": r["rpm_sha256"], "local_elf_path": str(p), "probe_status": status,
        "readelf_exit_code": rc, "fixed_point_und_count": len(hits),
        "fixed_point_und_symbols": ",".join(hits) if hits else "NONE",
        "compiler_producer_evidence": producer,
    })
    if status != "OK":
        fail_rows.append({
            "package_nevra": nevra(r), "arch": r["arch"], "elf_path": r["path"],
            "local_elf_path": str(p), "status": status, "exit_code": rc,
            "diagnostic": output.replace("\t", " ").replace("\n", " | ")[:2000] or "NONE",
        })
    for base, raw in sorted(und):
        hit_rows.append({
            "symbol": base, "raw_dynamic_symbol": raw, "elf_path": r["path"],
            "package_nevra": nevra(r), "package_name": r["name"], "source_rpm": r["sourcerpm"],
            "arch": r["arch"], "repo_id": r["repo_id"], "rpm_sha256": r["rpm_sha256"],
        })
    if r["name"] in vendor_names:
        vendor_rows.append({
            "vendor_package": r["name"], "package_nevra": nevra(r), "arch": r["arch"],
            "elf_path": r["path"], "probe_status": status, "fixed_point_und_count": len(hits),
            "fixed_point_und_symbols": ",".join(hits) if hits else "NONE",
            "compiler_producer_evidence": producer,
        })
    if producer in ("GCC_COMMENT_IDENTIFIED", "MIXED_GCC_AND_CLANG_COMMENT"):
        nonclang_rows.append({
            "package_nevra": nevra(r), "package_name": r["name"], "source_rpm": r["sourcerpm"],
            "arch": r["arch"], "elf_path": r["path"], "producer_class": producer,
            "third_party_or_prebuilt_status": "NOT_DETERMINABLE_FROM_ELF_COMMENT",
            "fixed_point_und_count": len(hits), "fixed_point_und_symbols": ",".join(hits) if hits else "NONE",
        })

write_tsv(OUT / "elf_probe_commands.tsv", ["sequence", "command", "exit_code", "status"], command_rows)
write_tsv(OUT / "runtime_elf_scan_results.tsv", list(scan_rows[0].keys()), scan_rows)
hit_fields = ["symbol", "raw_dynamic_symbol", "elf_path", "package_nevra", "package_name", "source_rpm", "arch", "repo_id", "rpm_sha256"]
write_tsv(OUT / "fixed_point_undefined_references.tsv", hit_fields, hit_rows)
write_tsv(OUT / "vendor_prebuilt_elf_scan.tsv", list(vendor_rows[0].keys()) if vendor_rows else ["vendor_package"], vendor_rows)
nonclang_fields = ["package_nevra", "package_name", "source_rpm", "arch", "elf_path", "producer_class", "third_party_or_prebuilt_status", "fixed_point_und_count", "fixed_point_und_symbols"]
write_tsv(OUT / "gcc_comment_identified_elfs.tsv", nonclang_fields, nonclang_rows)
fail_fields = ["package_nevra", "arch", "elf_path", "local_elf_path", "status", "exit_code", "diagnostic"]
write_tsv(OUT / "elf_probe_failures.tsv", fail_fields, fail_rows)

summary = []
for arch in (*ARCHES, "ALL"):
    subset = [x for x in scan_rows if arch == "ALL" or x["arch"] == arch]
    ok = [x for x in subset if x["probe_status"] == "OK"]
    h = [x for x in ok if x["fixed_point_und_count"]]
    arch_hits = [x for x in hit_rows if arch == "ALL" or x["arch"] == arch]
    summary.append({
        "arch": arch, "runtime_elf_denominator": len(subset), "successfully_scanned_elf": len(ok),
        "unavailable_or_failed_elf": len(subset) - len(ok),
        "fixed_point_export_symbol_count": sum(1 for a, _ in fixed if arch == "ALL" or a == arch),
        "referenced_fixed_point_symbol_count": len({x["symbol"] for x in arch_hits}),
        "referencing_elf_count": len(h), "referencing_binary_package_count": len({(x["package_nevra"]) for x in h}),
        "result": "ZERO_REFERENCES_CONFIRMED" if len(ok) == len(subset) and not h else ("REFERENCES_FOUND" if h else "UNDETERMINED"),
    })
write_tsv(OUT / "fixed_point_scan_summary.tsv", list(summary[0].keys()), summary)

original_gap = {"armv7l": 986, "aarch64": 25, "x86_64": 27}
corrected = []
for arch in ARCHES:
    s = next(x for x in summary if x["arch"] == arch)
    fixed_count = s["fixed_point_export_symbol_count"]
    if s["result"] == "ZERO_REFERENCES_CONFIRMED":
        corrected_count = original_gap[arch] - fixed_count
        rule = "ORIGINAL_GAP_MINUS_MEASURED_ZERO_REFERENCE_FIXED_POINT_FAMILY"
    elif s["result"] == "REFERENCES_FOUND":
        corrected_count = original_gap[arch]
        rule = "ORIGINAL_GAP_RETAINED_FIXED_POINT_FAMILY_HAS_REFERENCES"
    else:
        corrected_count = "UNDETERMINED"
        rule = "SCAN_INCOMPLETE_NO_NUMERIC_CORRECTION"
    corrected.append({
        "arch": arch, "original_missing_helper_count": original_gap[arch],
        "fixed_point_family_count": fixed_count,
        "referenced_fixed_point_symbol_count": s["referenced_fixed_point_symbol_count"],
        "corrected_missing_helper_count": corrected_count, "correction_rule": rule,
    })
write_tsv(OUT / "corrected_gap_summary.tsv", list(corrected[0].keys()), corrected)

print(f"runtime_elf={len(runtime)} success={sum(x['probe_status']=='OK' for x in scan_rows)} failures={len(fail_rows)}")
print(f"fixed_symbols={len(fixed)} hits={len(hit_rows)} vendor_elf={len(vendor_rows)} gcc_comment_elf={len(nonclang_rows)}")
for r in summary:
    print("SUMMARY", *(f"{k}={v}" for k, v in r.items()))
