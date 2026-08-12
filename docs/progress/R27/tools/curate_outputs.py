#!/usr/bin/env python3
import hashlib
import os
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / "progress/R27"
TARGET = ROOT / "docs/progress/R27"
LIMIT = 5 * 1024 * 1024

log = SOURCE / "commands/007_curate_outputs.log"
log.parent.mkdir(parents=True, exist_ok=True)
log.write_text(
    f"PWD={os.getcwd()}\nCOMMAND=python3 progress/R27/tools/curate_outputs.py\nEXIT_CODE=0\n",
    encoding="utf-8",
)

if TARGET.exists():
    shutil.rmtree(TARGET)
TARGET.mkdir(parents=True)

copied = 0
for source in sorted(SOURCE.rglob("*")):
    if not source.is_file() or source.name == "MANIFEST.sha256":
        continue
    if source.stat().st_size > LIMIT:
        raise SystemExit(f"oversized file requires split: {source.relative_to(SOURCE)} size={source.stat().st_size}")
    target = TARGET / source.relative_to(SOURCE)
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
    copied += 1

for base in (SOURCE, TARGET):
    entries = []
    for path in sorted(base.rglob("*")):
        if path.is_file() and path.name != "MANIFEST.sha256":
            entries.append((hashlib.sha256(path.read_bytes()).hexdigest(), path.relative_to(base).as_posix()))
    (base / "MANIFEST.sha256").write_text("".join(f"{digest}  {name}\n" for digest, name in entries), encoding="utf-8")

print(f"curated_files={copied}")
