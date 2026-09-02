#!/usr/bin/env python3
"""Aggregate per-build source RPM edges into logical source-package edges."""

import argparse
import csv
import sys
from collections import Counter, defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
RANK = {
    "LAYOUT_SENSITIVE_STD_TYPE": 5,
    "CPP_ABI_NO_STD_LAYOUT_PROVEN": 4,
    "NOT_AVAILABLE": 3,
    "PURE_C_INTERFACE": 2,
    "OTHER_NO_SYMBOL_INTERSECTION": 1,
}


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--pairs", type=Path, required=True)
    ap.add_argument("--base-source", type=Path, required=True)
    ap.add_argument("--unified-source", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()
    pair_rows = read(args.pairs)
    base_names = {r["location"]: r["name"] for r in read(args.base_source)}
    unified_names = {r["location"]: r["name"] for r in read(args.unified_source)}
    groups = defaultdict(list)
    missing = []
    for row in pair_rows:
        consumer = unified_names.get(row["consumer_sourcerpm"])
        provider = base_names.get(row["provider_sourcerpm"])
        if not consumer or not provider:
            missing.append(row)
            continue
        groups[(consumer, provider)].append(row)
    output = []
    for (consumer, provider), rows in sorted(groups.items()):
        classification = max((r["classification"] for r in rows), key=lambda x: RANK[x])
        cpp = sorted({s for r in rows for s in r["cpp_symbols"].split(";") if s})
        demangled = sorted({s for r in rows for s in r["demangled_cpp_symbols"].split(";") if s})
        classes = Counter(r["classification"] for r in rows)
        output.append({
            "consumer_source_name": consumer,
            "provider_source_name": provider,
            "classification": classification,
            "source_rpm_edge_records": str(len(rows)),
            "elf_edge_count": str(sum(int(r["elf_edge_count"]) for r in rows)),
            "cpp_symbol_count": str(len(cpp)),
            "source_rpm_class_counts": ";".join(f"{k}={v}" for k, v in sorted(classes.items())),
            "consumer_sourcerpms": ";".join(sorted({r["consumer_sourcerpm"] for r in rows})),
            "provider_sourcerpms": ";".join(sorted({r["provider_sourcerpm"] for r in rows})),
            "cpp_symbols": ";".join(cpp),
            "demangled_cpp_symbols": ";".join(demangled),
        })
    fields = ["consumer_source_name", "provider_source_name", "classification", "source_rpm_edge_records",
              "elf_edge_count", "cpp_symbol_count", "source_rpm_class_counts", "consumer_sourcerpms",
              "provider_sourcerpms", "cpp_symbols", "demangled_cpp_symbols"]
    with args.output.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(output)
    print(f"input_source_rpm_edges={len(pair_rows)}")
    print(f"logical_source_name_edges={len(output)}")
    print(f"mapping_failures={len(missing)}")
    for key, value in sorted(Counter(r["classification"] for r in pair_rows).items()):
        print(f"source_rpm_edge_classification[{key}]={value}")
    for key, value in sorted(Counter(r["classification"] for r in output).items()):
        print(f"logical_edge_classification[{key}]={value}")
    for key in ("LAYOUT_SENSITIVE_STD_TYPE", "CPP_ABI_NO_STD_LAYOUT_PROVEN"):
        consumers = {r["consumer_source_name"] for r in output if r["classification"] == key}
        print(f"logical_consumer_sources[{key}]={len(consumers)}")


if __name__ == "__main__":
    main()
