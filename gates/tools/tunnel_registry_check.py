#!/usr/bin/python3
"""Fail-closed tunnel registry checker (G6 governance gate)."""

from __future__ import annotations

import argparse
import csv
import hashlib
import sys
from datetime import date
from pathlib import Path


VERSION = "1.1.0"
OBSERVED_FIELDS = {
    "edge_id", "consumer_pkg", "provider_pkg", "api_symbol",
    "api_signature", "boundary_kind",
}
REGISTRY_FIELDS = {
    "entry_id", "consumer_pkg", "provider_pkg", "api_symbol",
    "api_signature_sha256", "status", "owner", "expiry",
}
EXPORT_FIELDS = {"provider_pkg", "api_symbol"}


def read_tsv(path: Path, required: set[str]) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        fields = set(reader.fieldnames or [])
        missing = required - fields
        if missing:
            raise ValueError(f"{path}: missing columns {','.join(sorted(missing))}")
        return list(reader)


def signature_sha256(signature: str) -> str:
    return hashlib.sha256(signature.encode("utf-8")).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--observed", type=Path, required=True)
    parser.add_argument("--registry", type=Path, required=True)
    parser.add_argument("--exports", type=Path, required=True)
    parser.add_argument("--as-of", type=date.fromisoformat, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    try:
        observed = read_tsv(args.observed, OBSERVED_FIELDS)
        registry = read_tsv(args.registry, REGISTRY_FIELDS)
        exports = read_tsv(args.exports, EXPORT_FIELDS)
    except (OSError, ValueError) as error:
        print(f"INPUT_ERROR {error}", file=sys.stderr)
        return 3

    args.output.mkdir(parents=True, exist_ok=True)
    findings: list[list[str]] = []

    registry_by_key: dict[tuple[str, str, str], list[dict[str, str]]] = {}
    for row in registry:
        key = (row["consumer_pkg"], row["provider_pkg"], row["api_symbol"])
        registry_by_key.setdefault(key, []).append(row)

    exported = {(row["provider_pkg"], row["api_symbol"]) for row in exports}
    for row in observed:
        if row["boundary_kind"] != "TUNNEL":
            continue
        key = (row["consumer_pkg"], row["provider_pkg"], row["api_symbol"])
        matches = registry_by_key.get(key, [])
        if len(matches) != 1:
            code = (
                "UNREGISTERED_TUNNEL_EDGE" if not matches
                else "AMBIGUOUS_REGISTRY_ENTRY"
            )
            findings.append([
                code, row["edge_id"], row["consumer_pkg"], row["provider_pkg"],
                row["api_symbol"], f"registry_matches={len(matches)}",
            ])
            continue
        entry = matches[0]
        if entry["status"] != "SIGNED":
            findings.append([
                "REGISTRY_NOT_SIGNED", row["edge_id"], row["consumer_pkg"],
                row["provider_pkg"], row["api_symbol"],
                f"status={entry['status']}",
            ])
        try:
            expired = date.fromisoformat(entry["expiry"]) < args.as_of
        except ValueError:
            expired = True
        if expired:
            findings.append([
                "REGISTRY_EXPIRED", row["edge_id"], row["consumer_pkg"],
                row["provider_pkg"], row["api_symbol"],
                f"expiry={entry['expiry']};as_of={args.as_of.isoformat()}",
            ])
        actual_digest = signature_sha256(row["api_signature"])
        if entry["api_signature_sha256"] != actual_digest:
            findings.append([
                "API_SIGNATURE_CHANGED", row["edge_id"], row["consumer_pkg"],
                row["provider_pkg"], row["api_symbol"],
                (
                    f"registry={entry['api_signature_sha256']};"
                    f"observed={actual_digest}"
                ),
            ])

    for entry in registry:
        if entry["status"] not in {"SIGNED", "PENDING"}:
            continue
        if (entry["provider_pkg"], entry["api_symbol"]) not in exported:
            findings.append([
                "REGISTRY_SYMBOL_DISAPPEARED", entry["entry_id"],
                entry["consumer_pkg"], entry["provider_pkg"],
                entry["api_symbol"], "provider export absent",
            ])

    findings_path = args.output / "findings.tsv"
    with findings_path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow([
            "code", "subject_id", "consumer_pkg", "provider_pkg",
            "api_symbol", "detail",
        ])
        writer.writerows(findings)

    result = "RED" if findings else "GREEN"
    (args.output / "gate_result.txt").write_text(
        f"{result}\n", encoding="utf-8"
    )
    print(f"TOOL=tunnel_registry_check VERSION={VERSION}")
    for item in findings:
        print(
            f"RED code={item[0]} subject={item[1]} "
            f"consumer={item[2]} provider={item[3]} symbol={item[4]} "
            f"detail={item[5]}"
        )
    print(f"GATE={result} blocking={len(findings)}")
    return 2 if findings else 0


if __name__ == "__main__":
    raise SystemExit(main())
