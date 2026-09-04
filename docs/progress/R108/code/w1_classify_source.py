#!/usr/bin/env python3
"""Classify lexical wbuffer_convert hits conservatively."""

from __future__ import annotations

import argparse
import csv
import re
from pathlib import Path


def split_line(line: str) -> tuple[str, str, str]:
    match = re.match(r"^(.*?):([0-9]+):(.*)$", line.rstrip("\n"))
    if not match:
        raise ValueError(f"unparseable rg row: {line!r}")
    return match.group(1), match.group(2), match.group(3)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    rows = []
    for line in args.input.read_text(errors="replace").splitlines():
        path, lineno, text = split_line(line)
        package = next((part for part in Path(path).parts if part.endswith(".src.rpm")),
                       "CONTROL")
        normalized = path.replace("\\", "/")
        if ("/libcxx/include/__locale_dir/wbuffer_convert.h" in normalized or
                "/libstdc++-v3/include/bits/locale_conv.h" in normalized):
            kind = "STDLIB_IMPLEMENTATION"
        elif "/libcxx/test/" in normalized or "/libstdc++-v3/testsuite/" in normalized:
            kind = "STDLIB_SELF_TEST"
        elif "/docs/" in normalized or normalized.endswith((".md", ".rst", ".html")):
            kind = "DOCUMENTATION"
        else:
            kind = "POTENTIAL_PLATFORM_USER"
        rows.append((package, path, lineno, kind, text.strip()))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source_rpm", "path", "line", "classification", "code"))
        writer.writerows(rows)
    for kind in sorted({row[3] for row in rows}):
        print(f"{kind}={sum(row[3] == kind for row in rows)}")
    print(f"TOTAL={len(rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
