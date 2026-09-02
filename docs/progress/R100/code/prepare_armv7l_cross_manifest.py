#!/usr/bin/env python3
"""Prepare an exact, bounded armv7l RPM manifest for R100 edge evidence."""

import csv
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


edges = [
    row for row in read("progress/R98/tables/unified_to_base_cpp_library_edges.tsv")
    if row["arch"] == "armv7l"
]
edge_output = Path("progress/R100/tables/armv7l_unified_to_base_edges.tsv")
with edge_output.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, edges[0].keys(), delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(edges)
wanted = {
    row[key]
    for row in edges
    for key in ("consumer_rpm_sha256", "provider_rpm_sha256")
}
records = []
for repo_id, path in (
    ("base", "progress/R98/tables/base_binary_records.tsv"),
    ("unified", "progress/R98/tables/unified_binary_records.tsv"),
):
    for row in read(path):
        if row["checksum"] not in wanted:
            continue
        records.append({
            "repo_id": repo_id,
            "kind": "binary",
            "name": row["name"],
            "arch": row["arch"],
            "epoch": row["epoch"],
            "version": row["version"],
            "release": row["release"],
            "sourcerpm": row["sourcerpm"],
            "location": row["location"],
            "checksum_type": "sha256",
            "checksum": row["checksum"],
            "package_size": row["package_size"],
            "build_time": "NOT_AVAILABLE",
        })

fields = [
    "repo_id", "kind", "name", "arch", "epoch", "version", "release",
    "sourcerpm", "location", "checksum_type", "checksum", "package_size",
    "build_time",
]
out = Path("progress/R100/tables/armv7l_cross_rpm_manifest.tsv")
with out.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(sorted(records, key=lambda row: (row["repo_id"], row["name"])))

print(f"edge_rows={len(edges)}")
print(f"wanted_rpms={len(wanted)}")
print(f"manifest_rows={len(records)}")
print(f"bytes={sum(int(row['package_size']) for row in records)}")
missing = wanted - {row["checksum"] for row in records}
print(f"missing={len(missing)}")
if missing:
    raise SystemExit(1)
