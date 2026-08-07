#!/usr/bin/env python3
import hashlib
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SRC = ROOT / "progress/R22"
DST = ROOT / "docs/progress/R22"

# Refresh the curation command after it has closed; add this helper itself.
shutil.copyfile(SRC / "commands/026_curate_outputs.log", DST / "commands/026_curate_outputs.log")
shutil.copyfile(SRC / "tools/finalize_curated_manifest.py", DST / "tools/finalize_curated_manifest.py")
shutil.copyfile(SRC / "tools/generate_reports_and_patch_docs.py", DST / "tools/generate_reports_and_patch_docs.py")
shutil.copyfile(SRC / "tools/validate_outputs.py", DST / "tools/validate_outputs.py")
shutil.copyfile(SRC / "R22_migration_report.md", DST / "R22_migration_report.md")
shutil.copyfile(SRC / "board/board_path_ranked.tsv", DST / "board/board_path_ranked.tsv")

def sha(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for b in iter(lambda: f.read(1024 * 1024), b""): h.update(b)
    return h.hexdigest()

manifest = DST / "MANIFEST.sha256"
files = [p for p in sorted(DST.rglob("*")) if p.is_file() and p != manifest]
manifest.write_text("".join(f"{sha(p)}  {p.relative_to(DST)}\n" for p in files), encoding="utf-8")
print("manifest_entries", len(files))
