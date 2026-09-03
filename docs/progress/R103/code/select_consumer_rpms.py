#!/usr/bin/env python3
import csv
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
csv.field_size_limit(sys.maxsize)
sources_path = ROOT / "progress/R98/tables/unified_source_records.tsv"
binaries_path = ROOT / "progress/R98/tables/unified_binary_records.tsv"
targets = {
    "eigen",
    "python-pycrypto",
    "inference-engine-tflite",
    "nnstreamer",
    "nntrainer",
}


def read(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


source_rpm_to_name = {
    row["location"]: row["name"]
    for row in read(sources_path)
    if row["kind"] == "source"
}
selected = []
for row in read(binaries_path):
    source = source_rpm_to_name.get(row["sourcerpm"])
    if source in targets:
        selected.append({**row, "source_name": source})

out = ROOT / "progress/R103/tables/consumer_binary_candidates.tsv"
out.parent.mkdir(parents=True, exist_ok=True)
fields = ["source_name", "name", "arch", "version", "release", "package_size", "checksum", "location", "sourcerpm", "requires", "provides"]
with out.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, delimiter="\t", fieldnames=fields, lineterminator="\n")
    writer.writeheader()
    for row in sorted(selected, key=lambda r: (r["source_name"], r["arch"], r["name"])):
        writer.writerow({field: row[field] for field in fields})

by_source = defaultdict(list)
for row in selected:
    by_source[row["source_name"]].append(row)
for source in sorted(targets):
    rows = by_source[source]
    print(f"{source}\tbinary_rows={len(rows)}\ttotal_bytes={sum(int(r['package_size']) for r in rows)}")
    for arch in sorted({r["arch"] for r in rows}):
        arch_rows = [r for r in rows if r["arch"] == arch]
        print(f"  {arch}\trows={len(arch_rows)}\tbytes={sum(int(r['package_size']) for r in arch_rows)}\tnames={';'.join(sorted(r['name'] for r in arch_rows))}")
print(f"selected_rows={len(selected)}")
print(f"selected_bytes={sum(int(r['package_size']) for r in selected)}")
