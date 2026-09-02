#!/usr/bin/env python3
"""Prepare exact Base Toolchain RPM manifest for packages containing .a files."""

import csv
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


records = {row["checksum"]: row for row in read("progress/R98/tables/base_binary_records.tsv")}
carrier_rows = [
    row for row in read("progress/R100/tables/base_filelist_cpp_carriers.tsv")
    if int(row["static_archive_count"]) > 0
]
rows = []
for carrier in carrier_rows:
    row = records[carrier["rpm_sha256"]]
    rows.append({
        "repo_id": "base", "kind": "binary", "name": row["name"],
        "arch": row["arch"], "epoch": row["epoch"], "version": row["version"],
        "release": row["release"], "sourcerpm": row["sourcerpm"],
        "location": row["location"], "checksum_type": "sha256",
        "checksum": row["checksum"], "package_size": row["package_size"],
        "build_time": "NOT_AVAILABLE",
    })
fields = list(rows[0])
out = Path("progress/R100/tables/static_rpm_manifest.tsv")
with out.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(sorted(rows, key=lambda row: (row["name"], row["arch"])))
print(f"manifest_rows={len(rows)}")
print(f"bytes={sum(int(row['package_size']) for row in rows)}")
