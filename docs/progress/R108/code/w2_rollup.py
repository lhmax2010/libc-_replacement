#!/usr/bin/env python3
import argparse
import csv
from collections import Counter
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    counts = Counter()
    with args.input.open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            key = (row["class"], row["package"])
            counts[key + ("TOTAL",)] += 1
            counts[key + (f'CPP_{row["cpp_indicator"]}',)] += 1
            counts[key + (f'TARGET_{row["target_symbol_visible"]}',)] += 1
            if row["nm_exit"] != "0":
                counts[key + ("NM_NONZERO",)] += 1
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("class", "package", "metric", "value"))
        for key, value in sorted(counts.items()):
            writer.writerow((*key, value))
    print(f"package_groups={len({key[:2] for key in counts})}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
