#!/usr/bin/env python3
import csv
from pathlib import Path

r92 = Path("docs/progress/R92/tables")
r93 = Path("progress/R93/tables")

def read(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))

coverage = {row["source_rpm"]: row for row in read(r93 / "eight_package_coverage.tsv")}
rows = read(r92 / "source_completeness.tsv")
counts = {}
for row in rows:
    detail = coverage.get(row["source_rpm"])
    if detail is None:
        status = row["source_content_status"]
    elif detail["targets_not_in_archive"] != "0":
        status = "PARTIAL_DANGLING_LINK_TARGETS_NOT_AVAILABLE"
        row["reason"] = (
            f"R93: {detail['excluded_total']} excluded entries were symlinks; "
            f"{detail['internal_targets_in_scan_root']} internal targets were in the scan root; "
            f"{detail['targets_not_in_archive']} dangling targets were absent from the archive"
        )
    else:
        status = "AVAILABLE_AND_SCANNED_R93_LINKS_CLOSED"
        row["reason"] = (
            f"R93: all {detail['excluded_total']} excluded entries were symlinks whose "
            "targets were ordinary files/directories in the scan root; no unique link content"
        )
    row["source_content_status"] = status
    counts[status] = counts.get(status, 0) + 1

expected = {
    "AVAILABLE_AND_SCANNED": 130,
    "AVAILABLE_AND_SCANNED_R93_LINKS_CLOSED": 6,
    "PARTIAL_DANGLING_LINK_TARGETS_NOT_AVAILABLE": 2,
    "NOT_AVAILABLE": 2,
}
if counts != expected:
    raise SystemExit(f"unexpected status counts: {counts!r}")

fields = ("source_rpm", "package_name", "scanner_status", "source_content_status", "reason", "candidate_rows")
with (r92 / "source_completeness.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(rows)

metrics = [
    ("target_source_rpms", 140),
    ("sha256_verified", 140),
    ("r87_scanner_completed", 140),
    ("source_content_cleanly_expanded_and_scanned_in_r92", 130),
    ("source_content_partial_before_r93", 8),
    ("source_content_promoted_after_r93_link_classification", 6),
    ("source_content_complete_after_r93", 136),
    ("source_content_partial_dangling_targets_after_r93", 2),
    ("source_content_not_available_packaging_only", 2),
    ("broad_candidate_rows", 150),
    ("true_target_uses", 0),
]
with (r92 / "final_summary.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(("metric", "value")); writer.writerows(metrics)

elf_coverage = read(r92 / "cpp_gap_binary_coverage.tsv")
if len(elf_coverage) != 4374:
    raise SystemExit(f"unexpected ELF coverage rows: {len(elf_coverage)}")
failed_source = "u-boot-rubikpi3-2025.10-0.src.rpm"
failed_path = "/boot/u-boot.mbn"
if any(row["source_rpm"] == failed_source or row["path"] == failed_path for row in elf_coverage):
    raise SystemExit("R87 failed ELF unexpectedly appears in R92 coverage")
for row in elf_coverage:
    row["r87_elf_scan_scope"] = "INCLUDED_AND_PARSED_R93_VERIFIED"
    row["basis"] = "R93 verified this row is not R87's sole parse failure (u-boot-rubikpi3:/boot/u-boot.mbn)"
elf_fields = tuple(elf_coverage[0])
with (r92 / "cpp_gap_binary_coverage.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=elf_fields, delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(elf_coverage)

coverage_metrics = read(r92 / "coverage_summary.tsv")
coverage_metrics = [row for row in coverage_metrics if row["metric"] not in {
    "cpp_gap_elf_rows_in_r87_scope", "cpp_gap_elf_rows_successfully_parsed", "r87_total_parse_failures"
}]
coverage_metrics.extend([
    {"metric": "cpp_gap_elf_rows_in_r87_scope", "value": "4374"},
    {"metric": "cpp_gap_elf_rows_successfully_parsed", "value": "4374"},
    {"metric": "r87_total_parse_failures", "value": "1"},
])
with (r92 / "coverage_summary.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=("metric", "value"), delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(coverage_metrics)

for status, count in sorted(counts.items()):
    print(f"{status}={count}")
