#!/usr/bin/env python3
import hashlib
import shutil
from pathlib import Path

root = Path(__file__).resolve().parents[3]
src = root / "progress/R32"
dst = root / "docs/progress/R32"
limit = 5_000_000
chunk_limit = 4_500_000

if dst.exists():
    raise SystemExit(f"destination already exists: {dst}")
dst.mkdir(parents=True)

def digest(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()

excluded = []
split_index = []
for path in sorted(p for p in src.rglob("*") if p.is_file()):
    rel = path.relative_to(src)
    if rel.as_posix() in ("MANIFEST.sha256", "EXCLUDED.tsv"):
        continue
    size = path.stat().st_size
    target = dst / rel
    if size <= limit:
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(path, target)
        continue
    if path.suffix == ".tsv":
        lines = path.read_bytes().splitlines(keepends=True)
        header = lines[0]
        parts = []
        body = []
        current = len(header)
        for line in lines[1:]:
            if body and current + len(line) > chunk_limit:
                parts.append(body)
                body = []
                current = len(header)
            body.append(line)
            current += len(line)
        if body or not parts:
            parts.append(body)
        target.parent.mkdir(parents=True, exist_ok=True)
        for index, rows in enumerate(parts, 1):
            part = target.with_name(f"{target.stem}.part{index:03d}.tsv")
            part.write_bytes(header + b"".join(rows))
            split_index.append((rel.as_posix(), part.relative_to(dst).as_posix(),
                                len(rows), part.stat().st_size, digest(part)))
    else:
        excluded.append((rel.as_posix(), size, digest(path), "single file exceeds 5 MB; full file remains in progress/R32"))

excluded_path = src / "EXCLUDED.tsv"
with excluded_path.open("w") as f:
    f.write("path\tsize_bytes\tsha256\treason\n")
    for row in excluded:
        f.write("\t".join(map(str, row)) + "\n")
shutil.copy2(excluded_path, dst / "EXCLUDED.tsv")

if split_index:
    index_path = dst / "SPLIT_INDEX.tsv"
    with index_path.open("w") as f:
        f.write("source_path\tcurated_part\tdata_rows\tsize_bytes\tsha256\n")
        for row in split_index:
            f.write("\t".join(map(str, row)) + "\n")

def manifest(base):
    rows = []
    for path in sorted(p for p in base.rglob("*") if p.is_file() and p.name != "MANIFEST.sha256"):
        rows.append(f"{digest(path)}  {path.relative_to(root)}\n")
    (base / "MANIFEST.sha256").write_text("".join(rows))

manifest(src)
manifest(dst)
print(f"curated_files={sum(1 for p in dst.rglob('*') if p.is_file())} excluded={len(excluded)} split_parts={len(split_index)}")
