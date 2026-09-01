#!/usr/bin/env python3
import hashlib
import pathlib
import subprocess
import sys


if len(sys.argv) != 3:
    raise SystemExit("usage: verify_suggested_provenance.py PROVENANCE EXPORT_DIR")

prov_path = pathlib.Path(sys.argv[1])
export_dir = pathlib.Path(sys.argv[2])
entries = {}
for line in prov_path.read_text().splitlines():
    if not line or line.startswith("#"):
        continue
    key, value = line.split("=", 1)
    if key in entries:
        raise SystemExit(f"duplicate key: {key}")
    entries[key] = value

count = int(entries["export_patch_count"])
patches = sorted(export_dir.glob("*.patch"))
assert len(patches) == count, (len(patches), count)

for index, patch in enumerate(patches, 1):
    prefix = f"export_patch_{index:04d}"
    data = patch.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    patch_id = subprocess.check_output(
        ["git", "patch-id", "--stable"], input=data, text=False
    ).decode().split()[0]
    assert entries[f"{prefix}_file"] == patch.name
    assert entries[f"{prefix}_sha256"] == digest
    assert entries[f"{prefix}_patch_id"] == patch_id
    print(f"PASS\t{index}\t{patch.name}\t{digest}\t{patch_id}")

tarball = export_dir / entries["source_tarball"]
tar_digest = hashlib.sha256(tarball.read_bytes()).hexdigest()
assert entries["source_tarball_sha256"] == tar_digest
print(f"PASS\tsource_tarball\t{tarball.name}\t{tar_digest}")
print("SUMMARY\tPASS")
