#!/usr/bin/env python3
import csv
import sys
from collections import defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
root = Path(__file__).resolve().parents[3]


def read(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def rpm_map(path):
    return {row["location"]: row["name"] for row in read(path) if row["kind"] == "source"}


base = rpm_map(root / "progress/R98/tables/base_source_records.tsv")
unified = rpm_map(root / "progress/R98/tables/unified_source_records.tsv")
edges = read(root / "docs/progress/R100/tables/armv7l_cross_edges/actual_cross_source_edge_classification.tsv")
by_provider = defaultdict(list)
for row in edges:
    by_provider[base[row["provider_sourcerpm"]]].append(
        (unified[row["consumer_sourcerpm"]], row["classification"], row["detail_classes"])
    )

for provider in sorted(by_provider):
    print(provider)
    for consumer, classification, details in sorted(by_provider[provider]):
        print(f"  {consumer}\t{classification}\t{details}")
