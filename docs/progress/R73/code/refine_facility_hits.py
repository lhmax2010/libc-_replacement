#!/usr/bin/env python3
"""Refine R72's deliberately broad facility hits to explicit std:: uses.

This pass accepts explicit ``std::name`` expressions and ``using std::name``
declarations.  It intentionally does not guess that every unqualified word
``future`` denotes ``std::future``.
"""

from __future__ import annotations

import argparse
import csv
import re
from collections import defaultdict
from pathlib import Path


NAMES = {
    "condition_variable": ("condition_variable", "condition_variable_any"),
    "future": ("future", "shared_future", "async", "packaged_task", "promise"),
    "timed_mutex": ("timed_mutex", "recursive_timed_mutex"),
    "shared_mutex": ("shared_mutex", "shared_timed_mutex"),
    "wbuffer_convert": ("wbuffer_convert",),
}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--broad-hits", type=Path, required=True)
    parser.add_argument("--scan-status", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--summary", type=Path, required=True)
    args = parser.parse_args()

    roots: dict[str, Path] = {}
    with args.scan_status.open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["status"] == "SCAN_OK":
                roots[Path(row["root"]).name] = Path(row["root"])

    files: dict[str, set[str]] = defaultdict(set)
    package_names: dict[str, str] = {}
    with args.broad_hits.open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            files[row["source_rpm"]].add(row["matching_file"])
            package_names[row["source_rpm"]] = row["package_name"]

    patterns: dict[str, re.Pattern[str]] = {}
    for facility, names in NAMES.items():
        alternatives = "|".join(re.escape(name) for name in names)
        patterns[facility] = re.compile(
            rf"(?:(?:::)?std\s*::\s*(?:{alternatives})\b|using\s+(?:::)?std\s*::\s*(?:{alternatives})\b)"
        )
    patterns["sleep_until"] = re.compile(r"(?:::)?std\s*::\s*this_thread\s*::\s*sleep_until\b")

    rows: list[tuple[str, str, str, str, int, str]] = []
    found: dict[str, set[str]] = defaultdict(set)
    found_packages: dict[str, set[str]] = defaultdict(set)
    for source_rpm in sorted(files):
        root = roots[source_rpm]
        package_name = package_names[source_rpm]
        for rel in sorted(files[source_rpm]):
            path = root / rel
            try:
                lines = path.read_text(errors="replace").splitlines()
            except OSError:
                continue
            for line_number, line in enumerate(lines, 1):
                for facility, pattern in patterns.items():
                    if pattern.search(line):
                        rows.append((source_rpm, package_name, facility, rel, line_number, line.strip()))
                        found[facility].add(source_rpm)
                        found_packages[facility].add(package_name)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source_rpm", "package_name", "facility", "file", "line", "source_text"))
        writer.writerows(rows)

    all_rpms = set().union(*found.values()) if found else set()
    all_packages = set().union(*found_packages.values()) if found_packages else set()
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("facility", "source_rpm_count", "package_name_count", "interpretation"))
        for facility in patterns:
            writer.writerow((facility, len(found[facility]), len(found_packages[facility]), "explicit std:: use or using declaration"))
        writer.writerow(("ANY", len(all_rpms), len(all_packages), "deduplicated explicit std:: use"))

    print(f"explicit_source_rpms={len(all_rpms)}")
    print(f"explicit_package_names={len(all_packages)}")
    for facility in patterns:
        print(f"{facility}={len(found[facility])}/{len(found_packages[facility])}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
