#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path

WORKSPACE = Path("/home/toolchain/development/libc++_replacement")
TABLES = WORKSPACE / "progress/R14b/L2/tables"
SYMBOLS = (
    "_Unwind_RaiseException",
    "_Unwind_Resume",
    "_Unwind_DeleteException",
    "_Unwind_ForcedUnwind",
)


def read_table(path: Path, stage: str):
    with path.open(newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    for row in rows:
        row["stage"] = stage
    return rows


def audit_records(text: str):
    if text == "NOT_OBSERVED":
        return []
    records = []
    for entry in text.split(" || "):
        fields = entry.split("\t")
        record = {}
        for field in fields[1:]:
            key, value = field.split("=", 1)
            record[key] = value
        records.append(record)
    return records


def provider_class(path: str):
    if "libgcc_s.so" in path:
        return "LIBGCC_S"
    if "libunwind.so" in path:
        return "LLVM_LIBUNWIND"
    return f"OTHER:{path}"


def joined(values):
    materialized = sorted(set(values))
    return " | ".join(materialized) if materialized else "NOT_OBSERVED"


def main():
    rows = read_table(TABLES / "matrix_same.tsv", "same")
    rows += read_table(TABLES / "matrix_cross.tsv", "cross")
    with (TABLES / "matrix_all.tsv").open("w", newline="") as stream:
        fields = ["stage"] + [field for field in rows[0] if field != "stage"]
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)

    landing_rows = []
    for row in rows:
        records = audit_records(row["audit_calls"])
        by_symbol = {
            symbol: [record for record in records if record["symbol"] == symbol]
            for symbol in SYMBOLS
        }
        observed_classes = {
            provider_class(record["provider"])
            for symbol in SYMBOLS
            for record in by_symbol[symbol]
        }
        required_observed = [
            symbol for symbol in SYMBOLS[:3] if by_symbol[symbol]
        ]
        if len(observed_classes) > 1:
            consistency = "IMPLEMENTATION_SWITCH_OBSERVED"
        elif len(required_observed) == 3 and len(observed_classes) == 1:
            consistency = "COMPLETE_SAME_IMPLEMENTATION"
        elif observed_classes:
            consistency = "PARTIAL_OBSERVATION_SAME_IMPLEMENTATION"
        else:
            consistency = "NOT_OBSERVED"
        legacy_records = [
            record
            for record in records
            if "liblegacy.so" in record["referrer"]
            or "libstdc++.so" in record["referrer"]
        ]
        new_records = [
            record
            for record in records
            if "libnew_" in record["referrer"]
            or "libc++abi.so" in record["referrer"]
        ]
        landing_rows.append(
            {
                "case_id": row["case_id"],
                "stage": row["stage"],
                "scheme": row["scheme"],
                "mode": row["mode"],
                "order": row["order"],
                "scope": row["scope"],
                "binding": row["binding"],
                "direction": row["direction"],
                "outcome": row["outcome"],
                "raise_provider_paths": joined(
                    record["provider"] for record in by_symbol[SYMBOLS[0]]
                ),
                "resume_provider_paths": joined(
                    record["provider"] for record in by_symbol[SYMBOLS[1]]
                ),
                "delete_provider_paths": joined(
                    record["provider"] for record in by_symbol[SYMBOLS[2]]
                ),
                "forced_provider_paths": joined(
                    record["provider"] for record in by_symbol[SYMBOLS[3]]
                ),
                "observed_provider_classes": joined(observed_classes),
                "same_exception_provider_observation": consistency,
                "legacy_ref_provider_classes": joined(
                    provider_class(record["provider"])
                    for record in legacy_records
                ),
                "new_ref_provider_classes": joined(
                    provider_class(record["provider"]) for record in new_records
                ),
            }
        )
    with (TABLES / "symbol_landing.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(
            stream, list(landing_rows[0]), delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(landing_rows)

    comparison = {}
    for row in rows:
        key = tuple(
            row[field]
            for field in ("stage", "mode", "order", "scope", "binding", "direction")
        )
        comparison.setdefault(key, {})[row["scheme"]] = row
    comparison_rows = []
    for key, schemes in sorted(comparison.items()):
        row_a = schemes["A"]
        row_b = schemes["B"]
        comparison_rows.append(
            {
                "stage": key[0],
                "mode": key[1],
                "order": key[2],
                "scope": key[3],
                "binding": key[4],
                "direction": key[5],
                "scheme_A_case": row_a["case_id"],
                "scheme_A_outcome": row_a["outcome"],
                "scheme_A_type": row_a["type_check"],
                "scheme_A_cleanup": row_a["cleanup_count_check"],
                "scheme_A_order_check": row_a["order_check"],
                "scheme_B_case": row_b["case_id"],
                "scheme_B_outcome": row_b["outcome"],
                "scheme_B_type": row_b["type_check"],
                "scheme_B_cleanup": row_b["cleanup_count_check"],
                "scheme_B_order_check": row_b["order_check"],
            }
        )
    with (TABLES / "scheme_A_B_comparison.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(
            stream, list(comparison_rows[0]), delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(comparison_rows)

    summary_rows = []
    for stage in ("same", "cross"):
        for scheme in ("A", "B"):
            selected = [
                row for row in rows if row["stage"] == stage and row["scheme"] == scheme
            ]
            outcomes = Counter(row["outcome"] for row in selected)
            summary_rows.append(
                {
                    "stage": stage,
                    "scheme": scheme,
                    "cells": len(selected),
                    "normal_exit_0": outcomes["NORMAL_EXIT_0"],
                    "normal_exit_nonzero": sum(
                        count
                        for outcome, count in outcomes.items()
                        if outcome.startswith("NORMAL_EXIT_")
                        and outcome != "NORMAL_EXIT_0"
                    ),
                    "signal_sigabrt": outcomes["SIGNAL_SIGABRT"],
                    "signal_sigsegv": outcomes["SIGNAL_SIGSEGV"],
                    "type_pass": sum(row["type_check"] == "PASS" for row in selected),
                    "cleanup_pass": sum(
                        row["cleanup_count_check"] == "PASS" for row in selected
                    ),
                    "order_pass": sum(row["order_check"] == "PASS" for row in selected),
                    "not_observed_checks": sum(
                        row["type_check"] == "NOT_OBSERVED" for row in selected
                    ),
                }
            )
    with (TABLES / "outcome_summary.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(
            stream, list(summary_rows[0]), delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(summary_rows)
    print(f"MATRIX_ROWS={len(rows)}")
    print(f"SYMBOL_LANDING_ROWS={len(landing_rows)}")
    print(f"A_B_COMPARISON_ROWS={len(comparison_rows)}")
    print(f"SUMMARY_ROWS={len(summary_rows)}")


if __name__ == "__main__":
    main()
