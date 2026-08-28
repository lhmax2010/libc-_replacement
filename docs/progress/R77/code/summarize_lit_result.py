#!/usr/bin/env python3
"""Write code counts and complete non-success records for a lit JSON result."""

import argparse
import collections
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("result", type=Path)
    parser.add_argument("--summary", type=Path, required=True)
    parser.add_argument("--failures", type=Path, required=True)
    args = parser.parse_args()

    data = json.loads(args.result.read_text())
    counts = collections.Counter(item["code"] for item in data["tests"])
    args.summary.write_text(
        "field\tvalue\n"
        f"result\t{args.result}\n"
        f"total\t{len(data['tests'])}\n"
        + "".join(f"code_{code}\t{counts[code]}\n" for code in sorted(counts)),
        encoding="utf-8",
    )
    failures = [
        item
        for item in data["tests"]
        if item["code"] in {"FAIL", "XPASS", "UNRESOLVED", "TIMEOUT"}
    ]
    args.failures.write_text(json.dumps(failures, indent=2) + "\n", encoding="utf-8")
    print(args.summary.read_text(), end="")
    print(f"failure_records\t{len(failures)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
