#!/usr/bin/env python3
"""Prepare exact armv7l RPM manifest for Base-internal provider edges."""

import csv
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


edges = read("progress/R100/tables/armv7l_base_internal_provider_edges.tsv")
wanted = {row[key] for row in edges for key in ("consumer_rpm_sha256", "provider_rpm_sha256")}
records = {row["checksum"]: row for row in read("progress/R98/tables/base_binary_records.tsv")}
rows = []
for checksum in sorted(wanted):
    row = records[checksum]
    rows.append({
        "repo_id": "base", "kind": "binary", "name": row["name"], "arch": row["arch"],
        "epoch": row["epoch"], "version": row["version"], "release": row["release"],
        "sourcerpm": row["sourcerpm"], "location": row["location"],
        "checksum_type": "sha256", "checksum": checksum,
        "package_size": row["package_size"], "build_time": "NOT_AVAILABLE",
    })
fields = list(rows[0])
out = Path("progress/R100/tables/armv7l_internal_rpm_manifest.tsv")
with out.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(sorted(rows, key=lambda row: row["name"]))
cross = {row["checksum"] for row in read("progress/R100/tables/armv7l_cross_rpm_manifest.tsv")}
print(f"manifest_rows={len(rows)}")
print(f"bytes={sum(int(row['package_size']) for row in rows)}")
print(f"already_in_cross={len(wanted & cross)}")
print(f"additional_bytes={sum(int(records[c]['package_size']) for c in wanted-cross)}")
