#!/usr/bin/env python3
import csv
import hashlib
import sys
from collections import Counter
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path(__file__).resolve().parents[3]
P = ROOT / "progress/R27"
D = ROOT / "docs/progress/R27"

def rows(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

checks = []
def check(name, ok, observed):
    checks.append((name, bool(ok), observed))

validation = rows(P / "validation.tsv")
check("analyzer_assertions_all_pass", all(r["status"] == "PASS" for r in validation), [r for r in validation if r["status"] != "PASS"])
summary = {r["set"]: r for r in rows(P / "tables/closure_summary.tsv")}
check("closure_summary_75_76_76", [int(summary[x]["source_rpm_count"]) for x in ("SEED", "CONSERVATIVE", "SEMANTIC")] == [75, 76, 76], [summary[x]["source_rpm_count"] for x in ("SEED", "CONSERVATIVE", "SEMANTIC")])
members = rows(P / "tables/closure_members.tsv")
check("closure_members_76", len(members) == 76, len(members))
check("only_one_nonseed", [r["source_rpm"] for r in members if r["seed_75"] == "NO"] == ["libcynara-commons-0.26.0-1.src.rpm"], [r["source_rpm"] for r in members if r["seed_75"] == "NO"])
full = rows(P / "tables/full_graph_semantic_classification.tsv")
check("full_graph_1234", len(full) == 1234, len(full))
check("full_graph_partition", Counter(r["semantic_class"] for r in full) == Counter({"CXX_SEMANTICS": 1221, "UNDETERMINED": 13}), Counter(r["semantic_class"] for r in full))
permanent_edges = rows(P / "tables/permanent_edges_12.tsv")
check("permanent_edges_12_all_vendor_and_CXX", len(permanent_edges) == 12 and all(r["consumer_is_vendor_source"] == "YES" and r["semantic_class"] == "CXX_SEMANTICS" for r in permanent_edges), len(permanent_edges))
permanent = rows(P / "tables/permanent_closure_members.tsv")
check("permanent_members_6", len(permanent) == 6, len(permanent))
check("permanent_all_in_main_closure", all(r["in_main_conservative_closure"] == "YES" and r["in_main_semantic_closure"] == "YES" for r in permanent), [r for r in permanent if r["in_main_conservative_closure"] != "YES"])
required = ["REPORT.md", "METHODOLOGY.md", "README.md", "inputs/input_identity.tsv", "tables/data_sources.tsv", "state.tsv", "EXCLUDED.tsv"]
check("progress_required_files", all((P / x).is_file() for x in required), [x for x in required if not (P / x).is_file()])

if D.exists():
    mismatches = []
    for source in P.rglob("*"):
        if not source.is_file() or source.name == "MANIFEST.sha256":
            continue
        target = D / source.relative_to(P)
        if not target.is_file() or hashlib.sha256(source.read_bytes()).digest() != hashlib.sha256(target.read_bytes()).digest():
            mismatches.append(str(source.relative_to(P)))
    check("curated_matches_progress", not mismatches, mismatches)
    sizes = [x.stat().st_size for x in D.rglob("*") if x.is_file()]
    check("curated_files_le_5MiB", bool(sizes) and max(sizes) <= 5 * 1024 * 1024, max(sizes) if sizes else 0)

for name, ok, observed in checks:
    print(f"{name}\t{observed}\t{'PASS' if ok else 'FAIL'}")
if not all(ok for _, ok, _ in checks):
    raise SystemExit(1)
