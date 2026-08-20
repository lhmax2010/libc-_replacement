#!/usr/bin/env python3
import re
import sys
from pathlib import Path

if len(sys.argv) != 3:
    raise SystemExit("usage: compare_regression_sequences.py INPUT OUTPUT")

source = Path(sys.argv[1])
target = Path(sys.argv[2])
cells = {}
current = None
payload = []

def keep(line):
    prefixes = (
        "EXECUTION_MODE=", "RUN_COMMAND=", "SDB_COMMAND=", "CELL_EXIT_CODE=",
        "CELL_SIGNAL=", "REMOTE_CELL_EXIT_CODE=", "REMOTE_CELL_SIGNAL=",
        "SDB_CARRIER_EXIT_CODE=", "R40_", "R41_", "R42_",
    )
    if line.startswith(prefixes):
        return False
    if line.startswith("/bin/sh: line ") and "Aborted" in line:
        return False
    return bool(line)

for raw in source.read_text(errors="replace").splitlines():
    line = raw.rstrip("\r")
    if line.startswith("CELL_BEGIN matrix=regression "):
        fields = dict(re.findall(r"([A-Za-z_]+)=([^ ]+)", line))
        current = (fields["arch"], fields["suite"], fields["test"])
        payload = []
    elif current is not None and line.startswith("CELL_END matrix=regression "):
        cells[current] = tuple(payload)
        current = None
    elif current is not None and keep(line):
        payload.append(line)

arch = next(iter(cells))[0] if cells else "NOT_OBSERVED"
tests = sorted({key[2] for key in cells if key[1] == "product"})
with target.open("w") as stream:
    stream.write("arch\ttest\tproduct_vs_prestrip_sequence\tproduct_vs_libstdcxx_sequence\tproduct_sequence\n")
    for test in tests:
        product = cells.get((arch, "product", test), ())
        prestrip = cells.get((arch, "prestrip", test), ())
        control = cells.get((arch, "libstdcxx_control", test), ())
        stream.write("\t".join((
            arch,
            test,
            "MATCH" if product == prestrip else "DIFFER",
            "MATCH" if product == control else "DIFFER",
            " | ".join(product).replace("\t", " "),
        )) + "\n")

print(f"INPUT={source}")
print(f"OUTPUT={target}")
print(f"ROW_COUNT={len(tests)}")
