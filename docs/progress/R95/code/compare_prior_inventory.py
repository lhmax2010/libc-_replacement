#!/usr/bin/env python3
"""Measure which actual R95 Base RPMs can reuse R11 ELF evidence by exact checksum."""

import argparse
import csv
import sys
from collections import defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    with path.open("w", encoding="utf-8", newline="") as stream:
        out = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        out.writeheader()
        out.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--actual", type=Path, required=True)
    ap.add_argument("--prior-binary", type=Path, required=True)
    ap.add_argument("--prior-elf", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()

    actual = read(args.actual)
    prior_binary = read(args.prior_binary)
    prior_elf = read(args.prior_elf)
    prior_sha = {row["checksum"] for row in prior_binary}
    elf_by_sha = defaultdict(list)
    for row in prior_elf:
        elf_by_sha[row["rpm_sha256"]].append(row)

    rows = []
    exact = 0
    exact_with_elf = 0
    exact_with_cpp_elf = 0
    for row in actual:
        sha = row["checksum"]
        matched = sha in prior_sha
        elfs = elf_by_sha.get(sha, [])
        cpp_elfs = [e for e in elfs if e["runtime_elf"] == "YES" and e["cpp_indicator"] == "YES"]
        exact += int(matched)
        exact_with_elf += int(bool(elfs))
        exact_with_cpp_elf += int(bool(cpp_elfs))
        rows.append({
            "name": row["name"], "arch": row["arch"], "version": row["version"],
            "release": row["release"], "sourcerpm": row["sourcerpm"],
            "rpm_sha256": sha, "exact_r11_rpm_match": "YES" if matched else "NO",
            "r11_elf_count": str(len(elfs)), "r11_cpp_runtime_elf_count": str(len(cpp_elfs)),
        })
    fields = ["name", "arch", "version", "release", "sourcerpm", "rpm_sha256",
              "exact_r11_rpm_match", "r11_elf_count", "r11_cpp_runtime_elf_count"]
    write(args.output, fields, rows)
    print(f"actual_base_binary_records={len(actual)}")
    print(f"exact_r11_rpm_checksum_matches={exact}")
    print(f"exact_matches_with_r11_elf_inventory={exact_with_elf}")
    print(f"exact_matches_with_r11_cpp_runtime_elf={exact_with_cpp_elf}")


if __name__ == "__main__":
    main()
