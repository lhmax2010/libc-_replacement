#!/usr/bin/env python3
import argparse
import csv
import hashlib
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", required=True)
    parser.add_argument("inputs", nargs="+")
    args = parser.parse_args()
    rows = []
    for spec in args.inputs:
        path_text, ranges_text = spec.rsplit(":", 1)
        path = Path(path_text)
        lines = path.read_text(errors="replace").splitlines()
        digest = hashlib.sha256(path.read_bytes()).hexdigest()
        for item in ranges_text.split(","):
            start_text, end_text = item.split("-")
            start, end = int(start_text), int(end_text)
            excerpt = "\n".join(f"{number}: {lines[number - 1]}" for number in range(start, end + 1))
            rows.append((str(path), digest, start, end, excerpt))
    output = Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    with output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source_path", "sha256", "line_start", "line_end", "excerpt"))
        writer.writerows(rows)
    print(f"evidence_rows={len(rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
