#!/usr/bin/env python3
import csv
import re
from collections import Counter, defaultdict
from pathlib import Path

WORKSPACE = Path("/home/toolchain/development/libc++_replacement")
PROGRESS = WORKSPACE / "progress/R14"
MATRIX = PROGRESS / "tables/matrix_execution.tsv"
TABLES = PROGRESS / "tables"
TARGETS = (
    "_Unwind_RaiseException",
    "_Unwind_Resume",
    "_Unwind_DeleteException",
    "_Unwind_ForcedUnwind",
)
AUDIT_RE = re.compile(
    r"^AUDIT_(BIND|CALL)\treferrer=(.*?)\tprovider=(.*?)\tsymbol=(\S+)$"
)
LD_RE = re.compile(
    r"binding file (.*?) \[.*?\] to (.*?) \[.*?\]: normal symbol `([^']+)'(?: \[([^]]+)\])?"
)


def write_table(name, fields, rows):
    path = TABLES / name
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(
            stream, fieldnames=fields, delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(rows)
    return path


def main():
    rows = list(csv.DictReader(MATRIX.open(), delimiter="\t"))
    audit_events = []
    ld_bindings = []
    event_rows = []
    calls_by_cell_symbol = defaultdict(list)
    binds_by_cell_symbol = defaultdict(list)
    for row in rows:
        case_id = row["case_id"]
        raw = WORKSPACE / row["raw_directory"]
        stdout = (raw / "program.stdout").read_text(errors="replace")
        stderr = (raw / "program.stderr").read_text(errors="replace")
        event_sequence = 0
        for line in stdout.splitlines():
            if line.startswith("EVENT\t"):
                event_sequence += 1
                pieces = line.split("\t", 2)
                event_rows.append(
                    {
                        "case_id": case_id,
                        "sequence": event_sequence,
                        "event": pieces[2] if len(pieces) == 3 else line,
                    }
                )
        audit_sequence = 0
        for line in stderr.splitlines():
            match = AUDIT_RE.match(line)
            if not match:
                continue
            kind, referrer, provider, symbol = match.groups()
            if symbol not in TARGETS:
                continue
            audit_sequence += 1
            record = {
                "case_id": case_id,
                "sequence": audit_sequence,
                "kind": kind,
                "symbol": symbol,
                "referrer": referrer,
                "provider": provider,
            }
            audit_events.append(record)
            if kind == "CALL":
                calls_by_cell_symbol[(case_id, symbol)].append(record)
        debug_files = sorted(raw.glob("ld_debug.*"))
        for debug_file in debug_files:
            binding_sequence = 0
            for line in debug_file.read_text(errors="replace").splitlines():
                match = LD_RE.search(line)
                if not match:
                    continue
                referrer, provider, symbol, version = match.groups()
                base_symbol = symbol.split("@", 1)[0]
                if base_symbol not in TARGETS:
                    continue
                binding_sequence += 1
                record = {
                    "case_id": case_id,
                    "sequence": binding_sequence,
                    "symbol": base_symbol,
                    "symbol_text": symbol,
                    "version": version or "UNVERSIONED",
                    "referrer": referrer,
                    "provider": provider,
                    "source_file": str(debug_file.relative_to(WORKSPACE)),
                }
                ld_bindings.append(record)
                binds_by_cell_symbol[(case_id, base_symbol)].append(record)

    symbol_rows = []
    for row in rows:
        case_id = row["case_id"]
        for symbol in TARGETS:
            calls = calls_by_cell_symbol[(case_id, symbol)]
            bindings = binds_by_cell_symbol[(case_id, symbol)]
            call_providers = sorted({item["provider"] for item in calls})
            bind_providers = sorted({item["provider"] for item in bindings})
            if calls:
                call_status = "OBSERVED"
            else:
                call_status = "NOT_OBSERVED"
            if len(call_providers) > 1:
                provider_shape = "MULTIPLE_CALL_PROVIDERS_OBSERVED"
            elif len(call_providers) == 1:
                provider_shape = "ONE_CALL_PROVIDER_OBSERVED"
            else:
                provider_shape = "NOT_OBSERVED"
            symbol_rows.append(
                {
                    "case_id": case_id,
                    "scheme": row["scheme"],
                    "mode": row["mode"],
                    "order": row["order"],
                    "scope": row["scope"],
                    "binding": row["binding"],
                    "direction": row["direction"],
                    "symbol": symbol,
                    "actual_call_status": call_status,
                    "actual_call_count": len(calls),
                    "actual_call_referrers": ";".join(
                        sorted({item["referrer"] for item in calls})
                    )
                    or "NOT_OBSERVED",
                    "actual_call_providers": ";".join(call_providers)
                    or "NOT_OBSERVED",
                    "provider_shape": provider_shape,
                    "ld_debug_binding_count": len(bindings),
                    "ld_debug_versions": ";".join(
                        sorted({item["version"] for item in bindings})
                    )
                    or "NOT_OBSERVED",
                    "ld_debug_providers": ";".join(bind_providers)
                    or "NOT_OBSERVED",
                    "per_exception_determination": "NOT_OBSERVED_INVALID_SAME_SIDE_CONTROL",
                }
            )

    summary_rows = []
    dimensions = (
        ("ALL", lambda row: "ALL"),
        ("SCHEME", lambda row: row["scheme"]),
        ("SCHEME_MODE", lambda row: f"{row['scheme']}:{row['mode']}"),
        ("SCHEME_DIRECTION", lambda row: f"{row['scheme']}:{row['direction']}"),
    )
    for dimension, key_function in dimensions:
        grouped = defaultdict(list)
        for row in rows:
            grouped[key_function(row)].append(row)
        for key, group in sorted(grouped.items()):
            outcomes = Counter(item["outcome"] for item in group)
            summary_rows.append(
                {
                    "dimension": dimension,
                    "key": key,
                    "cells": len(group),
                    "normal_exit_0": outcomes["NORMAL_EXIT_0"],
                    "sigabrt": outcomes["SIGNAL_SIGABRT"],
                    "other": len(group)
                    - outcomes["NORMAL_EXIT_0"]
                    - outcomes["SIGNAL_SIGABRT"],
                    "valid_catch_cleanup_order": sum(
                        item["type_check"] == "PASS"
                        and item["cleanup_count_check"] == "PASS"
                        and item["order_check"] == "PASS"
                        for item in group
                    ),
                }
            )

    controls = [
        row for row in rows if row["direction"] in ("new_new", "legacy_legacy")
    ]
    coverage_rows = [
        {
            "metric": "FULL_CARTESIAN",
            "numerator": 96,
            "denominator": 128,
            "ratio_percent": "75.000000",
            "scope": "2 schemes x 2 modes x 2 orders x 2 scopes x 2 bindings x 4 directions",
        },
        {
            "metric": "MEANINGFUL_MATRIX",
            "numerator": 96,
            "denominator": 96,
            "ratio_percent": "100.000000",
            "scope": "DT_NEEDED scope collapsed to N/A; dlopen retains GLOBAL and LOCAL",
        },
        {
            "metric": "SAME_SIDE_VALID_CONTROLS",
            "numerator": 0,
            "denominator": len(controls),
            "ratio_percent": "0.000000",
            "scope": "new->new and legacy->legacy cells with catch, two cleanups, rethrow order and exit 0",
        },
    ]
    determinations = []
    for row in rows:
        determinations.append(
            {
                "case_id": row["case_id"],
                "scheme": row["scheme"],
                "mode": row["mode"],
                "order": row["order"],
                "scope": row["scope"],
                "binding": row["binding"],
                "direction": row["direction"],
                "outcome": row["outcome"],
                "catch_type": row["type_check"],
                "cleanup_count": row["cleanup_count_check"],
                "cleanup_order": row["order_check"],
                "same_exception_unwinder": "NOT_OBSERVED_INVALID_SAME_SIDE_CONTROL",
                "legacy_versioned_binding": "NOT_ADJUDICATED_INVALID_SAME_SIDE_CONTROL",
                "new_binding": "NOT_ADJUDICATED_INVALID_SAME_SIDE_CONTROL",
                "memory_safety": "SIGABRT_OBSERVED;SEGFAULT_NOT_OBSERVED;DOUBLE_FREE_NOT_OBSERVED",
                "r14_result": "RED_STOP_INVALID_SAME_SIDE_CONTROL",
            }
        )

    provider_summary_rows = []
    provider_counter = Counter(
        (
            row["scheme"],
            row["symbol"],
            row["actual_call_status"],
            row["actual_call_providers"],
        )
        for row in symbol_rows
    )
    for (scheme, symbol, status, providers), count in sorted(
        provider_counter.items()
    ):
        provider_summary_rows.append(
            {
                "scheme": scheme,
                "symbol": symbol,
                "actual_call_status": status,
                "actual_call_providers": providers,
                "cell_count": count,
                "cell_denominator_per_scheme": 48,
                "adjudication": "NOT_ADJUDICATED_INVALID_SAME_SIDE_CONTROL",
            }
        )

    outputs = []
    outputs.append(
        write_table(
            "audit_unwind_events.tsv",
            ["case_id", "sequence", "kind", "symbol", "referrer", "provider"],
            audit_events,
        )
    )
    outputs.append(
        write_table(
            "ld_debug_unwind_bindings.tsv",
            [
                "case_id",
                "sequence",
                "symbol",
                "symbol_text",
                "version",
                "referrer",
                "provider",
                "source_file",
            ],
            ld_bindings,
        )
    )
    outputs.append(
        write_table(
            "program_events.tsv", ["case_id", "sequence", "event"], event_rows
        )
    )
    outputs.append(
        write_table("cell_symbol_observations.tsv", list(symbol_rows[0]), symbol_rows)
    )
    outputs.append(
        write_table("matrix_summary.tsv", list(summary_rows[0]), summary_rows)
    )
    outputs.append(write_table("coverage.tsv", list(coverage_rows[0]), coverage_rows))
    outputs.append(
        write_table("cell_determinations.tsv", list(determinations[0]), determinations)
    )
    outputs.append(
        write_table(
            "actual_call_provider_summary.tsv",
            list(provider_summary_rows[0]),
            provider_summary_rows,
        )
    )
    print(f"MATRIX_ROWS={len(rows)}")
    print(f"SAME_SIDE_CONTROLS={len(controls)}")
    print(f"AUDIT_EVENTS={len(audit_events)}")
    print(f"AUDIT_CALL_EVENTS={sum(item['kind'] == 'CALL' for item in audit_events)}")
    print(f"LD_DEBUG_BINDINGS={len(ld_bindings)}")
    print(f"PROGRAM_EVENTS={len(event_rows)}")
    for output in outputs:
        print(f"OUTPUT={output}")
    assert len(rows) == 96
    assert len(controls) == 48
    assert all(row["outcome"] == "SIGNAL_SIGABRT" for row in rows)
    assert len(event_rows) == 96
    assert all(event["event"].startswith("THROW:") for event in event_rows)


if __name__ == "__main__":
    main()
