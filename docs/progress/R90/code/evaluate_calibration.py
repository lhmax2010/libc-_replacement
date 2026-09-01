#!/usr/bin/env python3
"""Evaluate R90 positives without changing any R87 scanner."""

from __future__ import annotations

import csv
import sys
from pathlib import Path


if len(sys.argv) != 5:
    raise SystemExit(
        "usage: evaluate_calibration.py ELF_TSV STATIC_TSV SOURCE_TSV OUTPUT_TSV"
    )

elf_path, static_path, source_path, output_path = map(Path, sys.argv[1:])


def rows(path: Path) -> list[dict[str, str]]:
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


elf_rows = rows(elf_path)
static_rows = rows(static_path)
source_rows = rows(source_path)
results: list[tuple[str, str, str, str, int, str]] = []

binary_expectations = (
    (
        "B01_DEEP_WAIT_ADDRESS_AND_TYPE",
        "condition_variable4waitERiEEMS0_DoF",
        (
            "SITE_WITH_NOEXCEPT_TYPE_ENCODING",
            "SITE_WITH_POINTER_NOEXCEPT_ENCODING",
        ),
    ),
    (
        "B02_DEEP_TIMED_ADDRESS_AND_TYPE",
        "condition_variable15__do_timed_waitERlEEMS0_DoF",
        (
            "SITE_WITH_NOEXCEPT_TYPE_ENCODING",
            "SITE_WITH_POINTER_NOEXCEPT_ENCODING",
        ),
    ),
    (
        "B03_CONDITION_TYPE_ONLY_TEMPLATE",
        "_Z19condition_type_onlyIMSt18condition_variableDoF",
        ("CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE",),
    ),
    (
        "B04_CONDITION_POINTER_PARAMETER",
        "_Z27condition_pointer_parameterMSt18condition_variableDoF",
        ("CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE",),
    ),
    (
        "B05_WBUFFER_TYPE_ONLY_DEEP",
        "_Z17wbuffer_type_onlyISt15wbuffer_convertIcwEPDoF",
        ("WBUFFER_CONVERT_NOEXCEPT_FUNCTION_TYPE",),
    ),
    (
        "B06_WBUFFER_DESTRUCTOR_BOOL",
        "_Z23wbuffer_destructor_boolISt15wbuffer_convertIcwELb1E",
        ("WBUFFER_CONVERT_BOOL_TEMPLATE_CANDIDATE",),
    ),
)

elf_carriers = (
    "/usr/lib/debug/positive_symbols.o",
    "/usr/lib64/libpositive_symbols.so",
    "/usr/bin/positive_symbols.elf",
)

for carrier in elf_carriers:
    for sample, token, classifications in binary_expectations:
        for classification in classifications:
            count = sum(
                row["elf_path"] == carrier
                and token in row["raw_symbol"]
                and row["classification"] == classification
                for row in elf_rows
            )
            results.append(
                ("scan_elf_symbols.py", carrier, sample, classification, count,
                 "PASS" if count else "MISS")
            )

for sample, token, classifications in binary_expectations:
    for classification in classifications:
        count = sum(
            token in row["raw_symbol"] and row["classification"] == classification
            for row in static_rows
        )
        results.append(
            ("scan_static_archives.py", "libpositive_symbols.a", sample,
             classification, count, "PASS" if count else "MISS")
        )

source_expectations = (
    ("S01_EXPLICIT_ADDRESS", "D01", "EXPLICIT_ADDRESS"),
    ("S02_MEMBER_POINTER_TYPE", "D01", "MEMBER_POINTER_TYPE"),
    ("S03_DECLTYPE_OR_TRAIT", "D01", "DECLTYPE_OR_TRAIT"),
    ("S04_ALIAS_ADDRESS", "D01", "ALIAS_ADDRESS:cv_alias"),
    ("S05_PRIVATE_TIMED_HELPER", "D02-D04", "PRIVATE_HELPER_REFERENCE"),
    ("S06_DESTRUCTOR_SPELLING", "D05", "DESTRUCTOR_SPELLING"),
    ("S07_DESTRUCTOR_QUERY", "D05", "DESTRUCTOR_TYPE_QUERY"),
    ("S08_BROAD_WAIT_ADDRESS", "GENERAL", "BROAD_WAIT_ADDRESS"),
    ("S09_BROAD_WAIT_TYPE_QUERY", "GENERAL", "BROAD_WAIT_TYPE_QUERY"),
    ("S10_NOEXCEPT_WAIT_EXPRESSION", "GENERAL", "NOEXCEPT_WAIT_EXPRESSION"),
)

for sample, site, kind in source_expectations:
    count = sum(
        row["site"] == site and row["candidate_kind"] == kind
        for row in source_rows
    )
    results.append(
        ("scan_noexcept_type_uses.py", "source calibration root", sample,
         f"{site}:{kind}", count, "PASS" if count else "MISS")
    )

output_path.parent.mkdir(parents=True, exist_ok=True)
with output_path.open("w", newline="") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(
        ("scanner", "carrier", "sample", "expected_classification",
         "matching_rows", "result")
    )
    writer.writerows(results)

misses = [row for row in results if row[-1] != "PASS"]
print(f"calibration_checks={len(results)}")
print(f"calibration_passes={len(results) - len(misses)}")
print(f"calibration_misses={len(misses)}")
for row in results:
    print("\t".join(map(str, row)))
raise SystemExit(1 if misses else 0)
