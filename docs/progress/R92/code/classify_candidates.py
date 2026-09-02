#!/usr/bin/env python3
"""Classify every broad R87 source candidate; abort if a form is not understood."""

import csv
from collections import Counter
from pathlib import Path

source = Path("progress/R92/tables/source_candidates.tsv")
target = Path("progress/R92/tables/candidate_classification.tsv")
summary_target = Path("progress/R92/tables/candidate_classification_summary.tsv")
rows = list(csv.DictReader(source.open(newline="", encoding="utf-8"), delimiter="\t"))
classified = []
unknown = []
for row in rows:
    kind = row["candidate_kind"]
    text = row["source_text"]
    if kind in {"MEMBER_POINTER_TYPE", "ALIAS_MEMBER_POINTER_TYPE:condition_variable"} and "std::__condvar std::condition_variable::*" in text:
        classification = "DATA_MEMBER_POINTER_NOT_WAIT_FUNCTION"
        reason = "pointer names condition_variable's __condvar data member type; it does not take wait's address or type"
    elif kind == "PRIVATE_HELPER_REFERENCE" and "__do_timed_wait" in text:
        classification = "HELPER_DECLARATION_DEFINITION_OR_DIRECT_CALL"
        reason = "ordinary declaration, definition, or direct call; no address/type/trait use"
    elif kind == "DESTRUCTOR_SPELLING" and "wbuffer_convert" in text:
        classification = "DESTRUCTOR_DECLARATION_OR_DEFINITION"
        reason = "the destructor itself is declared or defined; its address/type is not consumed"
    elif kind == "NOEXCEPT_WAIT_EXPRESSION" and "atomic" in text and ".wait(" in text:
        classification = "UNRELATED_ATOMIC_WAIT"
        reason = "noexcept query targets atomic::wait, not condition_variable::wait"
    else:
        classification = "UNCLASSIFIED"
        reason = "manual review required"
        unknown.append(row)
    classified.append({**row, "classification": classification, "target_use": "NO" if classification != "UNCLASSIFIED" else "NOT_OBSERVED", "reason": reason})

fields = tuple(rows[0]) + ("classification", "target_use", "reason")
with target.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(classified)

counts = Counter(row["classification"] for row in classified)
unique = Counter()
seen = set()
for row in classified:
    key = (row["source_rpm"], row["file"], row["line"], row["source_text"], row["classification"])
    if key not in seen:
        seen.add(key)
        unique[row["classification"]] += 1
with summary_target.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(("classification", "raw_candidate_rows", "distinct_locations", "target_uses"))
    for name in sorted(counts):
        writer.writerow((name, counts[name], unique[name], 0 if name != "UNCLASSIFIED" else "NOT_OBSERVED"))
    writer.writerow(("TOTAL", len(classified), len(seen), 0 if not unknown else "NOT_OBSERVED"))
for name in sorted(counts):
    print(f"{name} raw={counts[name]} distinct={unique[name]}")
print(f"total={len(classified)} distinct={len(seen)} unclassified={len(unknown)} target_uses={0 if not unknown else 'NOT_OBSERVED'}")
raise SystemExit(0 if not unknown else 2)
