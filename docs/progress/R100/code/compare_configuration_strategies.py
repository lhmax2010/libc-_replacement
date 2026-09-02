#!/usr/bin/env python3
"""Produce package-level global-injection effects and strategy counts."""

import csv
from collections import Counter
from pathlib import Path


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


decisions = read("progress/R100/tables/base_cpp_package_decisions.tsv")
effects = []
for row in decisions:
    consumed = row["buildlog_arches"] != "NONE"
    decision = row["decision"]
    if decision == "NEED_LIBCXX":
        effect = "INTENDED_LIBCXX_SELECTION"
    elif decision == "CANNOT_SWITCH_CURRENTLY":
        effect = "GCC_REJECTS_STDLIB_IF_PROPAGATED"
    elif decision == "UNDETERMINED":
        effect = "UNRESOLVED_SEMANTIC_CHANGE"
    elif consumed:
        effect = "UNNEEDED_CLANG_CXX_RUNTIME_SELECTION_OR_C_WARNING"
    else:
        effect = "ENVIRONMENT_SET_BUT_CONSUMPTION_NOT_OBSERVED"
    effects.append({
        "source_package": row["source_package"],
        "package_decision": decision,
        "actual_cpp_build_consumption": "YES" if consumed else "NOT_OBSERVED",
        "global_injection_effect": effect,
        "silent_ignore_result": (
            "NO_DRIVER_SILENT_IGNORE_OBSERVED" if consumed
            else "NOT_OBSERVED_PACKAGE_BUILD_DID_NOT_SHOW_CPP_FLAG_CONSUMPTION"
        ),
    })

fields = list(effects[0])
with Path("progress/R100/tables/global_injection_package_effects.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(effects)

counts = Counter(row["decision"] for row in decisions)
strategy_rows = [
    {
        "strategy": "PER_PACKAGE_SPEC",
        "confirmed_packages_to_change": str(counts["NEED_LIBCXX"]),
        "confirmed_package_names": ";".join(row["source_package"] for row in decisions if row["decision"] == "NEED_LIBCXX"),
        "undetermined_packages": str(counts["UNDETERMINED"]),
        "current_gcc_conflicts_exposed": "0",
        "unneeded_cpp_packages_touched": "0",
        "scope_note": "Does not count any future action for the two UNDETERMINED packages.",
    },
    {
        "strategy": "GLOBAL_PROJECT_CONFIG",
        "confirmed_packages_to_change": str(counts["NEED_LIBCXX"]),
        "confirmed_package_names": ";".join(row["source_package"] for row in decisions if row["decision"] == "NEED_LIBCXX"),
        "undetermined_packages": str(counts["UNDETERMINED"]),
        "current_gcc_conflicts_exposed": str(counts["CANNOT_SWITCH_CURRENTLY"]),
        "unneeded_cpp_packages_touched": str(counts["NO_LIBCXX_NEEDED"]),
        "scope_note": "Exports apply project-wide; 69 canonical sources have actual C++ build-log consumption, while LDFLAGS consumption outside the 73-source denominator was not exhaustively measured.",
    },
]
fields = list(strategy_rows[0])
with Path("progress/R100/tables/configuration_strategy_comparison.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(strategy_rows)

print("effect_counts")
for key, value in sorted(Counter(row["global_injection_effect"] for row in effects).items()):
    print(f"{key}={value}")
print("strategy_rows=2")
