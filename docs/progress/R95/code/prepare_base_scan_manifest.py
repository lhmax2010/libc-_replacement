#!/usr/bin/env python3
"""Prepare exact actual-Base RPM manifest for the bounded runtime candidate scan."""

import argparse
import csv
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--records", type=Path, required=True)
    ap.add_argument("--candidates", type=Path, required=True)
    ap.add_argument("--invert", action="store_true", help="select records not present in candidates")
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()
    wanted = {row["rpm_sha256"] for row in read(args.candidates)}
    rows = []
    for row in read(args.records):
        selected = row["checksum"] in wanted
        if args.invert:
            selected = not selected
        if not selected:
            continue
        rows.append({
            "repo_id": "actual_base", "kind": "binary", "name": row["name"], "arch": row["arch"],
            "epoch": row["epoch"], "version": row["version"], "release": row["release"],
            "sourcerpm": row["sourcerpm"], "location": row["location"], "checksum_type": "sha256",
            "checksum": row["checksum"], "package_size": row["package_size"], "build_time": "NOT_AVAILABLE",
        })
    fields = ["repo_id", "kind", "name", "arch", "epoch", "version", "release", "sourcerpm",
              "location", "checksum_type", "checksum", "package_size", "build_time"]
    with args.output.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(sorted(rows, key=lambda r: (r["arch"], r["name"], r["version"], r["release"])))
    print(f"candidate_keys={len(wanted)} invert={args.invert} selected={len(rows)}")
    print(f"bytes={sum(int(r['package_size']) for r in rows)}")
    expected = len(read(args.records)) - len(wanted) if args.invert else len(wanted)
    print(f"expected={expected} missing={expected-len(rows)}")
    if expected != len(rows):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
