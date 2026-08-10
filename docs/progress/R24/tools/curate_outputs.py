#!/usr/bin/env python3
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / "progress/R24"
TARGET = ROOT / "docs/progress/R24"

if TARGET.exists():
    shutil.rmtree(TARGET)
TARGET.mkdir(parents=True)

copied = 0
for source in sorted(SOURCE.rglob("*")):
    if not source.is_file() or source.name == "MANIFEST.sha256":
        continue
    relative = source.relative_to(SOURCE)
    target = TARGET / relative
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
    copied += 1

print(f"curated_files={copied}")
