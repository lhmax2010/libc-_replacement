#!/usr/bin/python3
"""Fail-closed promotion-ledger gate with an authenticated census boundary."""

from __future__ import annotations

import argparse
import csv
import hashlib
import sys
from collections import Counter, defaultdict
from pathlib import Path
from typing import TypeAlias


VERSION = "2.0.0"
TRUSTED_CENSUS_MANIFEST_SHA256 = (
    "1ff1387e4094704c65dba29d9de40339d50ee747ae23788bef2a0a7357b87192"
)
MANIFEST_FIELDS = {
    "batch_id",
    "target_arch",
    "rpm_arch",
    "source_name",
    "source_nevra",
    "package",
    "nevra",
    "rpm_sha256",
    "tier1_component_id",
    "exact_lock_group",
}
LEDGER_FIELDS = {
    "record_type",
    "batch_id",
    "target_arch",
    "rpm_arch",
    "package",
    "nevra",
    "candidate_sha256",
    "disposition",
    "tier1_component_id",
    "tier1_closure_status",
    "exact_lock_status",
    "legacy_authority_nevra",
    "legacy_authority_sha256",
    "image_selected_sha256",
    "neutral_non_elf_proof",
    "neutral_no_cpp_surface_proof",
    "neutral_dual_source_sha_proof",
}
AUTHORITY_FIELDS = {
    "batch_id",
    "target_arch",
    "rpm_arch",
    "package",
    "nevra",
    "authority_nevra",
    "authority_sha256",
    "image_selected_sha256",
}
CENSUS_MANIFEST_FIELDS = {
    "census_id",
    "membership_sha256",
    "logical_path",
    "scope",
}
CENSUS_FIELDS = {
    "census_id",
    "target_arch",
    "package",
    "component_id",
    "component_pkg_count",
}
APPROVED_DISPOSITIONS = {
    "ADMIT",
    "ADMIT_STDLIB_NEUTRAL",
    "HOLD_SIBLING",
}
NEUTRAL_PROOF_FIELDS = (
    "neutral_non_elf_proof",
    "neutral_no_cpp_surface_proof",
    "neutral_dual_source_sha_proof",
)

IdentityKey: TypeAlias = tuple[str, str, str, str, str]
PackageKey: TypeAlias = tuple[str, str, str]


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


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


def identity_key(row: dict[str, str]) -> IdentityKey:
    return (
        row["batch_id"],
        row["target_arch"],
        row["package"],
        row["rpm_arch"],
        row["nevra"],
    )


def package_key(row: dict[str, str]) -> PackageKey:
    return row["batch_id"], row["target_arch"], row["package"]


def append_finding(
    findings: list[list[str]],
    code: str,
    row_key: IdentityKey,
    detail: str,
) -> None:
    findings.append(
        [
            code,
            row_key[0],
            row_key[1],
            row_key[2],
            row_key[3],
            row_key[4],
            detail,
        ]
    )


