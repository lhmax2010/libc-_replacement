#!/usr/bin/env python3
"""Map the historical 483-package list onto the actual Base and Unified snapshots."""

import argparse
import csv
import sys
from collections import Counter, defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--migration", type=Path, required=True)
    ap.add_argument("--base-source", type=Path, required=True)
    ap.add_argument("--unified-source", type=Path, required=True)
    ap.add_argument("--base-cpp", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()
    migration = read(args.migration)
    base = read(args.base_source)
    unified = read(args.unified_source)
    base_cpp = read(args.base_cpp)

    by_name = {"base": defaultdict(list), "unified": defaultdict(list)}
    for project, records in (("base", base), ("unified", unified)):
        for row in records:
            by_name[project][row["name"]].append(row)
    base_locations = {row["location"] for row in base}
    unified_locations = {row["location"] for row in unified}

    output = []
    counts = Counter()
    for row in migration:
        name = row["package_name"]
        in_base = name in by_name["base"]
        in_unified = name in by_name["unified"]
        if in_base and in_unified:
            membership = "BOTH_PROJECTS_BY_NAME"
        elif in_base:
            membership = "BASE_ONLY_BY_NAME"
        elif in_unified:
            membership = "UNIFIED_ONLY_BY_NAME"
        else:
            membership = "NEITHER_CURRENT_SNAPSHOT_BY_NAME"
        counts[membership] += 1
        output.append({
            "historical_source_rpm": row["source_rpm"],
            "package_name": name,
            "historical_stage": row["stage"],
            "actual_project_membership": membership,
            "exact_base_srpm_filename": "YES" if row["source_rpm"] in base_locations else "NO",
            "exact_unified_srpm_filename": "YES" if row["source_rpm"] in unified_locations else "NO",
            "actual_base_srpms": ";".join(sorted(r["location"] for r in by_name["base"].get(name, []))),
            "actual_unified_srpms": ";".join(sorted(r["location"] for r in by_name["unified"].get(name, []))),
        })
    fields = ["historical_source_rpm", "package_name", "historical_stage", "actual_project_membership",
              "exact_base_srpm_filename", "exact_unified_srpm_filename", "actual_base_srpms", "actual_unified_srpms"]
    write(args.output, fields, output)

    migration_names = {row["package_name"] for row in migration}
    srpm_to_name = {row["location"]: row["name"] for row in base}
    cpp_names = {srpm_to_name[row["sourcerpm"]] for row in base_cpp}
    print(f"historical_rows={len(migration)}")
    print(f"historical_unique_package_names={len(migration_names)}")
    for key in sorted(counts):
        print(f"membership[{key}]={counts[key]}")
    print(f"actual_base_cpp_source_names_from_supplied_evidence={len(cpp_names)}")
    print(f"actual_base_cpp_names_already_in_historical_483={len(cpp_names & migration_names)}")
    print(f"actual_base_cpp_names_not_in_historical_483={len(cpp_names - migration_names)}")
    print("actual_base_cpp_names_not_in_historical_483_list=" + ";".join(sorted(cpp_names - migration_names)))
    print(f"arithmetic_union_483_plus_missing_actual_base_cpp_names={len(migration_names | cpp_names)}")


if __name__ == "__main__":
    main()
