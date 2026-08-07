#!/usr/bin/env python3
import hashlib
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SRC = ROOT / "progress/R22"
DST = ROOT / "docs/progress/R22"
MAX = 4_700_000

if any(DST.rglob("*")):
    raise SystemExit("docs/progress/R22 is not empty; refusing overwrite")

def copy(rel, dest=None):
    src = SRC / rel
    out = DST / (dest or rel)
    out.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(src, out)

for name in ["R22_migration_report.md", "R22_board_path_report.md", "R22_patch_directory_report.md"]:
    copy(name)
for rel in ["inputs/selected_inputs.txt", "inputs/input_identity.tsv"]:
    copy(rel)
for path in sorted((SRC / "commands").glob("*.log")):
    copy(path.relative_to(SRC))
for path in sorted((SRC / "tools").glob("*")):
    if path.is_file(): copy(path.relative_to(SRC))
for path in sorted((SRC / "board").glob("*")):
    if path.is_file(): copy(path.relative_to(SRC))
for path in sorted((SRC / "patches").glob("*")):
    if path.is_file(): copy(path.relative_to(SRC))

table_dir = SRC / "tables"
split_registry = []
for path in sorted(table_dir.glob("*.tsv")):
    rel = path.relative_to(SRC)
    size = path.stat().st_size
    if size <= MAX:
        copy(rel)
        split_registry.append((str(rel), str(rel), size, "COPIED"))
        continue
    with path.open("rb") as f:
        header = f.readline()
        part = 1
        payload = bytearray(header)
        rows = 0
        for line in f:
            if rows and len(payload) + len(line) > MAX:
                out_rel = Path("tables/split") / f"{path.stem}.part{part:03d}.tsv"
                out = DST / out_rel; out.parent.mkdir(parents=True, exist_ok=True); out.write_bytes(payload)
                split_registry.append((str(rel), str(out_rel), len(payload), "SPLIT"))
                part += 1; payload = bytearray(header); rows = 0
            payload.extend(line); rows += 1
        if rows:
            out_rel = Path("tables/split") / f"{path.stem}.part{part:03d}.tsv"
            out = DST / out_rel; out.parent.mkdir(parents=True, exist_ok=True); out.write_bytes(payload)
            split_registry.append((str(rel), str(out_rel), len(payload), "SPLIT"))

reg = DST / "SPLIT_REGISTRY.tsv"
reg.write_text("source\tcurated_part\tbytes\tmode\n" + "".join("\t".join(map(str, r)) + "\n" for r in split_registry), encoding="utf-8")
(DST / "EXCLUDED.tsv").write_text("path\tsize\tsha256\treason\n", encoding="utf-8")

def sha(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for b in iter(lambda: f.read(1024 * 1024), b""): h.update(b)
    return h.hexdigest()

manifest = DST / "MANIFEST.sha256"
files = [p for p in sorted(DST.rglob("*")) if p.is_file() and p != manifest]
manifest.write_text("".join(f"{sha(p)}  {p.relative_to(DST)}\n" for p in files), encoding="utf-8")
print("curated_files", len(files) + 1)
print("max_file_size", max(p.stat().st_size for p in files))
