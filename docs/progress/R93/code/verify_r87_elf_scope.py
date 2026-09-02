#!/usr/bin/env python3
import csv
import json
import re
from pathlib import Path

summary = Path("docs/progress/R87/tables/binary_symbol_summary.tsv").read_text(encoding="utf-8")
match = re.search(r"FAILURE:tmp/R11/extracted/[0-9a-f]{2}/([0-9a-f]{64})(/[^\t]+)\t(.+)", summary)
if not match:
    raise SystemExit("R87 failure row not found")
sha256, failed_path, reason = match.groups()
package = None
for shard in Path("tmp/R11/elf_scan_shards").glob("*.json"):
    record = json.loads(shard.read_text(encoding="utf-8"))
    if record["package"]["actual_sha256"] == sha256:
        package = record["package"]
        break
if package is None:
    raise SystemExit("failed package shard not found")
with Path("docs/progress/R92/tables/cpp_gap_binary_coverage.tsv").open(newline="", encoding="utf-8") as stream:
    coverage = list(csv.DictReader(stream, delimiter="\t"))
in_coverage = [row for row in coverage if row["source_rpm"] == package["sourcerpm"] and row["path"] == failed_path]
in_140 = any(row["source_rpm"] == package["sourcerpm"] for row in coverage)
row = {
    "r87_elf_records": 39166,
    "r87_parse_failures": 1,
    "failed_source_rpm": package["sourcerpm"],
    "failed_binary_package": package["name"],
    "failed_arch": package["arch"],
    "failed_path": failed_path,
    "failure_reason": reason,
    "r92_140_source_scope": "NO" if not in_140 else "YES",
    "r92_4374_row_scope": "NO" if not in_coverage else "YES",
}
target = Path("progress/R93/tables/r87_elf_failure_scope.tsv")
with target.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=tuple(row), delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerow(row)
for key, value in row.items():
    print(f"{key}={value}")
raise SystemExit(0 if not in_140 and not in_coverage else 2)
