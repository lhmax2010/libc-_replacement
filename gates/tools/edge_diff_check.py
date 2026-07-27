#!/usr/bin/python3
"""Fail-closed cross-boundary ABI edge and SONAME transition diff gate."""

from __future__ import annotations

import argparse
import csv
import sys
from datetime import date
from pathlib import Path


VERSION = "1.0.0"
EDGE_FIELDS = {
    "consumer_pkg", "consumer_elf", "provider_pkg", "provider_soname",
    "symbol", "edge_class",
}
REGISTRY_FIELDS = {
    "consumer_pkg", "provider_pkg", "provider_soname", "symbol",
    "edge_class", "status",
}
TRANSITION_FIELDS = {
    "transition_id", "provider_pkg", "before_soname", "after_soname",
    "status", "expiry", "owner",
}


def read_tsv(path: Path, required: set[str]) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        fields = set(reader.fieldnames or [])
        missing = required - fields
        if missing:
            raise ValueError(f"{path}: missing columns {','.join(sorted(missing))}")
        return list(reader)


def edge_key(row: dict[str, str]) -> tuple[str, ...]:
    return (
        row["consumer_pkg"], row["consumer_elf"], row["provider_pkg"],
        row["provider_soname"], row["symbol"], row["edge_class"],
    )


def registry_key(row: dict[str, str]) -> tuple[str, ...]:
    return (
        row["consumer_pkg"], row["provider_pkg"], row["provider_soname"],
        row["symbol"], row["edge_class"],
    )


def logical_key(row: dict[str, str]) -> tuple[str, ...]:
    return (
        row["consumer_pkg"], row["consumer_elf"], row["provider_pkg"],
        row["symbol"], row["edge_class"],
    )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--before", type=Path, required=True)
    parser.add_argument("--after", type=Path, required=True)
    parser.add_argument("--after-registry", type=Path, required=True)
    parser.add_argument("--transitions", type=Path, required=True)
    parser.add_argument("--as-of", type=date.fromisoformat, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    try:
        before = read_tsv(args.before, EDGE_FIELDS)
        after = read_tsv(args.after, EDGE_FIELDS)
        registry = read_tsv(args.after_registry, REGISTRY_FIELDS)
        transitions = read_tsv(args.transitions, TRANSITION_FIELDS)
    except (OSError, ValueError) as error:
        print(f"INPUT_ERROR {error}", file=sys.stderr)
        return 3

    args.output.mkdir(parents=True, exist_ok=True)
    findings: list[list[str]] = []
    before_keys = {edge_key(row): row for row in before}
    after_keys = {edge_key(row): row for row in after}
    before_logical = {logical_key(row): row for row in before}
    after_logical = {logical_key(row): row for row in after}
    registry_status = {registry_key(row): row["status"] for row in registry}

    for key, row in sorted(after_keys.items()):
        if (
            key not in before_keys
            and logical_key(row) not in before_logical
            and row["edge_class"] == "CPP_ABI"
        ):
            findings.append([
                "NEW_CPP_ABI_EDGE", row["consumer_pkg"], row["provider_pkg"],
                row["provider_soname"], row["symbol"],
                f"consumer_elf={row['consumer_elf']}",
            ])

    for key, row in sorted(before_keys.items()):
        if key in after_keys or logical_key(row) in after_logical:
            continue
        reg_key = (
            row["consumer_pkg"], row["provider_pkg"], row["provider_soname"],
            row["symbol"], row["edge_class"],
        )
        if registry_status.get(reg_key) != "RETIRED":
            findings.append([
                "REMOVED_EDGE_REGISTRY_NOT_RETIRED",
                row["consumer_pkg"], row["provider_pkg"],
                row["provider_soname"], row["symbol"],
                f"registry_status={registry_status.get(reg_key, 'MISSING')}",
            ])

    for key in sorted(before_logical.keys() & after_logical.keys()):
        old = before_logical[key]
        new = after_logical[key]
        if old["provider_soname"] == new["provider_soname"]:
            continue
        matching = []
        for token in transitions:
            if (
                token["provider_pkg"] == old["provider_pkg"]
                and token["before_soname"] == old["provider_soname"]
                and token["after_soname"] == new["provider_soname"]
                and token["status"] == "APPROVED"
            ):
                try:
                    valid = date.fromisoformat(token["expiry"]) >= args.as_of
                except ValueError:
                    valid = False
                if valid:
                    matching.append(token)
        if len(matching) != 1:
            findings.append([
                "SONAME_CHANGE_UNDECLARED",
                old["consumer_pkg"], old["provider_pkg"],
                f"{old['provider_soname']}->{new['provider_soname']}",
                old["symbol"], f"valid_transition_tokens={len(matching)}",
            ])

    findings_path = args.output / "findings.tsv"
    with findings_path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow([
            "code", "consumer_pkg", "provider_pkg", "soname", "symbol",
            "detail",
        ])
        writer.writerows(findings)

    result = "RED" if findings else "GREEN"
    (args.output / "gate_result.txt").write_text(
        f"{result}\n", encoding="utf-8"
    )
    print(f"TOOL=edge_diff_check VERSION={VERSION}")
    for item in findings:
        print(
            f"RED code={item[0]} consumer={item[1]} provider={item[2]} "
            f"soname={item[3]} symbol={item[4]} detail={item[5]}"
        )
    print(f"GATE={result} blocking={len(findings)}")
    return 2 if findings else 0


if __name__ == "__main__":
    raise SystemExit(main())
