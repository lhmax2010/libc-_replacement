#!/usr/bin/env python3
import hashlib
import shutil
from pathlib import Path

root=Path.cwd(); source=root/"progress/R23"; dest=root/"docs/progress/R23"
dest.mkdir(parents=True,exist_ok=True)

def sha(path):
    h=hashlib.sha256()
    with path.open("rb") as f:
        while True:
            b=f.read(1024*1024)
            if not b:break
            h.update(b)
    return h.hexdigest()

def manifest(base,path,exclude_names=()):
    files=[p for p in base.rglob("*") if p.is_file() and p.name not in exclude_names]
    lines=[f"{sha(p)}  {p.relative_to(base)}\n" for p in sorted(files)]
    path.write_text("".join(lines),encoding="utf-8")

manifest(source,source/"MANIFEST.sha256",{"MANIFEST.sha256"})

# Copy every evidence file except the one >5 MB table, which is split below.
large=source/"tables/runtime_elf_scan_results.tsv"
for p in sorted(source.rglob("*")):
    if not p.is_file() or p==large:continue
    rel=p.relative_to(source); q=dest/rel; q.parent.mkdir(parents=True,exist_ok=True)
    shutil.copy2(p,q)

split_dir=dest/"tables/split"; split_dir.mkdir(parents=True,exist_ok=True)
max_bytes=4_500_000
with large.open("rb") as f:
    header=f.readline(); part=1; out=None; size=0
    for line in f:
        if out is None or size+len(line)>max_bytes:
            if out is not None:out.close()
            q=split_dir/f"runtime_elf_scan_results.part{part:03d}.tsv"
            out=q.open("wb");out.write(header);size=len(header);part+=1
        out.write(line);size+=len(line)
    if out is not None:out.close()

excluded=dest/"EXCLUDED.tsv"
parts=sorted(split_dir.glob("runtime_elf_scan_results.part*.tsv"))
excluded.write_text(
    "source_file\tsize_bytes\tsha256\tcuration_action\tcurated_parts\n"+
    f"{large.relative_to(root)}\t{large.stat().st_size}\t{sha(large)}\tSPLIT_SINGLE_FILE_OVER_5MB\t"+
    ";".join(str(p.relative_to(root)) for p in parts)+"\n",encoding="utf-8")

manifest(dest,dest/"MANIFEST.sha256",{"MANIFEST.sha256"})
oversized=[p for p in dest.rglob("*") if p.is_file() and p.stat().st_size>5_000_000]
print(f"progress_manifest_entries={sum(1 for _ in (source/'MANIFEST.sha256').open())}")
print(f"curated_parts={len(parts)} curated_oversized_files={len(oversized)}")
for p in oversized:print("OVERSIZED",p,p.stat().st_size)
if oversized:raise SystemExit(1)
