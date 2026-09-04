#!/usr/bin/env python3
import hashlib
from pathlib import Path

root = Path(__file__).resolve().parents[1]
output = root / "MANIFEST.sha256"
rows = []
for path in sorted(root.rglob("*")):
    if not path.is_file() or path == output:
        continue
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    rows.append(f"{digest}  {path.relative_to(root)}\n")
output.write_text("".join(rows))
print(f"manifest_files={len(rows)}")
