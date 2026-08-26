#!/usr/bin/env python3
"""Inventory cancellation-related source near R72 affected-facility hits.

The script deliberately does not label a package safe merely because no
pthread cancellation token is present.  Cancellation can be initiated by a
different translation unit or dependency.  Its output is evidence for manual
classification, not an automatic risk verdict.
"""

from __future__ import annotations

import argparse
import csv
import re
from collections import defaultdict
from pathlib import Path


CANCEL_PATTERNS = {
    "cancel_request": re.compile(r"\bpthread_cancel\s*\("),
    "cancel_configuration": re.compile(
        r"\bpthread_(?:setcancelstate|setcanceltype|testcancel|cleanup_push|cleanup_pop)\b"
    ),
    "thread_exit": re.compile(r"\bpthread_exit\s*\("),
}


def load_roots(path: Path) -> dict[str, tuple[str, Path]]:
    result: dict[str, tuple[str, Path]] = {}
    with path.open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["status"] == "SCAN_OK":
                result[Path(row["root"]).name] = (row["package_name"], Path(row["root"]))
    return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--hits", type=Path, required=True)
    parser.add_argument("--scan-status", type=Path, required=True)
    parser.add_argument("--inventory", type=Path, required=True)
    parser.add_argument("--cancel-hits", type=Path, required=True)
    args = parser.parse_args()

    roots = load_roots(args.scan_status)
    facilities: dict[str, set[str]] = defaultdict(set)
    facility_files: dict[str, set[str]] = defaultdict(set)
    with args.hits.open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            facilities[row["source_rpm"]].add(row["facility"])
            facility_files[row["source_rpm"]].add(row["matching_file"])

    cancellation_rows: list[tuple[str, str, str, int, str]] = []
    cancellation_kinds: dict[str, set[str]] = defaultdict(set)
    cancellation_files: dict[str, set[str]] = defaultdict(set)
    for source_rpm in sorted(facilities):
        package_name, root = roots[source_rpm]
        for path in root.rglob("*"):
            if not path.is_file() or path.suffix not in {
                ".c", ".cc", ".cp", ".cpp", ".cxx", ".C", ".h", ".hh",
                ".hpp", ".hxx", ".ipp", ".tcc", ".inl", ".ixx", ".cppm",
            }:
                continue
            try:
                lines = path.read_text(errors="replace").splitlines()
            except OSError:
                continue
            for line_number, line in enumerate(lines, 1):
                for kind, pattern in CANCEL_PATTERNS.items():
                    if pattern.search(line):
                        rel = str(path.relative_to(root))
                        cancellation_rows.append((source_rpm, package_name, rel, line_number, kind))
                        cancellation_kinds[source_rpm].add(kind)
                        cancellation_files[source_rpm].add(rel)

    args.inventory.parent.mkdir(parents=True, exist_ok=True)
    with args.inventory.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(
            (
                "source_rpm", "package_name", "facilities", "facility_file_count",
                "cancellation_kinds", "cancellation_file_count", "automatic_bucket",
            )
        )
        for source_rpm in sorted(facilities):
            package_name, _ = roots[source_rpm]
            kinds = cancellation_kinds[source_rpm]
            if "cancel_request" in kinds:
                bucket = "DIRECT_CANCEL_TOKEN_REVIEW_REQUIRED"
            elif kinds:
                bucket = "CANCEL_LIFECYCLE_TOKEN_REVIEW_REQUIRED"
            else:
                bucket = "NO_INTERNAL_CANCEL_TOKEN_OBSERVED"
            writer.writerow(
                (
                    source_rpm,
                    package_name,
                    ",".join(sorted(facilities[source_rpm])),
                    len(facility_files[source_rpm]),
                    ",".join(sorted(kinds)) or "none",
                    len(cancellation_files[source_rpm]),
                    bucket,
                )
            )

    with args.cancel_hits.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source_rpm", "package_name", "file", "line", "kind"))
        writer.writerows(cancellation_rows)

    print(f"affected_source_rpms={len(facilities)}")
    print(f"direct_cancel_source_rpms={sum('cancel_request' in cancellation_kinds[x] for x in facilities)}")
    print(f"lifecycle_only_source_rpms={sum(bool(cancellation_kinds[x]) and 'cancel_request' not in cancellation_kinds[x] for x in facilities)}")
    print(f"no_internal_cancel_token_source_rpms={sum(not cancellation_kinds[x] for x in facilities)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
