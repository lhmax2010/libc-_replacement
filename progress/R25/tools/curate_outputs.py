#!/usr/bin/env python3
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / "progress/R25"
TARGET = ROOT / "docs/progress/R25"
LIMIT = 5 * 1024 * 1024

if TARGET.exists():
    raise SystemExit("curated target already exists; refusing overwrite")
TARGET.mkdir(parents=True)

copied = 0
for source in sorted(SOURCE.rglob("*")):
    if not source.is_file() or source.name == "MANIFEST.sha256":
        continue
    if source.stat().st_size > LIMIT:
        raise SystemExit(f"file exceeds 5 MiB and must be split: {source}")
    relative = source.relative_to(SOURCE)
    target = TARGET / relative
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
    copied += 1

print(f"curated_files={copied}")
