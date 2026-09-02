#!/usr/bin/env python3
"""Create the minimal exact-RPM download set for Unified -> Base C++ edge inspection."""

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
    ap.add_argument("--edges", type=Path, required=True)
    ap.add_argument("--base", type=Path, required=True)
    ap.add_argument("--unified", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()
    edges = read(args.edges)
    wanted = {
        "actual_base": {r["provider_rpm_sha256"] for r in edges},
        "actual_unified": {r["consumer_rpm_sha256"] for r in edges},
    }
    fields = ["repo_id", "kind", "name", "arch", "epoch", "version", "release", "sourcerpm",
              "location", "checksum_type", "checksum", "package_size", "build_time"]
    rows = []
    for repo_id, path in (("actual_base", args.base), ("actual_unified", args.unified)):
        for row in read(path):
            if row["checksum"] not in wanted[repo_id]:
                continue
            rows.append({
                "repo_id": repo_id, "kind": "binary", "name": row["name"], "arch": row["arch"],
                "epoch": row["epoch"], "version": row["version"], "release": row["release"],
                "sourcerpm": row["sourcerpm"], "location": row["location"],
                "checksum_type": "sha256", "checksum": row["checksum"],
                "package_size": row["package_size"], "build_time": "NOT_AVAILABLE",
            })
    rows.sort(key=lambda r: (r["repo_id"], r["arch"], r["name"], r["version"], r["release"]))
    with args.output.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    missing = sum(len(wanted[k]) for k in wanted) - len(rows)
    print(f"selected_rpms={len(rows)}")
    print(f"selected_bytes={sum(int(r['package_size']) for r in rows)}")
    print(f"missing_metadata_rows={missing}")
    if missing:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
