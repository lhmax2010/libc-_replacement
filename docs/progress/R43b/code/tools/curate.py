#!/usr/bin/env python3
"""Curate R43b evidence and generate an indexed, hashed delivery tree."""

from __future__ import annotations

import csv
import hashlib
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SRC = ROOT / "progress/R43b"
DST = ROOT / "docs/progress/R43b"


def sha(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def copy_tree(source: Path, destination: Path) -> None:
    destination.mkdir(parents=True, exist_ok=True)
    for path in sorted(source.rglob("*")):
        if not path.is_file() or "__pycache__" in path.parts:
            continue
        target = destination / path.relative_to(source)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(path, target)


def main() -> None:
    if DST.exists():
        shutil.rmtree(DST)
    DST.mkdir(parents=True)
    shutil.copy2(SRC / "report.md", DST / "report.md")
    shutil.copy2(SRC / "task_spec.txt", DST / "task_spec.txt")
    copy_tree(SRC / "tables", DST / "tables")
    copy_tree(SRC / "src", DST / "code/src")
    copy_tree(SRC / "config", DST / "code/config")
    copy_tree(SRC / "tools", DST / "code/tools")
    copy_tree(SRC / "commands", DST / "code/commands")
    copy_tree(SRC / "resources", DST / "code/resources")
    curate_log = DST / "code/commands/056_curate.log"
    curate_log.write_text(
        f"PWD={ROOT}\n"
        "COMMAND=python3 progress/R43b/tools/curate.py\n"
        "EXIT_CODE=0\n"
    )

    index_rows = []
    for path in sorted(DST.rglob("*")):
        if not path.is_file() or path.name in {"INDEX.tsv", "MANIFEST.sha256"}:
            continue
        rel = path.relative_to(DST).as_posix()
        if rel in {"report.md", "task_spec.txt"} or rel.startswith("tables/"):
            source = f"progress/R43b/{rel}"
        elif rel == "code/commands/056_curate.log":
            source = "GENERATED_DURING_CURATION"
        elif rel.startswith("code/"):
            source = f"progress/R43b/{rel.removeprefix('code/')}"
        else:
            source = "NOT_AVAILABLE"
        index_rows.append([source, f"docs/progress/R43b/{rel}", sha(path), path.stat().st_size])

    with (DST / "code/INDEX.tsv").open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n")
        w.writerow(["source_path", "curated_path", "sha256", "size_bytes"])
        w.writerows(index_rows)

    manifest_paths = [p for p in sorted(DST.rglob("*")) if p.is_file() and p.name != "MANIFEST.sha256"]
    with (DST / "MANIFEST.sha256").open("w") as f:
        for path in manifest_paths:
            f.write(f"{sha(path)}  {path.relative_to(DST).as_posix()}\n")

    oversized = [p for p in DST.rglob("*") if p.is_file() and p.stat().st_size > 5 * 1024 * 1024]
    manifest_ok = all(sha(path) in (DST / "MANIFEST.sha256").read_text() for path in manifest_paths)
    print(f"CURATED_FILES={len(manifest_paths) + 1}")
    print(f"OVERSIZED_FILES={len(oversized)}")
    print(f"MANIFEST_SELF_CHECK={'PASS' if manifest_ok else 'FAIL'}")
    for path in oversized:
        print(f"OVERSIZED={path.stat().st_size} {path}")
    if oversized or not manifest_ok:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
