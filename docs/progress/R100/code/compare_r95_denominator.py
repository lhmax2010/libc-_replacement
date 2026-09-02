#!/usr/bin/env python3
"""Crosswalk the R95 runtime-ELF denominator to R100's Toolchain scope."""

import csv
from pathlib import Path


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


old = {row["source_name"] for row in read("docs/progress/R95/tables/base_cpp_source_records_exact.tsv")}
current = {row["source_package"]: row for row in read("progress/R100/tables/base_cpp_package_decisions.tsv")}
rows = []
for name in sorted(old | set(current)):
    if name in old and name in current:
        relation = "COMMON"
        reason = "R95_RUNTIME_CPP_ELF_AND_R100_TOOLCHAIN_CPP_CANDIDATE"
    elif name in current:
        relation = "R100_ONLY"
        reason = "R100_BROADER_BUILD_STATIC_HEADER_SCOPE_OR_DIFFERENT_TOOLCHAIN_CONTENT"
    else:
        relation = "R95_ONLY"
        reason = "NOT_IN_R100_TOOLCHAIN_CANDIDATE_SET"
    rows.append({
        "source_package": name,
        "relation": relation,
        "r95_runtime_cpp_elf": "YES" if name in old else "NO",
        "r100_denominator_evidence": current.get(name, {}).get("denominator_evidence", "NONE"),
        "reason": reason,
    })
fields = list(rows[0])
out = Path("progress/R100/tables/r95_r100_denominator_crosswalk.tsv")
with out.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
for relation in ("COMMON", "R100_ONLY", "R95_ONLY"):
    names = [row["source_package"] for row in rows if row["relation"] == relation]
    print(f"{relation}={len(names)}")
    print(f"{relation}_NAMES=" + (";".join(names) or "NONE"))
