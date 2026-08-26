#!/usr/bin/env python3
"""Generate the R66 delivery material index without hashing itself."""

from __future__ import annotations

import argparse
import hashlib
import pathlib


def sha256(path: pathlib.Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("root", type=pathlib.Path)
    parser.add_argument("output", type=pathlib.Path)
    args = parser.parse_args()

    rows = []
    output = args.output.resolve()
    for path in sorted(args.root.rglob("*")):
        if not path.is_file() or path.resolve() == output:
            continue
        rows.append((str(path.relative_to(args.root)), path.stat().st_size, sha256(path)))

    with args.output.open("w", encoding="utf-8", newline="") as stream:
        stream.write("path\tsize_bytes\tsha256\n")
        for path, size, digest in rows:
            stream.write(f"{path}\t{size}\t{digest}\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
