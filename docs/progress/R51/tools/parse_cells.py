#!/usr/bin/env python3
import re
import sys
from pathlib import Path

if len(sys.argv) != 3:
    raise SystemExit("usage: parse_cells.py INPUT OUTPUT")

source = Path(sys.argv[1])
target = Path(sys.argv[2])
cells = []
current = None
for line_no, raw in enumerate(source.read_text(errors="replace").splitlines(), 1):
    line = raw.rstrip("\r")
    if line.startswith("CELL_BEGIN "):
        fields = dict(re.findall(r"([A-Za-z_]+)=([^ ]+)", line))
        current = {
            "begin_line": line_no,
            "matrix": fields.get("matrix", "upstream"),
            "arch": fields.get("arch", ""),
            "suite": fields.get("suite", ""),
            "test": fields.get("test", fields.get("mode", "")),
            "repeat": fields.get("repeat", "1"),
            "exit_code": "NOT_OBSERVED",
            "signal": "NOT_OBSERVED",
            "join": "",
            "fatal": "0",
            "terminate": "0",
        }
    elif current is not None:
        if line.startswith("REMOTE_CELL_EXIT_CODE=") or line.startswith("CELL_EXIT_CODE="):
            current["exit_code"] = line.split("=", 1)[1]
        elif line.startswith("REMOTE_CELL_SIGNAL=") or line.startswith("CELL_SIGNAL="):
            current["signal"] = line.split("=", 1)[1]
        if "JOIN_" in line or ":join=" in line:
            current["join"] = line
        if "FATAL: exception not rethrown" in line:
            current["fatal"] = "1"
        if "libc++abi: terminating" in line:
            current["terminate"] = "1"
        if line.startswith("CELL_END "):
            current["end_line"] = line_no
            cells.append(current)
            current = None

target.parent.mkdir(parents=True, exist_ok=True)
columns = ["matrix", "arch", "suite", "test", "repeat", "exit_code", "signal", "join", "fatal", "terminate", "begin_line", "end_line"]
with target.open("w") as stream:
    stream.write("\t".join(columns) + "\n")
    for cell in cells:
        stream.write("\t".join(str(cell.get(column, "")).replace("\t", " ") for column in columns) + "\n")
print(f"INPUT={source}")
print(f"OUTPUT={target}")
print(f"CELL_COUNT={len(cells)}")
