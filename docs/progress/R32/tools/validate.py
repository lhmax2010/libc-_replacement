#!/usr/bin/env python3
import csv
import subprocess
from collections import Counter
from pathlib import Path

root = Path(__file__).resolve().parents[3]
task = root / "progress/R32"
tables = task / "tables"


def rows(name):
    with (tables / name).open(newline="") as handle:
        return list(csv.DictReader(handle, delimiter="\t"))


packages = rows("impact_package_classification.tsv")
scan = rows("scan_status.tsv")
summary = rows("impact_summary_detailed.tsv")
matrix = rows("candidate_behavior_matrix.tsv")
directions = rows("candidate_direction_summary.tsv")
red = rows("r30_red_item_classification.tsv")
wave = rows("wave1_cancel_path_findings.tsv")

assert len(packages) == 371, len(packages)
assert len(scan) == 371, len(scan)
assert Counter(r["status"] for r in scan) == {"SCAN_OK": 371}
assert sum(int(r["source_package_count"]) for r in summary) == 371
assert len(matrix) == 30, len(matrix)
assert {r["direction"] for r in directions} == {"A", "B", "C", "D"}
assert all(r["simultaneously_meets_all_three_observed_conditions"] == "NO" for r in directions)
assert {r["red_item"] for r in red} == {"_S_badbit", "__filebuf_type", "Boost.Test_ABI"}
assert {r["target_source_package"] for r in wave} == {"libcynara-commons", "security-manager"}
assert sum(int(r["propagation_locations"]) for r in wave) == 44
assert "PENDING" not in (task / "tables/supplemental_scan_attempts.tsv").read_text()
assert "SCAN_OK 371/371" in (task / "R32_report.md").read_text()

for repo in ("libcynara-commons", "libcynara-dbus", "security-manager"):
    cp = subprocess.run(
        ["git", "-C", str(root / "codes" / repo), "status", "--porcelain"],
        text=True, capture_output=True, check=True)
    assert cp.stdout == "", f"dirty source repo: {repo}: {cp.stdout}"

print("VALIDATION_PASS")
print("scanned_source_packages=371")
print("scan_ok=371 scan_failed=0")
print("classification_total=371")
print("candidate_matrix_rows=30")
print("candidate_directions=A,B,C,D; all_three_conditions=NO for all")
print("propagation_locations=44")
print("target_source_worktrees_clean=3/3")
