#!/usr/bin/env python3
import csv
import sys
from collections import Counter
from pathlib import Path

work, output = map(Path, sys.argv[1:])
rows = []
for path in sorted(work.glob("*.tsv")):
    with path.open(newline="", encoding="utf-8") as stream:
        rows.extend(csv.DictReader(stream, delimiter="\t"))
fields = ("source_rpm", "archive", "member", "member_type", "excluded_class", "link_target", "target_status", "resolved_target")
output.mkdir(parents=True, exist_ok=True)
with (output / "excluded_archive_members.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(rows)

type_counts = Counter(row["excluded_class"] for row in rows)
target_counts = Counter((row["member_type"], row["target_status"]) for row in rows if row["member_type"] in {"SYMLINK", "HARDLINK"})
with (output / "excluded_member_summary.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(("dimension", "member_type", "target_status", "count"))
    for name, count in sorted(type_counts.items()):
        writer.writerow(("EXCLUDED_CLASS", name, "", count))
    for (member_type, target_status), count in sorted(target_counts.items()):
        writer.writerow(("LINK_TARGET", member_type, target_status, count))
    writer.writerow(("TOTAL", "ALL", "", len(rows)))
print(f"excluded_total={len(rows)}")
for name, count in sorted(type_counts.items()):
    print(f"excluded_class={name} count={count}")
for (member_type, target_status), count in sorted(target_counts.items()):
    print(f"link_target={member_type}:{target_status} count={count}")
raise SystemExit(0 if len(rows) == 1003 else 2)
