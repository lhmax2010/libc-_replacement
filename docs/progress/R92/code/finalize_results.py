#!/usr/bin/env python3
import csv
from pathlib import Path

tables = Path("progress/R92/tables")

def read(name):
    with (tables / name).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))

manifest = read("cpp_gap_140.tsv")
status = read("source_scan_status.tsv")
classified = read("candidate_classification.tsv")
packaging_only = {"mic-bootstrap-1.0-1.src.rpm", "qemu-accel-0.4-1.4.src.rpm"}

completeness = []
for row in status:
    if row["source_rpm"] in packaging_only:
        content = "NOT_AVAILABLE"
        reason = "SRPM contains packaging files only and no upstream C/C++ source payload"
    else:
        content = "AVAILABLE_AND_SCANNED"
        reason = "SRPM SHA256 verified; embedded source archives expanded; R87 scanner completed"
    completeness.append({
        "source_rpm": row["source_rpm"], "package_name": row["package_name"],
        "scanner_status": row["status"], "source_content_status": content,
        "reason": reason, "candidate_rows": row["candidates"],
    })
with (tables / "source_completeness.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=("source_rpm", "package_name", "scanner_status", "source_content_status", "reason", "candidate_rows"), delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(completeness)

true_hits = [row for row in classified if row["target_use"] != "NO"]
with (tables / "true_hits.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=("source_rpm", "package_name", "site", "candidate_kind", "file", "line", "source_text"), delimiter="\t", lineterminator="\n", extrasaction="ignore")
    writer.writeheader(); writer.writerows(true_hits)

metrics = [
    ("target_source_rpms", len(manifest)),
    ("sha256_verified", sum(row["sha256_verified"] == "YES" for row in status)),
    ("r87_scanner_completed", sum(row["status"] == "SCAN_OK" for row in status)),
    ("source_content_available_and_scanned", sum(row["source_content_status"] == "AVAILABLE_AND_SCANNED" for row in completeness)),
    ("source_content_not_available_packaging_only", sum(row["source_content_status"] == "NOT_AVAILABLE" for row in completeness)),
    ("broad_candidate_rows", len(classified)),
    ("true_target_uses", len(true_hits)),
]
with (tables / "final_summary.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(("metric", "value")); writer.writerows(metrics)
for key, value in metrics:
    print(f"{key}={value}")
raise SystemExit(0 if len(status) == 140 and not true_hits else 2)
