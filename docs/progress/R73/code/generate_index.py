#!/usr/bin/env python3
"""Generate R73's material index, excluding the self-referential index."""

from __future__ import annotations

import csv
import hashlib
from pathlib import Path


ROOT = Path("docs/progress/R73")
OUTPUT = ROOT / "INDEX.tsv"
EXCLUDED = {"INDEX.tsv"}


def source_for(path: Path) -> str:
    relative = path.relative_to(ROOT).as_posix()
    if relative == "code/r72_noexcept_cancel_samples.cpp":
        return "docs/progress/R72/code/noexcept_cancel_samples.cpp"
    return f"progress/R73/{relative}"


def main() -> int:
    rows: list[tuple[str, str, str, int]] = []
    for path in sorted(ROOT.rglob("*")):
        if not path.is_file():
            continue
        relative = path.relative_to(ROOT).as_posix()
        if relative in EXCLUDED:
            continue
        digest = hashlib.sha256(path.read_bytes()).hexdigest()
        rows.append((source_for(path), path.as_posix(), digest, path.stat().st_size))

    with OUTPUT.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source", "uploaded_path", "sha256", "size_bytes"))
        writer.writerows(rows)
    print(f"indexed_files={len(rows)}")
    print(f"excluded={','.join(sorted(EXCLUDED))}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
