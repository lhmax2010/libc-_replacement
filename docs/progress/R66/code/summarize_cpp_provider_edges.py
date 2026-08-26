#!/usr/bin/env python3
"""Summarize R22 TRUE_CPP_ABI_COUPLING edges by provider source RPM."""

from __future__ import annotations

import argparse
import csv
import glob
from collections import defaultdict


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input-glob", required=True)
    parser.add_argument("--output", required=True)
    args = parser.parse_args()
    data = defaultdict(lambda: {"consumers": set(), "binaries": set(), "rows": 0,
                                "symbols": set(), "arches": set()})
    for path in glob.glob(args.input_glob):
        with open(path, newline="") as stream:
            for row in csv.DictReader(stream, delimiter="\t"):
                if row["classification"] != "TRUE_CPP_ABI_COUPLING":
                    continue
                item = data[row["provider_source_rpm"]]
                item["consumers"].add(row["consumer_source_rpm"])
                item["binaries"].add(row["provider_binary"])
                item["arches"].add(row["arch"])
                item["rows"] += 1
                item["symbols"].update(filter(None, row["cpp_symbols"].split(";")))
    with open(args.output, "w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("provider_source_rpm", "consumer_source_rpm_count", "evidence_rows",
                         "distinct_cpp_symbols", "architectures", "provider_binary_names"))
        for provider, item in sorted(data.items(), key=lambda pair: (-len(pair[1]["consumers"]), pair[0])):
            writer.writerow((provider, len(item["consumers"]), item["rows"], len(item["symbols"]),
                             ";".join(sorted(item["arches"])), ";".join(sorted(item["binaries"]))))


if __name__ == "__main__":
    main()
