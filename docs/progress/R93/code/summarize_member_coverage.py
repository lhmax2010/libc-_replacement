#!/usr/bin/env python3
import csv
from collections import Counter, defaultdict
from pathlib import Path

source = Path("progress/R93/tables/excluded_archive_members.tsv")
rows = list(csv.DictReader(source.open(newline="", encoding="utf-8"), delimiter="\t"))
by_package = defaultdict(list)
for row in rows:
    by_package[row["source_rpm"]].append(row)

summary_rows = []
for package, group in sorted(by_package.items()):
    types = Counter(row["member_type"] for row in group)
    statuses = Counter(row["target_status"] for row in group)
    unexpected = sum(row["member_type"] != "SYMLINK" or row["target_status"] not in {"INTERNAL_TARGET_INCLUDED", "TARGET_NOT_IN_ARCHIVE"} for row in group)
    if unexpected:
        coverage = "PARTIAL_UNCLASSIFIED_MEMBERS"
    elif statuses["TARGET_NOT_IN_ARCHIVE"]:
        coverage = "CLOSED_FOR_PRESENT_ARCHIVE_CONTENT_WITH_DANGLING_LINKS"
    else:
        coverage = "CLOSED_INTERNAL_TARGETS_INCLUDED"
    summary_rows.append({
        "source_rpm": package,
        "excluded_total": len(group),
        "symlinks": types["SYMLINK"],
        "internal_targets_in_scan_root": statuses["INTERNAL_TARGET_INCLUDED"],
        "targets_not_in_archive": statuses["TARGET_NOT_IN_ARCHIVE"],
        "unclassified": unexpected,
        "updated_coverage": coverage,
    })

fields = ("source_rpm", "excluded_total", "symlinks", "internal_targets_in_scan_root", "targets_not_in_archive", "unclassified", "updated_coverage")
with Path("progress/R93/tables/eight_package_coverage.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(summary_rows)

dangling = [row for row in rows if row["target_status"] == "TARGET_NOT_IN_ARCHIVE"]
with Path("progress/R93/tables/dangling_symlinks.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=tuple(rows[0]), delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(dangling)

print(f"packages={len(summary_rows)} excluded={len(rows)} symlinks={sum(r['symlinks'] for r in summary_rows)}")
print(f"internal_targets={sum(r['internal_targets_in_scan_root'] for r in summary_rows)} dangling={len(dangling)} unclassified={sum(r['unclassified'] for r in summary_rows)}")
for row in summary_rows:
    print("\t".join(str(row[field]) for field in fields))
raise SystemExit(0 if len(summary_rows) == 8 and len(rows) == 1003 and not sum(r["unclassified"] for r in summary_rows) else 2)
