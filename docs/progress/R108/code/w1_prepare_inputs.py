#!/usr/bin/env python3
"""Prepare immutable path inputs for W1 without modifying source corpora."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", required=True, type=Path)
    parser.add_argument("--roots", required=True, type=Path)
    parser.add_argument("--control-elf", type=Path)
    parser.add_argument("--control-shards", type=Path)
    parser.add_argument("--control-extracted", type=Path)
    parser.add_argument("--exclude-prefix", action="append", default=[])
    args = parser.parse_args()
    roots = sorted(path.resolve() for path in args.corpus.iterdir()
                   if path.is_dir() and not any(
                       path.name.startswith(prefix) for prefix in args.exclude_prefix))
    args.roots.parent.mkdir(parents=True, exist_ok=True)
    args.roots.write_text("".join(f"{path}\n" for path in roots))
    print(f"source_roots={len(roots)}")
    if args.control_elf is not None:
        if args.control_shards is None or args.control_extracted is None:
            parser.error("control paths must be supplied together")
        data = args.control_elf.read_bytes()
        import hashlib
        sha = hashlib.sha256(data).hexdigest()
        disk = args.control_extracted / sha[:2] / sha / "usr/lib/libwbuffer-positive.so"
        disk.parent.mkdir(parents=True, exist_ok=True)
        disk.write_bytes(data)
        args.control_shards.mkdir(parents=True, exist_ok=True)
        record = {
            "package": {"repo_id": "CONTROL", "name": "wbuffer-positive",
                        "arch": "x86_64", "actual_sha256": sha},
            "elfs": [{"path": "/usr/lib/libwbuffer-positive.so"}],
        }
        (args.control_shards / f"{sha}.json").write_text(json.dumps(record) + "\n")
        print(f"control_sha256={sha}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
