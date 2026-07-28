#!/usr/bin/python3
"""Fail-closed source-output promotion-ledger and image-authority gate."""

from __future__ import annotations

import argparse
import csv
import sys
from collections import Counter, defaultdict
from pathlib import Path


VERSION = "1.2.0"
MANIFEST_FIELDS = {
    "batch_id",
    "arch",
    "rpm_arch",
    "source_name",
    "source_nevra",
    "binary_package",
    "binary_nevra",
    "rpm_sha256",
    "tier1_component_id",
    "tier1_component_members",
    "exact_lock_group",
}
LEDGER_FIELDS = {
    "record_type",
    "batch_id",
    "arch",
    "binary_package",
    "candidate_rpm_sha256",
    "disposition",
    "tier1_component_id",
    "tier1_closure_status",
    "exact_lock_status",
    "legacy_authority_nevra",
    "legacy_authority_sha256",
    "image_selected_sha256",
}
AUTHORITY_FIELDS = {
    "batch_id",
    "arch",
    "binary_package",
    "authority_nevra",
    "authority_sha256",
    "image_selected_sha256",
}
APPROVED_DISPOSITIONS = {"ADMIT", "HOLD_SIBLING"}


def read_tsv(path: Path, required: set[str]) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        fields = set(reader.fieldnames or [])
        missing = required - fields
        if missing:
            raise ValueError(
                f"{path}: missing columns {','.join(sorted(missing))}"
            )
        rows = list(reader)
    if not rows:
        raise ValueError(f"{path}: empty input")
    return rows


def key(row: dict[str, str]) -> tuple[str, str, str]:
    return row["batch_id"], row["arch"], row["binary_package"]


