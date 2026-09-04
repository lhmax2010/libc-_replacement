#!/usr/bin/env python3
"""Validate the R113 audit deliverables without changing source materials."""

from __future__ import annotations

import csv
import hashlib
from pathlib import Path


ROOT = Path("/home/toolchain/development/libc++_replacement")
OUT = ROOT / "docs/progress/R113"
DELIVERY = Path("/tmp/r113_hq_20260904")


def rows(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


input_rows = rows(OUT / "INPUT_SHA256.tsv")
assert len(input_rows) == 9
for row in input_rows:
    path = ROOT / row["file"] if row["kind"] == "archive" else DELIVERY / row["file"]
    assert path.is_file(), path
    assert sha(path) == row["sha256"], path

number_rows = rows(OUT / "NUMBER_AUDIT.tsv")
attachment_rows = rows(OUT / "ATTACHMENT_AUDIT.tsv")
assert len(number_rows) >= 40
assert len(attachment_rows) == 4
assert {r["result"] for r in number_rows} >= {"PASS", "PASS_WITH_SCOPE", "FAIL_UNIT"}

report = (OUT / "REPORT.md").read_text(encoding="utf-8")
for text in (
    "需修改后发出",
    "11,321",
    "172 → 104",
    "tensorflow2",
    "39 个 ELF",
    "r32_status",
    "R32_FROZEN_SOURCE_RPM_COMPREHENSIVE_TEXT_RESCAN",
    "随附证据索引",
    "M8.",
    "未修改七份交付原件",
):
    assert text in report, text

for number in range(1, 27):
    matches = list((OUT / "raw").glob(f"{number:03d}_*.exitcode"))
    assert matches, number
    expected = "128" if number == 20 else "0"
    assert all(p.read_text(encoding="utf-8").strip() == expected for p in matches), matches

print("VALIDATION_PASS")
print(f"input_identity_rows={len(input_rows)}")
print(f"number_audit_rows={len(number_rows)}")
print(f"attachment_audit_rows={len(attachment_rows)}")
print("conclusion=需修改后发出")
