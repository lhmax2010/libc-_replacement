#!/usr/bin/env python3
import argparse
import csv
from collections import defaultdict
from pathlib import Path


def read(path):
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--sources", required=True, type=Path)
    parser.add_argument("--r32-status", required=True, type=Path)
    parser.add_argument("--elf-inventory", required=True, type=Path)
    parser.add_argument("--replacement-root", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()
    sources = read(args.sources)
    r32 = read(args.r32_status)
    elfs = read(args.elf_inventory)
    scanned = {r["source_rpm"] for r in r32 if r["status"] == "SCAN_OK"}
    frozen = {r["location"] for r in sources}
    exact_scanned = frozen & scanned
    gap = frozen - exact_scanned
    elf_by_source = defaultdict(list)
    cpp_sources = set()
    for row in elfs:
        elf_by_source[row["sourcerpm"]].append(row)
        if row["cpp_indicator"] == "YES":
            cpp_sources.add(row["sourcerpm"])
    no_cpp = sorted(gap - cpp_sources)
    if (len(sources), len(scanned), len(exact_scanned), len(gap), len(no_cpp)) != (1450, 371, 345, 1105, 965):
        raise RuntimeError("frozen denominator mismatch")
    by_name = {r["location"]: r for r in sources}
    rows = []
    for identity in no_cpp:
        source = by_name[identity]
        local = args.replacement_root / source["target"]
        related = elf_by_source[identity]
        rows.append({
            "source_rpm": identity,
            "package_name": source["name"],
            "compressed_bytes": source["package_size"],
            "sha256": source["actual_sha256"],
            "local_path": str(local),
            "download_result": source["result"],
            "local_file_present": "YES" if local.is_file() else "NO",
            "published_elf_rows": len(related),
            "runtime_elf_rows": sum(r["runtime_elf"] == "YES" for r in related),
            "cpp_indicator_elf_rows": sum(r["cpp_indicator"] == "YES" for r in related),
        })
    fields = list(rows[0])
    write(args.output, fields, rows)
    metrics = [
        ("frozen_source_records", len(sources)),
        ("r32_scan_ok_records", len(scanned)),
        ("exact_frozen_records_already_scanned", len(exact_scanned)),
        ("exact_frozen_gap", len(gap)),
        ("cpp_indicator_gap_records_already_addressed_by_R92", len(gap & cpp_sources)),
        ("no_cpp_indicator_gap_records", len(rows)),
        ("no_cpp_gap_compressed_bytes", sum(int(r["compressed_bytes"]) for r in rows)),
        ("no_cpp_gap_files_present", sum(r["local_file_present"] == "YES" for r in rows)),
        ("no_cpp_gap_download_pass", sum(r["download_result"] == "PASS" for r in rows)),
        ("no_cpp_gap_published_elf_rows", sum(int(r["published_elf_rows"]) for r in rows)),
        ("no_cpp_gap_cpp_indicator_elf_rows", sum(int(r["cpp_indicator_elf_rows"]) for r in rows)),
    ]
    write(args.summary, ("metric", "value"), [{"metric": k, "value": v} for k, v in metrics])
    for key, value in metrics:
        print(f"{key}={value}")
    return 0 if all(r["local_file_present"] == "YES" and r["download_result"] == "PASS" for r in rows) else 2


if __name__ == "__main__":
    raise SystemExit(main())
