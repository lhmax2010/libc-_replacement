#!/usr/bin/env python3
import argparse
import csv
import hashlib
from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("items", nargs="+")
    args = parser.parse_args()
    rows = []
    for item in args.items:
        path_text, ranges = item.rsplit(":", 1)
        path = Path(path_text)
        lines = path.read_text(errors="replace").splitlines()
        sha = hashlib.sha256(path.read_bytes()).hexdigest()
        for value in ranges.split(","):
            first, last = map(int, value.split("-"))
            text = "\n".join(f"{number}: {lines[number-1]}" for number in range(first, last + 1))
            rows.append((str(path), sha, first, last, text))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("path", "sha256", "line_start", "line_end", "excerpt"))
        writer.writerows(rows)
    print(f"evidence_rows={len(rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
