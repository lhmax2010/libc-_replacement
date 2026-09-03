#!/usr/bin/env python3
import sys
from collections import defaultdict
from pathlib import Path

raw = Path(__file__).resolve().parents[1] / "raw"
exclude_number = sys.argv[1] if len(sys.argv) > 1 else None
suffixes = (".command.txt", ".stdout", ".stderr", ".exitcode")
groups = defaultdict(set)
for path in raw.iterdir():
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
print(f"complete_raw_command_records={len(groups)}")
print("raw_record_validation=PASS")
