#!/usr/bin/env python3
import hashlib
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
src=ROOT/"progress/R22/inputs/selected_inputs.txt"
out=ROOT/"progress/R22/inputs/input_identity.tsv"
with out.open("w",encoding="utf-8") as d:
    d.write("path\tsize_bytes\tsha256\tstatus\n")
    for rel in src.read_text().splitlines():
        if not rel: continue
        p=ROOT/rel
        if not p.is_file(): d.write(f"{rel}\t\t\tNOT_FOUND\n"); continue
        h=hashlib.sha256()
        with p.open("rb") as f:
            while b:=f.read(8*1024*1024): h.update(b)
        d.write(f"{rel}\t{p.stat().st_size}\t{h.hexdigest()}\tPASS\n")
