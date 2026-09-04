#!/usr/bin/env python3
"""Read-only cross-check of the proposed HQ reply against R110 tables."""

from __future__ import annotations

import csv
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
R110 = ROOT / "docs" / "progress" / "R110"


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


tables = R110 / "tables"
reported = read_tsv(tables / "reported_50_audit.tsv")
clauses = read_tsv(tables / "strict_clause_sites_original.tsv")
propagating = read_tsv(tables / "strict_rethrow_sites_original.tsv")
current = read_tsv(tables / "current_rethrow_sites.tsv")
metrics = {row["metric"]: row["value"] for row in read_tsv(tables / "audit_metrics.tsv")}

logical_keys = {
    (
        row["package_name"],
        row["file"],
        row["line"],
        row["clause_text"],
        row["rethrow_kind"],
    )
    for row in propagating
}
reported_logical_keys = {
    (row["package_name"], row["file"], row["line"])
    for row in reported
    if row["row_nature"] == "ACTUAL_CATCH_CLAUSE"
}

print("R110_METRICS")
for key, value in metrics.items():
    print(f"{key}={value}")
print(f"reported_rows={len(reported)}")
print(f"reported_actual_clauses={sum(r['row_nature'] == 'ACTUAL_CATCH_CLAUSE' for r in reported)}")
print(f"reported_false_token_lines={sum(r['row_nature'] != 'ACTUAL_CATCH_CLAUSE' for r in reported)}")
print(f"strict_clauses={len(clauses)}")
print(f"strict_propagating={len(propagating)}")
print(f"strict_rethrow_kinds={dict(sorted(Counter(r['rethrow_kind'] for r in clauses).items()))}")
print(f"strict_logical_sites={len(logical_keys)}")
print(f"reported_actual_logical_sites={len(reported_logical_keys)}")
print(f"current_historical_positive_package_set={len(current)}")
print(f"source_rpm_instances={len({r['source_rpm'] for r in reported})}")
print(f"deduplicated_package_names={len({r['package_name'] for r in reported})}")

print("FALSE_POSITIVE_ROWS")
for row in reported:
    if row["row_nature"] != "ACTUAL_CATCH_CLAUSE":
        print("\t".join(row[key] for key in (
            "source_rpm", "file", "line", "actual_code_line", "context", "r32_status"
        )))

print("ATTACHMENT_ROWS")
for name in (
    "reported_50_audit.tsv",
    "strict_clause_sites_original.tsv",
    "strict_rethrow_sites_original.tsv",
    "current_rethrow_sites.tsv",
    "source_instance_duplicates.tsv",
):
    print(f"{name}={len(read_tsv(tables / name))}")