def census_finding_key(census_id: str) -> IdentityKey:
    return ("CENSUS", "ALL", census_id, "N/A", "N/A")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--ledger", type=Path, required=True)
    parser.add_argument("--authority", type=Path, required=True)
    parser.add_argument("--census-membership", type=Path, required=True)
    parser.add_argument("--census-manifest", type=Path, required=True)
    parser.add_argument("--census-id", required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    try:
        manifest = read_tsv(args.manifest, MANIFEST_FIELDS)
        ledger_all = read_tsv(args.ledger, LEDGER_FIELDS)
        authority = read_tsv(args.authority, AUTHORITY_FIELDS)
        census_manifest = read_tsv(
            args.census_manifest, CENSUS_MANIFEST_FIELDS
        )
        census = read_tsv(args.census_membership, CENSUS_FIELDS)
    except (OSError, ValueError) as error:
        print(f"INPUT_ERROR {error}", file=sys.stderr)
        return 3

    ledger = [row for row in ledger_all if row["record_type"] == "ENTRY"]
    if not ledger:
        print("INPUT_ERROR ledger has no ENTRY rows", file=sys.stderr)
        return 3

    args.output.mkdir(parents=True, exist_ok=True)
    findings: list[list[str]] = []

    census_verified = True
    actual_census_manifest_sha = sha256(args.census_manifest)
    if actual_census_manifest_sha != TRUSTED_CENSUS_MANIFEST_SHA256:
        append_finding(
            findings,
            "CENSUS_INPUT_UNVERIFIED",
            census_finding_key(args.census_id),
            (
                "census manifest digest is not the frozen gate authority;"
                f"expected={TRUSTED_CENSUS_MANIFEST_SHA256};"
                f"actual={actual_census_manifest_sha}"
            ),
        )
        census_verified = False

    census_authority_rows = [
        row for row in census_manifest if row["census_id"] == args.census_id
    ]
    if len(census_authority_rows) != 1:
        append_finding(
            findings,
            "CENSUS_INPUT_UNVERIFIED",
            census_finding_key(args.census_id),
            f"authority_rows={len(census_authority_rows)};expected=1",
        )
        census_verified = False
    else:
        expected_census_sha = census_authority_rows[0]["membership_sha256"]
        actual_census_sha = sha256(args.census_membership)
        if actual_census_sha != expected_census_sha:
            append_finding(
                findings,
                "CENSUS_INPUT_UNVERIFIED",
                census_finding_key(args.census_id),
                (
                    "membership digest differs from frozen census manifest;"
                    f"expected={expected_census_sha};actual={actual_census_sha}"
                ),
            )
            census_verified = False

    if any(row["census_id"] != args.census_id for row in census):
        append_finding(
            findings,
            "CENSUS_INPUT_UNVERIFIED",
            census_finding_key(args.census_id),
            "membership rows contain an unexpected census_id",
        )
        census_verified = False

    manifest_counts = Counter(identity_key(row) for row in manifest)
    ledger_counts = Counter(identity_key(row) for row in ledger)
    manifest_by_key: dict[IdentityKey, list[dict[str, str]]] = defaultdict(list)
    ledger_by_key: dict[IdentityKey, list[dict[str, str]]] = defaultdict(list)
    for row in manifest:
        manifest_by_key[identity_key(row)].append(row)
    for row in ledger:
        ledger_by_key[identity_key(row)].append(row)

    for item in sorted(manifest_counts):
        if manifest_counts[item] > 1:
            append_finding(
                findings,
                "MANIFEST_DUPLICATE_OUTPUT",
                item,
                f"manifest_rows={manifest_counts[item]};expected=1",
            )
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

    for item in sorted(ledger_counts):
        if manifest_counts[item] == 0:
            append_finding(
                findings,
                "LEDGER_ORPHAN_ROW",
                item,
                "promotion-ledger row has no candidate-manifest output",
            )
        if ledger_counts[item] > 1 and manifest_counts[item] == 0:
            append_finding(
                findings,
                "LEDGER_DUPLICATE_ROW",
                item,
                f"ledger_rows={ledger_counts[item]};expected=1",
            )

    unique_manifest = {
        item: rows[0]
        for item, rows in manifest_by_key.items()
        if len(rows) == 1
    }
    unique_ledger = {
        item: rows[0] for item, rows in ledger_by_key.items() if len(rows) == 1
    }

    for item, ledger_row in sorted(unique_ledger.items()):
        manifest_row = unique_manifest.get(item)
        if manifest_row is None:
            continue
        disposition = ledger_row["disposition"]
        if disposition not in APPROVED_DISPOSITIONS:
            append_finding(
                findings,
                "DISPOSITION_INVALID",
                item,
                f"disposition={disposition!r}",
            )
            continue
        if manifest_row["rpm_sha256"] != ledger_row["candidate_sha256"]:
            append_finding(
                findings,
                "PAYLOAD_IDENTITY_MISMATCH",
                item,
                (
                    f"manifest={manifest_row['rpm_sha256']};"
                    f"ledger={ledger_row['candidate_sha256']}"
                ),
            )
        if disposition in {"ADMIT", "ADMIT_STDLIB_NEUTRAL"}:
            if (
                ledger_row["image_selected_sha256"]
                != ledger_row["candidate_sha256"]
            ):
                append_finding(
                    findings,
                    "ADMIT_IMAGE_DRIFT",
                    item,
                    (
                        f"candidate={ledger_row['candidate_sha256']};"
                        f"image={ledger_row['image_selected_sha256']}"
                    ),
                )
        if disposition == "ADMIT_STDLIB_NEUTRAL":
            failed_proofs = [
                field
                for field in NEUTRAL_PROOF_FIELDS
                if ledger_row[field] != "PASS"
            ]
            if failed_proofs:
                append_finding(
                    findings,
                    "STDLIB_NEUTRAL_EVIDENCE_INCOMPLETE",
                    item,
                    "failed_proofs=" + ",".join(failed_proofs),
                )

    authority_by_key: dict[
        IdentityKey, list[dict[str, str]]
    ] = defaultdict(list)
    for row in authority:
        authority_by_key[identity_key(row)].append(row)
    for item, ledger_row in sorted(unique_ledger.items()):
        if ledger_row["disposition"] != "HOLD_SIBLING":
            continue
        matches = authority_by_key.get(item, [])
        drift = len(matches) != 1
        detail = f"authority_rows={len(matches)}"
        if len(matches) == 1:
            authority_row = matches[0]
            expected = authority_row["authority_sha256"]
            values = {
                "ledger": ledger_row["legacy_authority_sha256"],
                "ledger_image": ledger_row["image_selected_sha256"],
                "authority_image": authority_row["image_selected_sha256"],
            }
            drift = any(value != expected for value in values.values())
            detail = (
                f"authority={expected};ledger={values['ledger']};"
                f"ledger_image={values['ledger_image']};"
                f"authority_image={values['authority_image']}"
            )
        if drift:
            append_finding(
                findings,
                "HOLD_AUTHORITY_DRIFT",
                item,
                detail,
            )

    if census_verified:
        census_by_arch_package: dict[
            tuple[str, str], dict[str, str]
        ] = {}
        census_components: dict[
            tuple[str, str], list[str]
        ] = defaultdict(list)
        duplicate_census_keys = set()
        for row in census:
            census_key = (row["target_arch"], row["package"])
            if census_key in census_by_arch_package:
                duplicate_census_keys.add(census_key)
            census_by_arch_package[census_key] = row
            census_components[
                (row["target_arch"], row["component_id"])
            ].append(row["package"])
        if duplicate_census_keys:
            append_finding(
                findings,
                "CENSUS_INPUT_UNVERIFIED",
                census_finding_key(args.census_id),
                f"duplicate_package_rows={len(duplicate_census_keys)}",
            )
            census_verified = False

    if census_verified:
        ledger_by_package: dict[
            PackageKey, list[dict[str, str]]
        ] = defaultdict(list)
        for row in ledger:
            ledger_by_package[package_key(row)].append(row)
        checked_components: set[tuple[str, str, str]] = set()
        for item, ledger_row in sorted(unique_ledger.items()):
            if ledger_row["disposition"] != "ADMIT":
                continue
            census_row = census_by_arch_package.get((item[1], item[2]))
            if census_row is None:
                append_finding(
                    findings,
                    "ADMIT_CLOSURE_BROKEN",
                    item,
                    "ADMIT package absent from authenticated census membership",
                )
                continue
            census_component = census_row["component_id"]
            manifest_row = unique_manifest.get(item)
            reported_components = {
                ledger_row["tier1_component_id"],
                manifest_row["tier1_component_id"] if manifest_row else "",
            }
            if reported_components != {census_component}:
                append_finding(
                    findings,
                    "COMPONENT_ID_CROSSCHECK_MISMATCH",
                    item,
                    (
                        f"census={census_component};"
                        f"ledger={ledger_row['tier1_component_id']};"
                        "manifest="
                        f"{manifest_row['tier1_component_id'] if manifest_row else 'MISSING'}"
                    ),
                )
            component_key = (item[0], item[1], census_component)
            if component_key in checked_components:
                continue
            checked_components.add(component_key)
            missing_or_hold = []
            for member in sorted(
                census_components[(item[1], census_component)]
            ):
                member_rows = ledger_by_package.get(
                    (item[0], item[1], member), []
                )
                if len(member_rows) != 1:
                    missing_or_hold.append(
                        f"{member}:rows={len(member_rows)}"
                    )
                elif member_rows[0]["disposition"] != "ADMIT":
                    missing_or_hold.append(
                        f"{member}:{member_rows[0]['disposition']}"
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
                        f"component={census_component};"
                        f"missing_or_hold={','.join(missing_or_hold) or 'NONE'};"
                        f"ledger_status={ledger_row['tier1_closure_status']}"
                    ),
                )

    lock_groups: dict[
        tuple[str, str, str], list[tuple[str, str]]
    ] = defaultdict(list)
    for manifest_row in manifest:
        group = manifest_row["exact_lock_group"]
        if not group or group == "NONE":
            continue
        item = identity_key(manifest_row)
        ledger_row = unique_ledger.get(item)
        disposition = ledger_row["disposition"] if ledger_row else "MISSING"
        promotion_state = (
            "ADMIT"
            if disposition in {"ADMIT", "ADMIT_STDLIB_NEUTRAL"}
            else disposition
        )
        lock_groups[(item[0], item[1], group)].append(
            (item[2], promotion_state)
        )
    for (batch, target_arch, group), members in sorted(lock_groups.items()):
        dispositions = {disposition for _, disposition in members}
        if len(dispositions) > 1 or "MISSING" in dispositions:
            package = ",".join(name for name, _ in members)
            append_finding(
                findings,
                "EXACT_LOCK_SPLIT",
                (batch, target_arch, package, "MULTI", group),
                (
                    f"exact_lock_group={group};members="
                    + ",".join(f"{name}:{disp}" for name, disp in members)
                ),
            )

    findings_path = args.output / "findings.tsv"
    with findings_path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(
            [
                "code",
                "batch_id",
                "target_arch",
                "package",
                "rpm_arch",
                "nevra",
                "detail",
            ]
        )
        writer.writerows(findings)

    result = "RED" if findings else "GREEN"
    (args.output / "gate_result.txt").write_text(
        f"{result}\n", encoding="utf-8"
    )
    print(f"TOOL=promotion_ledger_check VERSION={VERSION}")
    print(
        f"CENSUS_ID={args.census_id} "
        f"CENSUS_VERIFIED={'YES' if census_verified else 'NO'}"
    )
    for finding in findings:
        print(
            f"RED code={finding[0]} batch={finding[1]} "
            f"target_arch={finding[2]} package={finding[3]} "
            f"rpm_arch={finding[4]} nevra={finding[5]} "
            f"detail={finding[6]}"
        )
    print(f"GATE={result} blocking={len(findings)}")
    return 2 if findings else 0


if __name__ == "__main__":
    raise SystemExit(main())
