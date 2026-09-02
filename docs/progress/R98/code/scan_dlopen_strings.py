#!/usr/bin/env python3
"""Collect non-DT_NEEDED shared-library strings from GCC-selected dlopen users."""

import csv
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
EXTRACT = Path("/tmp/r98_symbol_edges_extract_20260902")
TABLES = Path("progress/R98/tables")
GCC = {"gcc", "gcc-aarch64", "gcc-armv7hl", "gcc-armv7l", "gcc-riscv64",
       "glibc", "lapack", "nntrainer", "onnxruntime", "openblas", "yaca"}
SONAME = re.compile(r"(?<![A-Za-z0-9_+.-])([A-Za-z0-9_+./-]*lib[A-Za-z0-9_+.-]+\.so(?:\.[0-9]+)*)")


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


source = {}
for project in ("base", "unified"):
    for row in read(TABLES / f"{project}_source_records.tsv"):
        source[row["location"]] = row["name"]
needed = defaultdict(set)
for row in read(Path("progress/R98/symbol_scan/dt_needed_edges.tsv")):
    needed[(row["rpm_sha256"], row["path"])].add(row["soname"])

rows = []
ledger = []
for elf in read(Path("progress/R98/symbol_scan/elf_inventory.tsv")):
    name = source.get(elf["sourcerpm"], "NOT_AVAILABLE")
    if name not in GCC or elf["runtime_elf"] != "YES" or elf["dlopen_und"] != "YES":
        continue
    local = EXTRACT / elf["rpm_sha256"][:2] / elf["rpm_sha256"] / elf["path"].lstrip("/")
    command = ["strings", "-a", str(local)]
    run = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    found = sorted({match.group(1) for line in run.stdout.splitlines() for match in SONAME.finditer(line)})
    dt = needed[(elf["rpm_sha256"], elf["path"])]
    non_needed = sorted(item for item in found if Path(item).name not in dt)
    rows.append({
        "source_package": name, "binary_package": elf["name"], "arch": elf["arch"],
        "elf_path": elf["path"], "dt_needed": ";".join(sorted(dt)) or "NONE",
        "all_shared_library_strings": ";".join(found) or "NOT_OBSERVED",
        "non_dt_needed_shared_library_strings": ";".join(non_needed) or "NOT_OBSERVED",
        "strings_exit_code": str(run.returncode),
    })
    ledger.append((command, run.returncode, run.stderr))

fields = list(rows[0])
with (TABLES / "gcc_dlopen_string_evidence.tsv").open("w", encoding="utf-8", newline="") as stream:
    out = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    out.writeheader()
    out.writerows(rows)
with Path("progress/R98/raw/054_individual_strings_commands.log").open("w", encoding="utf-8") as stream:
    for index, (command, rc, stderr) in enumerate(ledger, 1):
        stream.write(f"LABEL=strings_probe_{index:05d}\nCOMMAND={' '.join(command)}\n")
        if stderr:
            stream.write("STDERR=" + stderr.replace("\n", "\\n") + "\n")
        stream.write(f"EXIT_CODE={rc}\n\n")
    stream.write(f"TOTAL={len(ledger)}\n")

print(f"dlopen_elf_records={len(rows)}")
for row in rows:
    if row["non_dt_needed_shared_library_strings"] != "NOT_OBSERVED":
        print("CANDIDATE\t" + "\t".join(row[k] for k in (
            "source_package", "binary_package", "arch", "elf_path",
            "non_dt_needed_shared_library_strings")))
