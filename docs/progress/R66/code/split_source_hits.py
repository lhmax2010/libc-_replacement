#!/usr/bin/env python3
"""Split the large source-compatibility hit table into reviewable TSV parts."""

from __future__ import annotations

import argparse
import csv
import pathlib


MAX_BYTES = 4_500_000


def safe_name(scope: str) -> str:
    return scope.lower().replace("_", "-")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=pathlib.Path)
    parser.add_argument("output_dir", type=pathlib.Path)
    args = parser.parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)

    handles = {}
    writers = {}
    sizes = {}
    parts = {}
    fieldnames = None
    try:
        with args.input.open(newline="", encoding="utf-8") as source:
            reader = csv.DictReader(source, delimiter="\t")
            fieldnames = reader.fieldnames
            if not fieldnames:
                raise ValueError("input has no header")
            header_size = len(("\t".join(fieldnames) + "\n").encode())
            for row in reader:
                scope = row["scope"]
                encoded_size = len(("\t".join(row[name] for name in fieldnames) + "\n").encode())
                key = (scope, parts.get(scope, 1))
                if key in sizes and sizes[key] + encoded_size > MAX_BYTES:
                    handles[key].close()
                    parts[scope] = key[1] + 1
                    key = (scope, parts[scope])
                if key not in writers:
                    path = args.output_dir / f"source_compat_hits_{safe_name(scope)}_{key[1]:02d}.tsv"
                    handle = path.open("w", newline="", encoding="utf-8")
                    handles[key] = handle
                    writer = csv.DictWriter(handle, fieldnames=fieldnames, delimiter="\t", lineterminator="\n")
                    writer.writeheader()
                    writers[key] = writer
                    sizes[key] = header_size
                writers[key].writerow(row)
                sizes[key] += encoded_size
    finally:
        for handle in handles.values():
            if not handle.closed:
                handle.close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
