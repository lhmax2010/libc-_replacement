#!/usr/bin/env python3
import csv
import hashlib
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
P = ROOT / "progress/R25"
D = ROOT / "docs/progress/R25"

def rows(path):
    csv.field_size_limit(sys.maxsize)
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

checks = []
def check(name, ok, observed):
    checks.append((name, bool(ok), observed))

cut = rows(P / "tables/cut_summary.tsv")
check("six_cuts", len(cut) == 6, len(cut))
check("forward_counts", [int(r["forward_cross_boundary_edge_count"]) for r in cut] == [0,953,574,259,118,12], [r["forward_cross_boundary_edge_count"] for r in cut])
check("reverse_zero", all(int(r["reverse_cross_boundary_edge_count"]) == 0 for r in cut), [r["reverse_cross_boundary_edge_count"] for r in cut])

props = rows(P / "tables/cross_boundary_edge_properties.tsv")
pc = Counter(r["requested_property_bucket"] for r in props)
check("crossing_union_1191", len(props) == 1191, len(props))
check("property_partition", pc == Counter({"HIGH_RISK_OBJECTIVE_MARKERS":1137,"MANUAL_JUDGEMENT_REQUIRED":54}), str(pc))

comps = rows(P / "tables/atomic_component_summary.tsv")
check("components_99", len(comps) == 99, len(comps))
check("component_distribution", Counter(int(r["member_count"]) for r in comps) == Counter({1:97,13:1,373:1}), str(Counter(int(r["member_count"]) for r in comps)))

gtest = rows(P / "tables/gtest_consumers_153.tsv")
gc = Counter(r["stage_disposition"] for r in gtest)
check("gtest_153", len(gtest) == 153, len(gtest))
check("gtest_partition", gc == Counter({"PHASE1":4,"PHASE2":121,"PHASE3":10,"VENDOR_PREBUILT_SEPARATE":1,"NOT_IN_R22_483_SCOPE":17}), str(gc))

vendor = rows(P / "tables/vendor_five_to_four_accounting.tsv")
check("vendor_five_names", len(vendor) == 5, len(vendor))
check("vendor_four_sources", len({r["source_rpm_identity"] for r in vendor}) == 4, len({r["source_rpm_identity"] for r in vendor}))
check("pen_wave_overlap_named", sum(r["source_rpm_identity"] == "pen-wave-1.4.0-2.src.rpm" for r in vendor) == 2, sum(r["source_rpm_identity"] == "pen-wave-1.4.0-2.src.rpm" for r in vendor))

required = ["README.md", "METHODOLOGY.md", "REPORT.md", "EXCLUDED.tsv", "validation.tsv", "inputs/input_identity.tsv", "tables/data_sources.tsv"]
check("required_files", all((P / r).is_file() for r in required), [r for r in required if not (P/r).is_file()])
check("all_progress_files_le_5MiB", all(x.stat().st_size <= 5*1024*1024 for x in P.rglob("*") if x.is_file()), max(x.stat().st_size for x in P.rglob("*") if x.is_file()))

if D.exists():
    mismatches = []
    for path in P.rglob("*"):
        if path.is_file() and path.name != "MANIFEST.sha256":
            other = D / path.relative_to(P)
            if not other.is_file() or digest(path) != digest(other):
                mismatches.append(str(path.relative_to(P)))
    check("curated_matches_progress", not mismatches, mismatches)

for name, ok, observed in checks:
    print(f"{name}\t{observed}\t{'PASS' if ok else 'FAIL'}")
if not all(ok for _, ok, _ in checks):
    raise SystemExit(1)
