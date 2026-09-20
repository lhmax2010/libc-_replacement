#!/usr/bin/env python3
"""Prepare exact source-RPM inputs for GCC override spec confirmation."""

import csv
from pathlib import Path

TABLES = Path("progress/R98/tables")
WANTED = {
    ("base", "glibc"),
    ("unified", "lapack"), ("unified", "nntrainer"),
    ("unified", "onnxruntime"), ("unified", "openblas"), ("unified", "yaca"),
}
rows = []
for project in ("base", "unified"):
    with (TABLES / f"{project}_source_records.tsv").open(encoding="utf-8", newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if (project, row["name"]) not in WANTED:
                continue
            rows.append({
                "repo_id": f"{project}_source", "kind": "source", "name": row["name"],
                "arch": row["arch"], "epoch": row["epoch"], "version": row["version"],
                "release": row["release"], "sourcerpm": row["location"],
                "location": row["location"], "checksum_type": "sha256",
                "checksum": row["checksum"], "package_size": row["package_size"],
                "build_time": "NOT_AVAILABLE",
            })
fields = list(rows[0])
with (TABLES / "gcc_source_rpm_manifest.tsv").open("w", encoding="utf-8", newline="") as stream:
    out = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    out.writeheader()
    out.writerows(rows)
print(f"source_rpms={len(rows)}")
print(f"source_bytes={sum(int(r['package_size']) for r in rows)}")
print("sources=" + ";".join(r["name"] for r in rows))
