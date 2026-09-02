#!/usr/bin/env python3
"""Resolve Base->Base runtime edges into the providers visible from Unified."""

import csv
import sys
from collections import defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


binaries = read("progress/R98/tables/base_binary_records.tsv")
source_name = {row["location"]: row["name"] for row in read("progress/R98/tables/base_source_records.tsv")}
for row in binaries:
    row["source_name"] = source_name.get(row["sourcerpm"], "NOT_AVAILABLE")

provider_srpms = {
    row["provider_sourcerpm"]
    for row in read("progress/R100/tables/armv7l_unified_to_base_edges.tsv")
}
providers = [row for row in binaries if row["sourcerpm"] in provider_srpms]
by_capability = defaultdict(list)
for row in providers:
    for capability in filter(None, row["provides"].split(";")):
        by_capability[(row["arch"], capability)].append(row)

edges = {}
for consumer in binaries:
    for requirement in filter(None, consumer["requires"].split(";")):
        if ".so" not in requirement:
            continue
        for provider in by_capability.get((consumer["arch"], requirement), []):
            if consumer["checksum"] == provider["checksum"]:
                continue
            key = (consumer["checksum"], provider["checksum"], requirement)
            edges[key] = {
                "arch": consumer["arch"],
                "consumer_sourcerpm": consumer["sourcerpm"],
                "consumer_binary": consumer["name"],
                "consumer_rpm_sha256": consumer["checksum"],
                "requirement": requirement,
                "provider_sourcerpm": provider["sourcerpm"],
                "provider_binary": provider["name"],
                "provider_rpm_sha256": provider["checksum"],
                "provider_location": provider["location"],
            }

rows = [edges[key] for key in sorted(edges)]
fields = list(rows[0])
out = Path("progress/R100/tables/base_internal_provider_edges.tsv")
with out.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
arm = [row for row in rows if row["arch"] == "armv7l"]
with Path("progress/R100/tables/armv7l_base_internal_provider_edges.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(arm)
print(f"all_edge_records={len(rows)}")
print(f"all_source_pairs={len({(r['consumer_sourcerpm'], r['provider_sourcerpm']) for r in rows})}")
print(f"armv7l_edge_records={len(arm)}")
print(f"armv7l_source_pairs={len({(r['consumer_sourcerpm'], r['provider_sourcerpm']) for r in arm})}")
