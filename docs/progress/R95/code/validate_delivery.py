#!/usr/bin/env python3
"""Mechanical consistency checks for the R95 delivery."""

import csv
from collections import Counter
from pathlib import Path


ROOT = Path("docs/progress/R95")


def rows(name):
    with (ROOT / "tables" / name).open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def expect(label, actual, expected):
    print(f"{label}={actual}")
    if actual != expected:
        raise SystemExit(f"FAIL {label}: expected {expected}, got {actual}")


source = rows("base_cpp_source_records_exact.tsv")
binary = rows("base_cpp_binary_records_exact.tsv")
elfs = rows("base_cpp_runtime_elfs_exact.tsv")
images = rows("base_cpp_image_membership_exact.tsv")
logical = rows("actual_cross_logical_edge_classification.tsv")
unresolved = rows("actual_cross_edge_unresolved.tsv")
migration = rows("migration_483_actual_project_mapping.tsv")

expect("base_cpp_source_records", len(source), 65)
expect("base_cpp_source_names", len({r["source_name"] for r in source}), 38)
expect("base_libstdcpp_source_records", sum(r["libstdcpp_linked"] == "YES" for r in source), 59)
expect("base_libstdcpp_source_names", len({r["source_name"] for r in source if r["libstdcpp_linked"] == "YES"}), 35)
expect("base_cpp_binary_records", len(binary), 276)
expect("base_cpp_binary_names", len({r["binary_name"] for r in binary}), 106)
linked_binary = [r for r in binary if int(r["libstdcpp_needed_elf_count"])]
expect("base_libstdcpp_binary_records", len(linked_binary), 245)
expect("base_libstdcpp_binary_names", len({r["binary_name"] for r in linked_binary}), 96)
expect("base_cpp_runtime_elfs", len(elfs), 2292)
expect("image_cpp_rows", len(images), 114)
expect("image_cpp_binary_names", len({r["binary_name"] for r in images}), 15)
expect("image_cpp_source_records", len({r["sourcerpm"] for r in images}), 17)
expect("image_cpp_source_names", len({r["source_name"] for r in images}), 11)
expect("image_manifest_count", len(list((ROOT / "inputs/image_packages").glob("*.packages"))), 14)
expect("image_manifest_rows", sum(len(p.read_text(encoding="utf-8").splitlines()) for p in (ROOT / "inputs/image_packages").glob("*.packages")), 5995)
expect("logical_edges", len(logical), 65)
expect("logical_edge_classes", Counter(r["classification"] for r in logical), Counter({
    "LAYOUT_SENSITIVE_STD_TYPE": 29,
    "CPP_ABI_NO_STD_LAYOUT_PROVEN": 18,
    "PURE_C_INTERFACE": 17,
    "OTHER_NO_SYMBOL_INTERSECTION": 1,
}))
expect("unresolved_edges", len(unresolved), 0)
expect("migration_rows", len(migration), 483)
expect("migration_unique_names", len({r["package_name"] for r in migration}), 455)
expect("migration_membership", Counter(r["actual_project_membership"] for r in migration), Counter({
    "BASE_ONLY_BY_NAME": 63,
    "UNIFIED_ONLY_BY_NAME": 416,
    "NEITHER_CURRENT_SNAPSHOT_BY_NAME": 4,
}))

required = [
    "README.md", "REPORT.md", "SUMMARY_COUNTS.tsv", "OPTIONS.tsv",
    "evidence/KEY_EVIDENCE.md", "inputs/actual_repositories.tsv",
    "code/reused_scan_binary_rpms.py",
]
for name in required:
    if not (ROOT / name).is_file():
        raise SystemExit(f"FAIL missing {name}")
print("required_files=PASS")

report = (ROOT / "REPORT.md").read_text(encoding="utf-8")
for marker in ("NOT_OBSERVED", "NOT_AVAILABLE", "不作推荐", "未实施迁移"):
    if marker not in report:
        raise SystemExit(f"FAIL report marker absent: {marker}")
print("report_markers=PASS")
print("delivery_validation=PASS")
