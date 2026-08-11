#!/usr/bin/env python3
import hashlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
for relative_root in ("progress/R25", "docs/progress/R25"):
    base = ROOT / relative_root
    rows = []
    for path in sorted(base.rglob("*")):
        if path.is_file() and path.name != "MANIFEST.sha256":
            rows.append((hashlib.sha256(path.read_bytes()).hexdigest(), path.relative_to(base).as_posix()))
    (base / "MANIFEST.sha256").write_text("".join(f"{digest}  {name}\n" for digest, name in rows), encoding="utf-8")
    print(f"{relative_root}: manifest_rows={len(rows)}")
