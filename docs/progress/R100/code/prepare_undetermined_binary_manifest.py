#!/usr/bin/env python3
"""Prepare every exact binary RPM for currently undetermined R100 sources."""

import csv
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


wanted = {
    row["source_package"] for row in read("progress/R100/tables/base_cpp_package_decisions.tsv")
    if row["decision"] == "UNDETERMINED"
}
source_by_srpm = {row["location"]: row["name"] for row in read("progress/R98/tables/base_source_records.tsv")}
rows = []
for row in read("progress/R98/tables/base_binary_records.tsv"):
    if source_by_srpm.get(row["sourcerpm"]) not in wanted:
        continue
    rows.append({
        "repo_id": "base", "kind": "binary", "name": row["name"], "arch": row["arch"],
        "epoch": row["epoch"], "version": row["version"], "release": row["release"],
        "sourcerpm": row["sourcerpm"], "location": row["location"],
        "checksum_type": "sha256", "checksum": row["checksum"],
        "package_size": row["package_size"], "build_time": "NOT_AVAILABLE",
    })
fields = list(rows[0])
out = Path("progress/R100/tables/undetermined_binary_manifest.tsv")
with out.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(sorted(rows, key=lambda row: (row["name"], row["arch"])))
found = {source_by_srpm.get(row["sourcerpm"]) for row in rows}
print(f"wanted_sources={len(wanted)}")
print(f"represented_sources={len(found)}")
print(f"missing_sources={';'.join(sorted(wanted-found)) or 'NONE'}")
print(f"manifest_rows={len(rows)}")
print(f"bytes={sum(int(row['package_size']) for row in rows)}")
