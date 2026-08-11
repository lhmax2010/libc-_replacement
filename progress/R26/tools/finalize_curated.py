#!/usr/bin/env python3
import csv
import hashlib
import os
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / "progress/R26"
TARGET = ROOT / "docs/progress/R26"
LIMIT = 5 * 1024 * 1024
PART_LIMIT = 4 * 1024 * 1024
LARGE_REL = Path("tables/semantic_evidence_symbols.tsv")

log = SOURCE / "commands/008_finalize_curated.log"
log.parent.mkdir(parents=True, exist_ok=True)
log.write_text(f"PWD={os.getcwd()}\nCOMMAND=python3 progress/R26/tools/finalize_curated.py\nEXIT_CODE=0\n", encoding="utf-8")

TARGET.mkdir(parents=True, exist_ok=True)
split_dir = TARGET / "tables/split"
if split_dir.exists():
    shutil.rmtree(split_dir)
split_dir.mkdir(parents=True)

large = SOURCE / LARGE_REL
with large.open("rb") as f:
    header = f.readline()
    chunks = []
    current = []
    current_size = len(header)
    for line in f:
        if current and current_size + len(line) > PART_LIMIT:
            chunks.append(current)
            current = []
            current_size = len(header)
        current.append(line)
        current_size += len(line)
    if current:
        chunks.append(current)

map_rows = []
for i, lines in enumerate(chunks, 1):
    name = f"semantic_evidence_symbols.part{i:03d}.tsv"
    path = split_dir / name
    path.write_bytes(header + b"".join(lines))
    map_rows.append({
        "source_path": LARGE_REL.as_posix(),
        "source_size_bytes": large.stat().st_size,
        "source_sha256": hashlib.sha256(large.read_bytes()).hexdigest(),
        "curated_part_path": f"tables/split/{name}",
        "curated_part_size_bytes": path.stat().st_size,
        "curated_part_sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
        "data_row_count": len(lines),
        "split_reason": "source exceeds 5 MiB; complete row-preserving split",
    })

map_path = SOURCE / "tables/CURATION_MAP.tsv"
with map_path.open("w", newline="", encoding="utf-8") as f:
    fields = list(map_rows[0])
    w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    w.writeheader()
    w.writerows(map_rows)

copied = 0
for source in sorted(SOURCE.rglob("*")):
    if not source.is_file() or source.name == "MANIFEST.sha256" or source.relative_to(SOURCE) == LARGE_REL:
        continue
    if source.stat().st_size > LIMIT:
        raise SystemExit(f"unexpected oversized file: {source}")
    target = TARGET / source.relative_to(SOURCE)
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
    copied += 1

for base in (SOURCE, TARGET):
    rows = []
    for path in sorted(base.rglob("*")):
        if path.is_file() and path.name != "MANIFEST.sha256":
            rows.append((hashlib.sha256(path.read_bytes()).hexdigest(), path.relative_to(base).as_posix()))
    (base / "MANIFEST.sha256").write_text("".join(f"{digest}  {name}\n" for digest, name in rows), encoding="utf-8")

print(f"curated_files={copied} split_parts={len(chunks)} split_rows={sum(len(x) for x in chunks)}")
