#!/usr/bin/env python3
import hashlib
from pathlib import Path

root = Path("/home/toolchain/development/libc++_replacement/docs/progress/R45")
index_path = root / "INDEX.tsv"
manifest_path = root / "MANIFEST.sha256"

def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            value.update(chunk)
    return value.hexdigest()

files = sorted(path for path in root.rglob("*") if path.is_file() and path not in {index_path, manifest_path})
with index_path.open("w") as stream:
    stream.write("path\tsize_bytes\tsha256\n")
    for path in files:
        stream.write(f"{path.relative_to(root)}\t{path.stat().st_size}\t{digest(path)}\n")

manifest_files = sorted(path for path in root.rglob("*") if path.is_file() and path != manifest_path)
with manifest_path.open("w") as stream:
    for path in manifest_files:
        stream.write(f"{digest(path)}  {path.relative_to(root)}\n")

print(f"INDEX_ROWS={len(files)}")
print(f"MANIFEST_ROWS={len(manifest_files)}")
