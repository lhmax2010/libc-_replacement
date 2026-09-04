#!/usr/bin/env python3
"""Create a compact index over the per-command raw evidence files."""

import sys
from pathlib import Path


raw = Path(sys.argv[1])
output = Path(sys.argv[2])
rows = []
for command_file in sorted(raw.glob("*.command.txt")):
    stem = command_file.name.removesuffix(".command.txt")
    exit_file = raw / f"{stem}.exitcode"
    command = command_file.read_text(encoding="utf-8").strip()
    exitcode = exit_file.read_text(encoding="utf-8").strip() if exit_file.exists() else "NOT_AVAILABLE"
    rows.append((stem, exitcode, command, f"raw/{stem}.stdout.txt", f"raw/{stem}.stderr.txt"))
with open(output, "w", encoding="utf-8") as stream:
    stream.write("id\texit_code\tcommand\tstdout\tstderr\n")
    for row in rows:
        stream.write("\t".join(value.replace("\t", " ").replace("\n", " ") for value in row) + "\n")
print(f"indexed={len(rows)}")
