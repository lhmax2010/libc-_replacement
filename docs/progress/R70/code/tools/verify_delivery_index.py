#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import pathlib
import sys

ROOT = pathlib.Path("/home/toolchain/development/libc++_replacement")
DELIVERY = ROOT / "docs/progress/R70"


def digest(path: pathlib.Path) -> str:
    hasher = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            hasher.update(block)
    return hasher.hexdigest()


def main() -> int:
    failures = 0
    count = 0
    with (DELIVERY / "INDEX.tsv").open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            count += 1
            uploaded = DELIVERY / row["upload_path"]
            source = ROOT / row["source_path"]
            expected_hash = row["sha256"]
            expected_size = int(row["size_bytes"])
            problems: list[str] = []
            if not uploaded.is_file():
                problems.append("UPLOAD_MISSING")
            else:
                if uploaded.stat().st_size != expected_size:
                    problems.append("UPLOAD_SIZE_MISMATCH")
                if digest(uploaded) != expected_hash:
                    problems.append("UPLOAD_HASH_MISMATCH")
            if not source.is_file():
                problems.append("SOURCE_MISSING")
            elif digest(source) != expected_hash:
                problems.append("SOURCE_HASH_MISMATCH")
            if problems:
                failures += 1
                print(f"FAIL\t{row['upload_path']}\t{','.join(problems)}")
    print(f"index_entries={count}")
    print(f"index_failures={failures}")
    return 0 if failures == 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
