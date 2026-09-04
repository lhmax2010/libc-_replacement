#!/usr/bin/env python3
"""Independent assertions for the R112 fact-check deliverables."""

from __future__ import annotations

import csv
from pathlib import Path


ROOT = next(parent for parent in Path(__file__).resolve().parents if (parent / ".git").exists())
OUT = ROOT / "docs" / "progress" / "R112"
R110 = ROOT / "docs" / "progress" / "R110" / "tables"


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


reported = read_tsv(R110 / "reported_50_audit.tsv")
clauses = read_tsv(R110 / "strict_clause_sites_original.tsv")
propagating = read_tsv(R110 / "strict_rethrow_sites_original.tsv")
facts = read_tsv(OUT / "FACT_CHECK.tsv")

assert len(reported) == 50
assert sum(row["row_nature"] == "ACTUAL_CATCH_CLAUSE" for row in reported) == 46
assert sum(row["row_nature"] == "NON_CLAUSE_TOKEN_LINE" for row in reported) == 4
assert len(clauses) == 175
assert len(propagating) == 172
assert sum(row["rethrow_kind"] == "LITERAL_THROW" for row in clauses) == 46
assert sum(row["rethrow_kind"] == "LIBSTDCXX_RETHROW_MACRO" for row in clauses) == 126

logical = {
    (row["package_name"], row["file"], row["line"], row["clause_text"], row["rethrow_kind"])
    for row in propagating
}
assert len(logical) == 104
assert sum(row[4] == "LITERAL_THROW" for row in logical) == 41
assert sum(row[4] == "LIBSTDCXX_RETHROW_MACRO" for row in logical) == 63

assert len(facts) == 19
assert {row["severity"] for row in facts} <= {"NONE", "MEDIUM", "HIGH", "CRITICAL"}
assert {"definition_site", "number_104", "reproduction_steps"} <= {
    row["item"] for row in facts if row["severity"] == "CRITICAL"
}

report = (OUT / "REPORT.md").read_text(encoding="utf-8")
replacement = (OUT / "SUGGESTED_REPLACEMENT.md").read_text(encoding="utf-8")
replacement_flat = " ".join(replacement.split())
for needle in (
    "50 个“源码 RPM occurrence/token 行”",
    "104 个“逻辑",
    "只针对原表阳性 11 个源码 RPM 实例",
    "NOT_OBSERVED",
    "没有直接产出 104 行",
):
    assert needle in report, needle
for needle in (
    "50 | 45",
    "46 | 41",
    "172 | **104**",
    "41 and 63 logical sites",
    "not a certified current full-platform total",
    "not run the same unrestricted strict scan",
):
    assert needle in replacement_flat, needle
assert "R110" not in replacement

print("VALIDATION_PASS")
print("facts=19 critical_items=4")
print("source_occurrences: old_rows=50 true_rows=46 corrected_propagating=172")
print("logical_sites: old_rows=45 true_rows=41 corrected_propagating=104")
