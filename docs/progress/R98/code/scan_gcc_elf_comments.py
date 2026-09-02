#!/usr/bin/env python3
"""Collect .comment compiler markers from GCC-selected package ELF artifacts."""

import csv
import re
import subprocess
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)
TABLES = Path("progress/R98/tables")
EXTRACT = Path("/tmp/r98_symbol_edges_extract_20260902")
GCC = {"gcc", "gcc-aarch64", "gcc-armv7hl", "gcc-armv7l", "gcc-riscv64",
       "glibc", "lapack", "nntrainer", "onnxruntime", "openblas", "yaca"}


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


srpm_to_name = {}
for project in ("base", "unified"):
    for row in read(TABLES / f"{project}_source_records.tsv"):
        srpm_to_name[row["location"]] = row["name"]

rows = []
commands = []
for elf in read(Path("progress/R98/symbol_scan/elf_inventory.tsv")):
    source = srpm_to_name.get(elf["sourcerpm"], "NOT_AVAILABLE")
    if source not in GCC:
        continue
    path = EXTRACT / elf["rpm_sha256"][:2] / elf["rpm_sha256"] / elf["path"].lstrip("/")
    command = ["readelf", "-p", ".comment", "-W", str(path)]
    run = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    strings = []
    for line in run.stdout.splitlines():
        match = re.match(r"\s*\[\s*[0-9a-fA-F]+\]\s+(.+)$", line)
        if match:
            strings.append(match.group(1))
    markers = sorted(set(strings))
    kinds = []
    if any("GCC:" in marker or "GNU" in marker for marker in markers):
        kinds.append("GCC_MARKER")
    if any("clang version" in marker.lower() for marker in markers):
        kinds.append("CLANG_MARKER")
    if not kinds:
        kinds.append("NO_COMPILER_MARKER")
    rows.append({
        "source_package": source, "binary_package": elf["name"], "arch": elf["arch"],
        "elf_path": elf["path"], "runtime_elf": elf["runtime_elf"],
        "marker_classes": ";".join(kinds), "comment_strings": ";".join(markers) or "NOT_OBSERVED",
        "readelf_exit_code": str(run.returncode),
        "readelf_stderr": run.stderr.replace("\t", " ").replace("\n", "\\n") or "NONE",
    })
    commands.append((command, run.returncode, run.stderr))

fields = list(rows[0])
with (TABLES / "gcc_elf_comment_evidence.tsv").open("w", encoding="utf-8", newline="") as stream:
    out = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    out.writeheader()
    out.writerows(rows)
with Path("progress/R98/raw/050_individual_comment_commands.log").open("w", encoding="utf-8") as stream:
    for index, (command, rc, stderr) in enumerate(commands, 1):
        stream.write(f"LABEL=comment_probe_{index:05d}\n")
        stream.write("COMMAND=" + " ".join(command) + "\n")
        if stderr:
            stream.write("STDERR=" + stderr.replace("\n", "\\n") + "\n")
        stream.write(f"EXIT_CODE={rc}\n\n")
    stream.write(f"TOTAL={len(commands)}\n")

print(f"elf_records={len(rows)}")
for source in sorted(GCC):
    subset = [row for row in rows if row["source_package"] == source]
    print(f"{source}_elf_records={len(subset)}")
    print(f"{source}_marker_classes=" + ";".join(sorted({x for row in subset for x in row["marker_classes"].split(";")})))
print(f"producer_information=NOT_OBSERVED (selected binary RPMs contain no separate debuginfo inputs)")
