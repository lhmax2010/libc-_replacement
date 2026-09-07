#!/usr/bin/env python3
from __future__ import annotations

import csv
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def read_rows(name: str) -> list[dict[str, str]]:
    with (ROOT / "tables" / name).open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def main() -> None:
    item1 = read_rows("item1_facility_cancel.tsv")
    assert len(item1) == 30
    for mode in ("timed_mutex", "shared_mutex", "shared_timed_mutex", "future_wait", "cv_any_wait", "cv_any_throw"):
        selected = [row for row in item1 if row["mode"] == mode]
        assert len(selected) == 5
    for row in item1:
        if row["mode"] in ("shared_mutex", "shared_timed_mutex"):
            assert row["returncode"] == "0" and row["cleanup"] == "1" and row["write_entered"] == "1" and row["reusable"] == "0"
        elif row["mode"] == "cv_any_throw":
            assert row["returncode"] == "86" and row["terminate"] == "1"
        else:
            assert row["returncode"] == "0" and row["cleanup"] == "1" and row["reusable"] == "1"

    item3 = read_rows("item3_dual_version.tsv")
    assert len(item3) == 20
    expected = {
        "old_prebound_old": ("LLVM_22", "old", "terminate"),
        "old_relinked_dual": ("LLVM_23", "new", "retained_lock"),
        "new_linked_dual": ("LLVM_23", "new", "clean"),
        "new_rebuilt_before_dual": ("LLVM_22", "old", "terminate"),
    }
    for case, values in expected.items():
        selected = [row for row in item3 if row["case"] == case]
        assert len(selected) == 5
        assert all((row["wait_version"], row["wrapper"], row["behavior"]) == values for row in selected)

    report = (ROOT / "REPORT.md").read_text()
    questions = (ROOT / "QUESTIONS.md").read_text()
    for token in ("NOT_AVAILABLE", "不成立", "成立", "NOT_OBSERVED", "不给处置建议"):
        assert token in report
    assert "armv7l" in questions and "async" in questions
    print("VALIDATION=PASS item1_rows=30 item3_rows=20 item2=NOT_AVAILABLE item4_claims=5")


if __name__ == "__main__":
    main()
