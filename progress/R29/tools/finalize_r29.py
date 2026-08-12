#!/usr/bin/env python3
from __future__ import annotations

import hashlib
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PROGRESS = ROOT / "progress" / "R29"
DOCS = ROOT / "docs" / "progress" / "R29"
ARTIFACTS = ROOT / "artifacts" / "R29"


def digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def write_manifest(root: Path) -> None:
    manifest = root / "MANIFEST.sha256"
    entries = []
    for path in sorted(root.rglob("*")):
        if not path.is_file() or path == manifest:
            continue
        entries.append(f"{digest(path)}  {path.relative_to(root).as_posix()}\n")
    manifest.write_text("".join(entries), encoding="utf-8")


def main() -> None:
    write_manifest(ARTIFACTS)
    artifact_rows = ["sha256\tpath\tstatus\n"]
    for line in (ARTIFACTS / "MANIFEST.sha256").read_text(encoding="utf-8").splitlines():
        value, relative = line.split("  ", 1)
        status = "SUPERSEDED" if "superseded" in relative else "FINAL"
        artifact_rows.append(f"{value}\tartifacts/R29/{relative}\t{status}\n")
    (PROGRESS / "tables" / "artifact_sha256.tsv").write_text(
        "".join(artifact_rows), encoding="utf-8"
    )
    write_manifest(PROGRESS)
    DOCS.mkdir(parents=True, exist_ok=True)
    shutil.copytree(PROGRESS, DOCS, dirs_exist_ok=True)
    write_manifest(DOCS)


if __name__ == "__main__":
    main()
