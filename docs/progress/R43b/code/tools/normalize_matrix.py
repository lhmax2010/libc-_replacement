#!/usr/bin/env python3
"""Convert the compact key/value matrix records into a regular TSV."""

from __future__ import annotations

import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
TABLES = ROOT / "progress/R43b/tables"


def parse(path: Path):
    for raw in path.read_text(errors="replace").splitlines():
        left, exit_field = raw.split("\t", 1)
        fields = dict(item.split("=", 1) for item in left.split())
        fields["exit_code"] = exit_field.split("=", 1)[1]
        section = fields["section"]
        test = fields["test"]
        suite = fields["suite"]
        code = int(fields["exit_code"])
        if section == "A":
            expected = 0
            if test == "forced_unwind4" and fields["arch"] != "aarch64":
                status = "OUTSIDE_UPSTREAM_REQUIRES_OBSERVED"
                observation = "upstream_requires_linux_aarch64; execution_recorded_for_information"
            else:
                status = "PASS" if code == expected else "RED"
                observation = "normal_exit" if code == 0 else "abnormal_exit"
        else:
            if test in {"cout", "getline", "packaged_task"}:
                if suite.startswith("libcxx"):
                    expected = 134
                    status = "REPRODUCED" if code == 134 else "NOT_REPRODUCED"
                    observation = "FATAL_exception_not_rethrown" if code == 134 else "no_abort"
                else:
                    expected = 0
                    status = "CONTROL_PASS" if code == 0 else "CONTROL_RED"
                    observation = "pthread_cancel_joined" if code == 0 else "abnormal_exit"
            elif test == "shrink_to_fit":
                expected = 0
                status = "NOT_OBSERVED" if code == 0 else "RED"
                observation = "internal_catch_path_not_triggered"
            else:
                expected = 0
                status = "CONTROL_PASS" if code == 0 else "CONTROL_RED"
                observation = "pthread_cancel_joined" if code == 0 else "abnormal_exit"
        yield {
            **fields,
            "expected_exit": expected,
            "status": status,
            "observation": observation,
        }


def main() -> None:
    rows = list(parse(TABLES / "x86_matrix_quick.tsv")) + list(parse(TABLES / "arm_matrix_quick.tsv"))
    fields = ["section", "arch", "suite", "test", "exit_code", "expected_exit", "status", "observation"]
    with (TABLES / "runtime_matrix.tsv").open("w", newline="") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    print(f"MATRIX_ROWS={len(rows)}")
    print(f"A_ROWS={sum(r['section'] == 'A' for r in rows)}")
    print(f"B_ROWS={sum(r['section'] == 'B' for r in rows)}")
    print(f"NONZERO_ROWS={sum(int(r['exit_code']) != 0 for r in rows)}")


if __name__ == "__main__":
    main()
