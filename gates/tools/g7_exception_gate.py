#!/usr/bin/python3
"""Fail-closed G7 exception inventory and rider binary gate."""

from __future__ import annotations

import argparse
import csv
import sys
from pathlib import Path


VERSION = "1.1.0"
INVENTORY_FIELDS = {
    "interface_id", "escape_status", "ast_status", "callback_required",
    "disposition",
}
RIDER_FIELDS = {
    "rider_id", "interface_id", "required", "callback_count",
    "terminate_count", "stale_request_count", "closure_leak_count",
    "partial_result_count", "cross_dso_unwind_count", "status",
}
ACCEPTED_ESCAPE = {"CAUGHT_BEFORE_BOUNDARY", "NO_ESCAPE_REVIEWED"}
ACCEPTED_DISPOSITIONS = {"FIX_WITH_SEAL", "ACCEPT_WITH_RELEASE_NOTE"}


def read_tsv(path: Path, required: set[str]) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        fields = set(reader.fieldnames or [])
        missing = required - fields
        if missing:
            raise ValueError(f"{path}: missing columns {','.join(sorted(missing))}")
        return list(reader)


def parse_count(row: dict[str, str], field: str) -> int:
    value = int(row[field])
    if value < 0:
        raise ValueError(f"{row['rider_id']}: negative {field}")
    return value


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--inventory", type=Path, required=True)
    parser.add_argument("--riders", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    try:
        inventory = read_tsv(args.inventory, INVENTORY_FIELDS)
        riders = read_tsv(args.riders, RIDER_FIELDS)
    except (OSError, ValueError) as error:
        print(f"INPUT_ERROR {error}", file=sys.stderr)
        return 3

    args.output.mkdir(parents=True, exist_ok=True)
    findings: list[list[str]] = []
    riders_by_interface: dict[str, list[dict[str, str]]] = {}
    for row in riders:
        riders_by_interface.setdefault(row["interface_id"], []).append(row)

    for interface in inventory:
        interface_id = interface["interface_id"]
        escape = interface["escape_status"]
        if escape not in ACCEPTED_ESCAPE:
            findings.append([
                "ESCAPE_STATUS_BLOCKING", interface_id, "",
                f"escape_status={escape}",
            ])
        if interface["ast_status"] != "COMPLETE":
            findings.append([
                "AST_EVIDENCE_INCOMPLETE", interface_id, "",
                f"ast_status={interface['ast_status']}",
            ])
        if interface["disposition"] not in ACCEPTED_DISPOSITIONS:
            findings.append([
                "DISPOSITION_INVALID", interface_id, "",
                f"disposition={interface['disposition'] or 'EMPTY'}",
            ])
        linked = riders_by_interface.get(interface_id, [])
        if interface["callback_required"] == "YES" and len(linked) != 1:
            findings.append([
                "RIDER_CARDINALITY_INVALID", interface_id, "",
                f"rider_rows={len(linked)};expected=1",
            ])

    for rider in riders:
        rider_id = rider["rider_id"]
        if rider["required"] != "YES":
            findings.append([
                "RIDER_DISABLED", rider["interface_id"], rider_id,
                f"required={rider['required'] or 'EMPTY'};expected=YES",
            ])
        try:
            callback_count = parse_count(rider, "callback_count")
            terminate_count = parse_count(rider, "terminate_count")
            stale_count = parse_count(rider, "stale_request_count")
            closure_leak = parse_count(rider, "closure_leak_count")
            partial_result = parse_count(rider, "partial_result_count")
            cross_dso_unwind = parse_count(
                rider, "cross_dso_unwind_count"
            )
        except ValueError as error:
            print(f"INPUT_ERROR {error}", file=sys.stderr)
            return 3
        metrics = (
            ("CALLBACK_COUNT_NOT_ONE", callback_count, 1),
            ("TERMINATE_OCCURRED", terminate_count, 0),
            ("STALE_REQUEST_REMAINS", stale_count, 0),
            ("CLOSURE_LEAK_REMAINS", closure_leak, 0),
            ("PARTIAL_RESULT_ACCEPTED", partial_result, 0),
            ("CROSS_DSO_UNWIND", cross_dso_unwind, 0),
        )
        for code, actual, expected in metrics:
            if actual != expected:
                findings.append([
                    code, rider["interface_id"], rider_id,
                    f"actual={actual};expected={expected}",
                ])
        if rider["status"] != "EXECUTED":
            findings.append([
                "RIDER_NOT_EXECUTED", rider["interface_id"], rider_id,
                f"status={rider['status']}",
            ])

    findings_path = args.output / "findings.tsv"
    with findings_path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(["code", "interface_id", "rider_id", "detail"])
        writer.writerows(findings)

    result = "RED" if findings else "GREEN"
    (args.output / "gate_result.txt").write_text(
        f"{result}\n", encoding="utf-8"
    )
    print(f"TOOL=g7_exception_gate VERSION={VERSION}")
    for item in findings:
        print(
            f"RED code={item[0]} interface={item[1]} "
            f"rider={item[2] or '-'} detail={item[3]}"
        )
    print(f"GATE={result} blocking={len(findings)}")
    return 2 if findings else 0


if __name__ == "__main__":
    raise SystemExit(main())
