#!/usr/bin/env python3
import hashlib
from pathlib import Path

base=Path(__file__).resolve().parents[1]
out=base/"MANIFEST.sha256"
rows=[]
for p in sorted(base.rglob("*")):
    if not p.is_file() or p==out or p.name=="009_manifest.log": continue
    h=hashlib.sha256()
    with p.open("rb") as f:
        while b:=f.read(4*1024*1024): h.update(b)
    rows.append(f"{h.hexdigest()}  {p.relative_to(base)}\n")
out.write_text("".join(rows),encoding="utf-8")
