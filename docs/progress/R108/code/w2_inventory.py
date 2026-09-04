#!/usr/bin/env python3
"""Inventory frozen archives and source-embedded binary objects for W2."""

from __future__ import annotations

import argparse
import csv
import json
import os
import re
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

CPP_NAME = re.compile(r"^_Z")
TARGET = re.compile(r"condition_variable.*__do_timed_wait|wbuffer_convert")


def inspect(path: Path) -> tuple[str, str, str, str]:
    try:
        proc = subprocess.run(["nm", "-A", "--no-demangle", "--", str(path)],
                              text=True, stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, timeout=60)
    except (OSError, subprocess.TimeoutExpired) as error:
        return "NOT_AVAILABLE", "NOT_AVAILABLE", "NOT_AVAILABLE", str(error)
    cpp = target = "NO"
    for line in proc.stdout.splitlines():
        fields = line.rsplit(maxsplit=1)
        if len(fields) != 2:
            continue
        name = fields[-1]
        if CPP_NAME.match(name):
            cpp = "YES"
        if TARGET.search(name):
            target = "YES"
    diagnostic = proc.stderr.strip().replace("\t", " ").replace("\n", " | ")
    return cpp, target, str(proc.returncode), diagnostic


def package_from_source(path: Path) -> str:
    return next((part for part in path.parts if part.endswith(".src.rpm")),
                "NOT_AVAILABLE")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--static-paths", required=True, type=Path)
    parser.add_argument("--base-workspace", required=True, type=Path)
    parser.add_argument("--shards", required=True, type=Path)
    parser.add_argument("--source-root", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    parser.add_argument("--skip-source", action="store_true")
    args = parser.parse_args()

    items: list[tuple[str, str, str, Path]] = []
    metadata = {}
    for raw in args.static_paths.read_text().splitlines():
        if not raw:
            continue
        path = Path(raw)
        if not path.is_absolute():
            path = args.base_workspace / path
        parts = path.parts
        try:
            sha = parts[parts.index("extracted") + 2]
        except (ValueError, IndexError):
            sha = "NOT_AVAILABLE"
        if sha not in metadata and sha != "NOT_AVAILABLE":
            try:
                metadata[sha] = json.loads((args.shards / f"{sha}.json").read_text())["package"]
            except (OSError, ValueError, KeyError):
                metadata[sha] = {}
        package = metadata.get(sha, {})
        items.append(("PUBLISHED_STATIC_ARCHIVE",
                      package.get("name", "NOT_AVAILABLE"),
                      package.get("arch", "NOT_AVAILABLE"), path))

    if not args.skip_source:
        for directory, _, filenames in os.walk(args.source_root):
            parent = Path(directory)
            for filename in filenames:
                if not (filename.endswith(".o") or filename.endswith(".so") or ".so." in filename):
                    continue
                path = parent / filename
                kind = ("SOURCE_EMBEDDED_OBJECT" if filename.endswith(".o")
                        else "SOURCE_EMBEDDED_SHARED_CANDIDATE")
                items.append((kind, package_from_source(path), "MULTI_OR_HOST", path))

    args.output.parent.mkdir(parents=True, exist_ok=True)
    counts: dict[tuple[str, str], int] = {}
    with ThreadPoolExecutor(max_workers=2) as pool:
        results = pool.map(lambda item: inspect(item[3]), items)
        with args.output.open("w", newline="") as stream:
            writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
            writer.writerow(("class", "package", "arch", "path", "cpp_indicator",
                             "target_symbol_visible", "nm_exit", "diagnostic"))
            for item, result in zip(items, results):
                writer.writerow((*item[:3], str(item[3]), *result))
                counts[(item[0], "TOTAL")] = counts.get((item[0], "TOTAL"), 0) + 1
                counts[(item[0], f"CPP_{result[0]}")] = counts.get(
                    (item[0], f"CPP_{result[0]}"), 0) + 1
                counts[(item[0], f"TARGET_{result[1]}")] = counts.get(
                    (item[0], f"TARGET_{result[1]}"), 0) + 1
                counts[(item[0], f"NM_EXIT_{result[2]}")] = counts.get(
                    (item[0], f"NM_EXIT_{result[2]}"), 0) + 1
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("class", "metric", "value"))
        for (kind, metric), value in sorted(counts.items()):
            writer.writerow((kind, metric, value))
            print(f"{kind}.{metric}={value}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
