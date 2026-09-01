#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path


with Path("docs/progress/R32/tables/scan_status.tsv").open() as stream:
    source_rows = list(csv.DictReader(stream, delimiter="\t"))
print(f"source_records={len(source_rows)}")
print(f"source_unique_packages={len({row['package_name'] for row in source_rows})}")
print("source_statuses=" + repr(sorted(Counter(row["status"] for row in source_rows).items())))

with Path("progress/R11/tables/binary_package_records.tsv").open() as stream:
    binary_rows = list(csv.DictReader(stream, delimiter="\t"))
print(f"binary_total={len(binary_rows)}")
print("binary_arch_counts=" + repr(sorted(Counter(row["arch"] for row in binary_rows).items())))

print("final_elf_summary:")
print(Path("progress/R87/tables/binary_symbol_summary_type_only.tsv").read_text(), end="")
with Path("progress/R87/tables/static_archive_recheck2_summary.tsv").open() as stream:
    metrics = dict(csv.reader(stream, delimiter="\t"))
print(f"final_static_recheck_failures={metrics['scan_failures']}")