def append_finding(
    findings: list[list[str]],
    code: str,
    row_key: tuple[str, str, str],
    detail: str,
) -> None:
    findings.append([code, row_key[0], row_key[1], row_key[2], detail])


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--ledger", type=Path, required=True)
    parser.add_argument("--authority", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    try:
        manifest = read_tsv(args.manifest, MANIFEST_FIELDS)
        ledger_all = read_tsv(args.ledger, LEDGER_FIELDS)
        authority = read_tsv(args.authority, AUTHORITY_FIELDS)
    except (OSError, ValueError) as error:
        print(f"INPUT_ERROR {error}", file=sys.stderr)
        return 3

    ledger = [row for row in ledger_all if row["record_type"] == "ENTRY"]
    if not ledger:
        print("INPUT_ERROR ledger has no ENTRY rows", file=sys.stderr)
        return 3

    args.output.mkdir(parents=True, exist_ok=True)
    findings: list[list[str]] = []
    manifest_counts = Counter(key(row) for row in manifest)
    ledger_counts = Counter(key(row) for row in ledger)
    ledger_by_key: dict[tuple[str, str, str], list[dict[str, str]]] = defaultdict(
        list
    )
    for row in ledger:
        ledger_by_key[key(row)].append(row)

    for item in sorted(manifest_counts):
        if ledger_counts[item] == 0:
            append_finding(
                findings,
                "LEDGER_MISSING_OUTPUT",
                item,
                "candidate manifest output has no promotion-ledger row",
            )
        elif ledger_counts[item] > 1:
            append_finding(
                findings,
                "LEDGER_DUPLICATE_ROW",
                item,
                f"ledger_rows={ledger_counts[item]};expected=1",
            )

    unique_ledger = {
        item: rows[0] for item, rows in ledger_by_key.items() if len(rows) == 1
    }
    manifest_by_key = {key(row): row for row in manifest}

    for item, ledger_row in sorted(unique_ledger.items()):
        if item not in manifest_by_key:
            continue
        if ledger_row["disposition"] not in APPROVED_DISPOSITIONS:
            print(
                f"INPUT_ERROR {item}: invalid disposition "
                f"{ledger_row['disposition']!r}",
                file=sys.stderr,
            )
            return 3

    checked_components: set[tuple[str, str, str]] = set()
    for item, ledger_row in sorted(unique_ledger.items()):
        manifest_row = manifest_by_key.get(item)
        if manifest_row is None or ledger_row["disposition"] != "ADMIT":
            continue
        component_key = (
            item[0],
            item[1],
            manifest_row["tier1_component_id"],
        )
        if component_key in checked_components:
            continue
        checked_components.add(component_key)
        members = [
            member
            for member in manifest_row["tier1_component_members"].split(",")
            if member
        ]
        missing_or_hold = []
        for member in members:
            member_row = unique_ledger.get((item[0], item[1], member))
            if member_row is None or member_row["disposition"] != "ADMIT":
                missing_or_hold.append(
                    f"{member}:"
                    + (
                        "MISSING"
                        if member_row is None
                        else member_row["disposition"]
                    )
                )
        if (
            missing_or_hold
            or ledger_row["tier1_closure_status"] != "CLOSED"
        ):
            append_finding(
                findings,
                "ADMIT_CLOSURE_BROKEN",
                item,
                (
                    f"component={manifest_row['tier1_component_id']};"
                    f"missing_or_hold={','.join(missing_or_hold) or 'NONE'};"
                    f"ledger_status={ledger_row['tier1_closure_status']}"
                ),
            )

    authority_by_key: dict[
        tuple[str, str, str], list[dict[str, str]]
    ] = defaultdict(list)
    for row in authority:
        authority_by_key[key(row)].append(row)
    for item, ledger_row in sorted(unique_ledger.items()):
        if ledger_row["disposition"] != "HOLD_SIBLING":
            continue
        matches = authority_by_key.get(item, [])
        drift = len(matches) != 1
        detail = f"authority_rows={len(matches)}"
        if len(matches) == 1:
            authority_row = matches[0]
            expected = authority_row["authority_sha256"]
            actuals = {
                "ledger": ledger_row["legacy_authority_sha256"],
                "ledger_image": ledger_row["image_selected_sha256"],
                "authority_image": authority_row["image_selected_sha256"],
            }
            drift = any(value != expected for value in actuals.values())
            detail = (
                f"authority={expected};ledger={actuals['ledger']};"
                f"ledger_image={actuals['ledger_image']};"
                f"authority_image={actuals['authority_image']}"
            )
        if drift:
            append_finding(
                findings,
                "HOLD_AUTHORITY_DRIFT",
                item,
                detail,
            )

    lock_groups: dict[tuple[str, str, str], list[tuple[str, str]]] = defaultdict(
        list
    )
    for manifest_row in manifest:
        group = manifest_row["exact_lock_group"]
        if not group or group == "NONE":
            continue
        item = key(manifest_row)
        ledger_row = unique_ledger.get(item)
        disposition = ledger_row["disposition"] if ledger_row else "MISSING"
        lock_groups[(item[0], item[1], group)].append(
            (item[2], disposition)
        )
    for (batch, arch, group), members in sorted(lock_groups.items()):
        dispositions = {disposition for _, disposition in members}
        if len(dispositions) > 1 or "MISSING" in dispositions:
            package = ",".join(name for name, _ in members)
            append_finding(
                findings,
                "EXACT_LOCK_SPLIT",
                (batch, arch, package),
                (
                    f"exact_lock_group={group};"
                    f"members="
                    + ",".join(f"{name}:{disp}" for name, disp in members)
                ),
            )

    findings_path = args.output / "findings.tsv"
    with findings_path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(["code", "batch_id", "arch", "package", "detail"])
        writer.writerows(findings)

    result = "RED" if findings else "GREEN"
    (args.output / "gate_result.txt").write_text(
        f"{result}\n", encoding="utf-8"
    )
    print(f"TOOL=promotion_ledger_check VERSION={VERSION}")
    for finding in findings:
        print(
            f"RED code={finding[0]} batch={finding[1]} "
            f"arch={finding[2]} package={finding[3]} detail={finding[4]}"
        )
    print(f"GATE={result} blocking={len(findings)}")
    return 2 if findings else 0


if __name__ == "__main__":
    raise SystemExit(main())
