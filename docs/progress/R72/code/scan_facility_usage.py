#!/usr/bin/env python3
"""Count source-RPMs containing lexical uses of affected libc++ facilities.

This is deliberately an impact approximation.  A token hit proves only that
the extracted source mentions a facility; it does not prove that the code is
built, migrated to libc++, executed, or cancelled while blocked there.
"""

from __future__ import annotations

import argparse
import csv
import re
import subprocess
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path


FACILITIES = {
    "condition_variable": r"\b(?:std::)?condition_variable(?:_any)?\b",
    "future": r"\b(?:std::)?(?:future|shared_future|async|packaged_task|promise)\b",
    "timed_mutex": r"\b(?:std::)?(?:timed_mutex|recursive_timed_mutex)\b",
    "shared_mutex": r"\b(?:std::)?(?:shared_mutex|shared_timed_mutex)\b",
    "sleep_until": r"\b(?:std::)?this_thread\s*::\s*sleep_until\b",
    "wbuffer_convert": r"\b(?:std::)?wbuffer_convert\b",
}

SOURCE_GLOBS = (
    "*.c", "*.cc", "*.cp", "*.cpp", "*.cxx", "*.C", "*.h", "*.hh",
    "*.hpp", "*.hxx", "*.ipp", "*.tcc", "*.inl", "*.ixx", "*.cppm",
)


def load_roots(scan_status: Path) -> dict[str, tuple[str, Path]]:
    roots: dict[str, tuple[str, Path]] = {}
    with scan_status.open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["status"] == "SCAN_OK":
                roots[Path(row["root"]).name] = (row["package_name"], Path(row["root"]))
    return roots


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--scan-status", required=True, type=Path)
    parser.add_argument("--hits", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()

    roots = load_roots(args.scan_status)
    args.hits.parent.mkdir(parents=True, exist_ok=True)
    matched: dict[str, set[str]] = {name: set() for name in FACILITIES}
    matched_packages: dict[str, set[str]] = {name: set() for name in FACILITIES}
    hit_rows: list[tuple[str, str, str, str]] = []

    combined = "(?:" + ")|(?:".join(FACILITIES.values()) + ")"
    compiled = {name: re.compile(pattern) for name, pattern in FACILITIES.items()}

    def scan_one(item: tuple[str, tuple[str, Path]]) -> tuple[str, list[str], str]:
        source_rpm, (_, root) = item
        command = ["rg", "-l", "--threads", "1", "--no-messages", "--pcre2", combined]
        for glob in SOURCE_GLOBS:
            command.extend(["-g", glob])
        command.append(str(root))
        try:
            proc = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=300)
        except subprocess.TimeoutExpired:
            return source_rpm, [], "TIMEOUT_300S"
        if proc.returncode not in (0, 1):
            return source_rpm, [], f"RG_EXIT_{proc.returncode}:{proc.stderr.strip()}"
        return source_rpm, sorted(set(proc.stdout.splitlines())), "OK"

    failures: list[tuple[str, str]] = []
    with ThreadPoolExecutor(max_workers=2) as executor:
        futures = [executor.submit(scan_one, item) for item in roots.items()]
        for future in as_completed(futures):
            source_rpm, raw_paths, status = future.result()
            if status != "OK":
                failures.append((source_rpm, status))
                continue
            package_name, root = roots[source_rpm]
            for raw_path in raw_paths:
                path = Path(raw_path)
                try:
                    contents = path.read_text(errors="replace")
                except OSError as error:
                    failures.append((source_rpm, f"READ_ERROR:{path}:{error}"))
                    continue
                rel_file = path.relative_to(root)
                for facility, pattern in compiled.items():
                    if pattern.search(contents):
                        matched[facility].add(source_rpm)
                        matched_packages[facility].add(package_name)
                        hit_rows.append((facility, source_rpm, package_name, str(rel_file)))

    with args.hits.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("facility", "source_rpm", "package_name", "matching_file"))
        writer.writerows(sorted(hit_rows))

    union = set().union(*matched.values())
    package_union = set().union(*matched_packages.values())
    all_package_names = {package_name for package_name, _ in roots.values()}
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("facility", "source_rpm_count", "package_name_count", "source_rpm_denominator", "package_name_denominator", "interpretation"))
        for facility in FACILITIES:
            writer.writerow((facility, len(matched[facility]), len(matched_packages[facility]), len(roots), len(all_package_names), "lexical source mention; approximate"))
        writer.writerow(("ANY_AFFECTED_FACILITY", len(union), len(package_union), len(roots), len(all_package_names), "deduplicated lexical source mention; approximate"))
        writer.writerow(("SCAN_FAILURES", len({name for name, _ in failures}), "NA", len(roots), len(all_package_names), ";".join(f"{name}:{reason}" for name, reason in failures) or "none"))

    print(f"scan_roots={len(roots)}")
    for facility in FACILITIES:
        print(f"{facility}={len(matched[facility])}")
    print(f"ANY_AFFECTED_FACILITY={len(union)}")
    print(f"ANY_AFFECTED_PACKAGE_NAME={len(package_union)}")
    print(f"SCAN_FAILURES={len(failures)}")
    return 0 if not failures else 2


if __name__ == "__main__":
    raise SystemExit(main())
