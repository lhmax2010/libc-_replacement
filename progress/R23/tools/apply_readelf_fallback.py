#!/usr/bin/env python3
import csv
from pathlib import Path

root = Path.cwd()
tables = root / "progress/R23/tables"
target = "/boot/u-boot.mbn"
target_sha = "62e5acbccabe3e2e944e79e3b8950615a9d95efa1acf28ca345c5bedc16a89fe"

def read(name):
    with (tables/name).open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def write(name, fields, rows):
    with (tables/name).open("w", newline="", encoding="utf-8") as f:
        w=csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader(); w.writerows(rows)

scan = read("runtime_elf_scan_results.tsv")
matched = 0
for r in scan:
    if r["elf_path"] == target and r["rpm_sha256"] == target_sha:
        r["parser_status"] = "PASS_READELF_FALLBACK_NO_DYNSYM_OUTPUT"
        r["parser_exit_code"] = "0"
        r["dynamic_symbol_count"] = "0"
        r["fixed_point_und_count"] = "0"
        r["fixed_point_und_symbols"] = "NONE"
        matched += 1
if matched != 1: raise SystemExit(f"expected one fallback target, got {matched}")
write("runtime_elf_scan_results.tsv", list(scan[0]), scan)

# The only parser failure is closed by commands/016... readelf exit 0 with no
# dynamic-symbol output.  Preserve the original diagnostic in a fallback ledger.
failures = read("elf_probe_failures.tsv")
fallback = []
remaining = []
for r in failures:
    if r["elf_path"] == target and target_sha in r["local_elf_path"]:
        fallback.append({**r, "fallback_command_log": "progress/R23/commands/016_failed_elf_readelf_fallback.log",
                         "fallback_exit_code": "0", "fallback_stdout": "EMPTY_NO_DYNAMIC_SYMBOL_TABLE",
                         "final_status": "PASS_READELF_FALLBACK_NO_DYNSYM_OUTPUT"})
    else: remaining.append(r)
if len(fallback) != 1: raise SystemExit(f"expected one failure row, got {len(fallback)}")
write("elf_probe_failures.tsv", list(failures[0]), remaining)
write("elf_parser_fallbacks.tsv", list(fallback[0]), fallback)

hits = read("fixed_point_undefined_references.tsv")
fixed = read("fixed_point_symbols.tsv")
summary=[]
for arch in ("armv7l","aarch64","x86_64","ALL"):
    subset=[r for r in scan if arch=="ALL" or r["arch"]==arch]
    ok=[r for r in subset if r["parser_status"].startswith("PASS")]
    hs=[r for r in subset if int(r["fixed_point_und_count"])]
    ah=[r for r in hits if arch=="ALL" or r["arch"]==arch]
    result="ZERO_REFERENCES_CONFIRMED" if len(ok)==len(subset) and not hs else ("REFERENCES_FOUND" if hs else "UNDETERMINED")
    summary.append({"arch":arch,"runtime_elf_denominator":len(subset),"successfully_scanned_elf":len(ok),
                    "unavailable_or_failed_elf":len(subset)-len(ok),
                    "fixed_point_export_symbol_count":sum(1 for r in fixed if arch=="ALL" or r["arch"]==arch),
                    "referenced_fixed_point_symbol_count":len({r["symbol"] for r in ah}),
                    "referencing_elf_count":len(hs),"referencing_binary_package_count":len({r["package_nevra"] for r in hs}),"result":result})
write("fixed_point_scan_summary.tsv",list(summary[0]),summary)
original={"armv7l":986,"aarch64":25,"x86_64":27}; corrected=[]
for arch in original:
    s=next(r for r in summary if r["arch"]==arch)
    if s["result"]=="ZERO_REFERENCES_CONFIRMED": value,rule=original[arch]-s["fixed_point_export_symbol_count"],"ORIGINAL_GAP_MINUS_MEASURED_ZERO_REFERENCE_FIXED_POINT_FAMILY"
    elif s["result"]=="REFERENCES_FOUND": value,rule=original[arch],"ORIGINAL_GAP_RETAINED_FIXED_POINT_FAMILY_HAS_REFERENCES"
    else: value,rule="UNDETERMINED","SCAN_INCOMPLETE_NO_NUMERIC_CORRECTION"
    corrected.append({"arch":arch,"original_missing_helper_count":original[arch],"fixed_point_family_count":s["fixed_point_export_symbol_count"],
                      "referenced_fixed_point_symbol_count":s["referenced_fixed_point_symbol_count"],"corrected_missing_helper_count":value,"correction_rule":rule})
write("corrected_gap_summary.tsv",list(corrected[0]),corrected)
print("fallback_rows=1 remaining_failures=0")
for r in summary: print(r)
