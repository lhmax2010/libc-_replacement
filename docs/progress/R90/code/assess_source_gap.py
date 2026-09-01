#!/usr/bin/env python3
"""Quantify R87's source-corpus gap without unpacking or scanning it."""

from __future__ import annotations

import csv
import sys
from collections import defaultdict
from pathlib import Path


if len(sys.argv) != 7:
    raise SystemExit(
        "usage: assess_source_gap.py DOWNLOAD_STATUS R32_STATUS ELF_INVENTORY "
        "METRICS_OUT LARGEST_OUT DRIFT_OUT"
    )

download_path, r32_path, elf_path, metrics_path, largest_path, drift_path = map(
    Path, sys.argv[1:]
)


def read(path: Path) -> list[dict[str, str]]:
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


all_source = read(download_path)
r32 = read(r32_path)
elfs = read(elf_path)
r32_ok_rows = [row for row in r32 if row["status"] == "SCAN_OK"]
covered = {row["source_rpm"] for row in r32_ok_rows}
all_names = {row["location"] for row in all_source}
exact_covered = all_names & covered
outside_r11_snapshot = covered - all_names
missing = all_names - exact_covered

if len(all_source) != 1450 or len(covered) != 371:
    raise SystemExit(
        f"unexpected denominator: all={len(all_source)} covered={len(covered)} "
        f"missing={len(missing)}"
    )

source_bytes = {row["location"]: int(row["package_size"]) for row in all_source}
source_path_present = {
    row["location"]: Path(row["target"]).is_file() for row in all_source
}

runtime = defaultdict(bool)
cpp = defaultdict(bool)
for row in elfs:
    name = row["sourcerpm"]
    if row["runtime_elf"] == "YES":
        runtime[name] = True
        if row["cpp_indicator"] == "YES":
            cpp[name] = True


def count(group: set[str], predicate) -> int:
    return sum(1 for name in group if predicate(name))


metrics = [
    ("all_source_records", len(all_names), "source RPM records"),
    ("r87_source_scan_ok", len(covered), "source RPM records"),
    ("reported_arithmetic_gap_1450_minus_371", len(all_names) - len(covered), "source RPM records"),
    ("r87_exact_r11_snapshot_records_scanned", len(exact_covered), "source RPM records"),
    ("r87_scanned_records_outside_r11_snapshot", len(outside_r11_snapshot), "source RPM records"),
    ("exact_r11_snapshot_gap", len(missing), "source RPM records"),
    ("all_srpm_bytes", sum(source_bytes.values()), "compressed bytes"),
    ("exact_covered_srpm_bytes", sum(source_bytes[n] for n in exact_covered), "compressed bytes"),
    ("missing_srpm_bytes", sum(source_bytes[n] for n in missing), "compressed bytes"),
    ("missing_srpm_files_present", count(missing, source_path_present.get), "source RPM records"),
    ("exact_covered_with_runtime_elf", count(exact_covered, runtime.get), "source RPM records"),
    ("missing_with_runtime_elf", count(missing, runtime.get), "source RPM records"),
    ("exact_covered_with_cpp_indicator", count(exact_covered, cpp.get), "source RPM records"),
    ("missing_with_cpp_indicator", count(missing, cpp.get), "source RPM records"),
    ("exact_covered_without_cpp_indicator", count(exact_covered, lambda n: not cpp[n]), "source RPM records"),
    ("missing_without_cpp_indicator", count(missing, lambda n: not cpp[n]), "source RPM records"),
]

metrics_path.parent.mkdir(parents=True, exist_ok=True)
with metrics_path.open("w", newline="") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(("metric", "value", "unit"))
    writer.writerows(metrics)

largest = sorted(missing, key=source_bytes.get, reverse=True)
with largest_path.open("w", newline="") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(("rank", "source_rpm", "compressed_bytes", "runtime_elf",
                     "cpp_indicator", "local_file_present"))
    for rank, name in enumerate(largest[:20], 1):
        writer.writerow((rank, name, source_bytes[name], "YES" if runtime[name] else "NO",
                         "YES" if cpp[name] else "NO",
                         "YES" if source_path_present[name] else "NO"))

r32_name_by_source = {
    row["source_rpm"]: row["package_name"] for row in r32_ok_rows
}
r11_by_package = defaultdict(list)
for row in all_source:
    r11_by_package[row["name"]].append(row["location"])
with drift_path.open("w", newline="") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(("r87_scanned_source_rpm", "package_name", "r11_snapshot_source_rpms"))
    for source_rpm in sorted(outside_r11_snapshot):
        package_name = r32_name_by_source[source_rpm]
        writer.writerow((source_rpm, package_name,
                         ";".join(sorted(r11_by_package.get(package_name, []))) or "NOT_AVAILABLE"))

for metric in metrics:
    print("\t".join(map(str, metric)))
print("largest_missing_source_rpms")
for rank, name in enumerate(largest[:20], 1):
    print(rank, name, source_bytes[name], "CPP" if cpp[name] else "NO_CPP_INDICATOR")
print("r87_scanned_records_outside_r11_snapshot")
for name in sorted(outside_r11_snapshot):
    print(name, r32_name_by_source[name], ";".join(r11_by_package.get(r32_name_by_source[name], [])) or "NOT_AVAILABLE")
