#!/usr/bin/env python3
import hashlib
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SRC = ROOT / "progress/R102"
DST = ROOT / "docs/progress/R102"

relative_files = [
    "README.md",
    "REPORT.md",
    "NO_CHANGE_GROUPS.md",
    "INSUFFICIENT_REASONS.md",
    "tables/base_all_source_packages_rewritten.tsv",
    "tables/base_all_source_packages_rewritten.md",
    "tables/reason_before_after.tsv",
    "tables/reason_before_after.md",
]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


for relative in relative_files:
    source = SRC / relative
    archived = DST / relative
    assert source.is_file() and archived.is_file(), relative
    assert digest(source) == digest(archived), relative

exclude_number = sys.argv[1] if len(sys.argv) > 1 else None
suffixes = (".command.txt", ".stdout", ".stderr", ".exitcode")
groups = defaultdict(set)
for path in (SRC / "raw").iterdir():
    if not path.is_file() or path.name == ".gitkeep":
        continue
    for suffix in suffixes:
        if path.name.endswith(suffix):
            prefix = path.name[: -len(suffix)]
            if exclude_number and prefix.startswith(exclude_number + "_"):
                break
            groups[prefix].add(suffix)
            break
missing = {
    prefix: sorted(set(suffixes) - found)
    for prefix, found in groups.items()
    if found != set(suffixes)
}
assert not missing, missing

print(f"key_material_sha256_matches={len(relative_files)}/{len(relative_files)}")
print(f"complete_raw_command_records={len(groups)}")
print("archive_validation=PASS")
