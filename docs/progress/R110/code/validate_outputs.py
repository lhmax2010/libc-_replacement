#!/usr/bin/env python3
"""Independent consistency checks for the archived R110 tables and reports."""

from __future__ import annotations

import csv
from collections import Counter
from pathlib import Path


ROOT = next(parent for parent in Path(__file__).resolve().parents if (parent / ".git").exists())
OUT = ROOT / "docs" / "progress" / "R110"


def rows(name: str) -> list[dict[str, str]]:
    with (OUT / "tables" / name).open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


reported = rows("reported_50_audit.tsv")
clauses = rows("strict_clause_sites_original.tsv")
rethrows = rows("strict_rethrow_sites_original.tsv")
current = rows("current_rethrow_sites.tsv")
identities = rows("current_source_identity.tsv")

assert len(reported) == 50
assert Counter(row["row_nature"] for row in reported) == {
    "ACTUAL_CATCH_CLAUSE": 46,
    "NON_CLAUSE_TOKEN_LINE": 4,
}
assert len(clauses) == 175
assert Counter(row["rethrow_kind"] for row in clauses) == {
    "LITERAL_THROW": 46,
    "LIBSTDCXX_RETHROW_MACRO": 126,
    "NO_RETHROW_OBSERVED": 3,
}
assert len(rethrows) == 172
assert all(row["rethrow_kind"] != "NO_RETHROW_OBSERVED" for row in rethrows)

logical = {
    (
        row["package_name"],
        row["file"],
        row["line"],
        row["clause_text"],
        row["rethrow_kind"],
    )
    for row in rethrows
}
assert len(logical) == 104

assert len(current) == 140
assert Counter(row["package_name"] for row in current) == {
    "boost": 22,
    "gcc": 66,
    "key-manager": 5,
    "libcynara-commons": 1,
    "llvm": 42,
    "security-manager": 1,
    "tensorflow2": 2,
    "webauthn": 1,
}
assert len(identities) == 8
assert len({row["package_name"] for row in identities}) == 8
assert all(len(row["head"]) == 40 for row in identities)

bad_log_paths = {
    (row["file"], row["line"])
    for row in reported
    if row["row_nature"] == "NON_CLAUSE_TOKEN_LINE"
}
assert bad_log_paths == {
    ("src/manager/client-async/client-manager-async.cpp", "322"),
    ("src/manager/client-capi/ckmc-manager.cpp", "1371"),
    ("src/manager/client/client-common.cpp", "328"),
    ("src/manager/client/client-common.cpp", "350"),
}

for table in (reported, clauses, rethrows, current):
    assert all(row["actual_code_line"].strip() for row in table)
    assert all(row["context"].strip() not in {"", "NOT_OBSERVED"} for row in table)

required = {
    "REPORT.md": ["50 行中只有 46 行", "175 个识别 catch 子句", "NOT_OBSERVED"],
    "FOR_HQ_SUMMARY.md": ["50", "46", "172", "104", "140"],
    "METHOD_AND_LIMITS.md": ["guard_status", "__throw_exception_again"],
}
for filename, needles in required.items():
    body = (OUT / filename).read_text(encoding="utf-8")
    for needle in needles:
        assert needle in body, (filename, needle)

print("VALIDATION_PASS")
print("reported_rows=50 clauses=46 log_messages=4")
print("strict_clause_occurrences=175 strict_rethrow_occurrences=172")
print("strict_rethrow_unique_logical_sites=104 current_positive_set_sites=140")
