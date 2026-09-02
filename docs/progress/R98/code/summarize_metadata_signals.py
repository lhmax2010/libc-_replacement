#!/usr/bin/env python3
"""Summarize compiler-related build requirements and C++ runtime requirements."""

import csv
import re
import sys
from collections import defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
TABLES = Path("progress/R98/tables")


def read(name):
    with (TABLES / name).open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(name, fields, rows):
    with (TABLES / name).open("w", encoding="utf-8", newline="") as stream:
        out = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        out.writeheader()
        out.writerows(rows)


build_rows = []
runtime_rows = []
for project in ("base", "unified"):
    sources = read(f"{project}_source_records.tsv")
    binaries = read(f"{project}_binary_records.tsv")
    source_name = {row["location"]: row["name"] for row in sources}
    for row in sources:
        requirements = set(filter(None, row["requires"].split(";")))
        matched = sorted(x for x in requirements if re.search(
            r"(^|[-_])(gcc|binutils|libstdc\+\+)([-_]|$)|^(gcc|binutils|libstdc\+\+)", x, re.I))
        if matched:
            build_rows.append({"project": project, "source_name": row["name"],
                               "sourcerpm": row["location"], "matched_build_requires": ";".join(matched)})
    groups = defaultdict(list)
    for row in binaries:
        requirements = set(filter(None, row["requires"].split(";")))
        signals = sorted(x for x in requirements if x.startswith(("libstdc++.so", "libc++.so", "libc++abi.so")))
        if signals:
            groups[row["sourcerpm"]].append((row, signals))
    for srpm, values in sorted(groups.items()):
        runtime_rows.append({
            "project": project,
            "source_name": source_name.get(srpm, "NOT_AVAILABLE"),
            "sourcerpm": srpm,
            "binary_names": ";".join(sorted({row["name"] for row, _ in values})),
            "arches": ";".join(sorted({row["arch"] for row, _ in values})),
            "libstdcxx_binary_records": str(sum(any(x.startswith("libstdc++") for x in sig) for _, sig in values)),
            "libcxx_binary_records": str(sum(any(x.startswith(("libc++.so", "libc++abi.so")) for x in sig) for _, sig in values)),
            "runtime_signals": ";".join(sorted({x for _, sig in values for x in sig})),
        })

write("compiler_build_requirement_candidates.tsv",
      ["project", "source_name", "sourcerpm", "matched_build_requires"], build_rows)
write("runtime_signal_source_summary.tsv",
      ["project", "source_name", "sourcerpm", "binary_names", "arches",
       "libstdcxx_binary_records", "libcxx_binary_records", "runtime_signals"], runtime_rows)

for project in ("base", "unified"):
    br = [r for r in build_rows if r["project"] == project]
    rr = [r for r in runtime_rows if r["project"] == project]
    print(f"{project}_build_requirement_records={len(br)}")
    print(f"{project}_build_requirement_names={len({r['source_name'] for r in br})}")
    print(f"{project}_runtime_signal_source_records={len(rr)}")
    print(f"{project}_runtime_signal_source_names={len({r['source_name'] for r in rr})}")
    print(f"{project}_libstdcxx_source_names={len({r['source_name'] for r in rr if int(r['libstdcxx_binary_records'])})}")
    print(f"{project}_libcxx_source_names={len({r['source_name'] for r in rr if int(r['libcxx_binary_records'])})}")
    print(f"{project}_libstdcxx_names=" + ";".join(sorted({r['source_name'] for r in rr if int(r['libstdcxx_binary_records'])})))
    print(f"{project}_libcxx_names=" + ";".join(sorted({r['source_name'] for r in rr if int(r['libcxx_binary_records'])})))
