#!/usr/bin/env python3
"""Fail-closed consistency checks for the R43b curated conclusions."""

from __future__ import annotations

import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
TABLES = ROOT / "progress/R43b/tables"

with (TABLES / "runtime_matrix.tsv").open() as f:
    matrix = list(csv.DictReader(f, delimiter="\t"))
assert len(matrix) == 90, len(matrix)
nonzero = {(r["section"], r["arch"], r["suite"], r["test"], r["exit_code"]) for r in matrix if r["exit_code"] != "0"}
expected_nonzero = {
    ("A", "armv7l", "libcxx_r42", "forced_unwind1", "134"),
    *(("B", arch, suite, test, "134")
      for arch in ("x86_64", "armv7l")
      for suite in ("libcxx_unpatched", "libcxx_r42")
      for test in ("cout", "getline", "packaged_task")),
}
assert nonzero == expected_nonzero, (nonzero - expected_nonzero, expected_nonzero - nonzero)

with (TABLES / "libstdcxx_guards_65.tsv").open() as f:
    guards = list(csv.DictReader(f, delimiter="\t"))
assert len(guards) == 65, len(guards)
assert len({r["libstdcxx_file"] for r in guards}) == 16

report = (ROOT / "progress/R43b/report.md").read_text()
assert "__CONSUMER_COUNT__" not in report
assert "__THREAD_COUNT__" not in report

print("MATRIX_ROWS=90 PASS")
print("NONZERO_SET=13_EXPECTED_ROWS PASS")
print("LIBSTDCXX_GUARDS=65 PASS")
print("LIBSTDCXX_GUARD_FILES=16 PASS")
print("REPORT_PLACEHOLDERS=0 PASS")
