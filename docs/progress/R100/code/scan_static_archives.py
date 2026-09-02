#!/usr/bin/env python3
"""Extract static archives and classify whether they contain C++ ABI symbols."""

import csv
import hashlib
import os
import shlex
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

csv.field_size_limit(sys.maxsize)
status_path, extract_root, output_path, ledger_path = map(Path, sys.argv[1:5])
jobs = int(sys.argv[5])


def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            value.update(block)
    return value.hexdigest()


def scan(index_row):
    index, row = index_row
    rpm = Path(row["target"]).resolve()
    out = (extract_root / row["checksum"]).resolve()
    out.mkdir(parents=True, exist_ok=True)
    commands = []
    if row["result"] != "PASS" or digest(rpm) != row["checksum"]:
        return index, row, [], commands, "RPM_NOT_VERIFIED"
    extract = f"rpm2cpio {shlex.quote(str(rpm))} | cpio -idm --quiet -- '*.a'"
    run = subprocess.run(["bash", "-o", "pipefail", "-c", extract], cwd=out,
                         stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    commands.append((str(out), extract, run.returncode, run.stderr))
    if run.returncode:
        return index, row, [], commands, "EXTRACT_FAILED"
    archive_rows = []
    for archive in sorted(out.rglob("*.a")):
        # Only externally visible definitions can form an archive/consumer ABI
        # boundary.  This excludes Clang-generated local fortify wrappers.
        command = ["nm", "-g", "-A", "--defined-only", str(archive)]
        probe = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        commands.append((os.getcwd(), shlex.join(command), probe.returncode, probe.stderr))
        candidates = sorted({
            word for line in probe.stdout.splitlines() for word in line.split()
            if word.startswith("_Z")
        })
        mangled = []
        if candidates:
            demangle = subprocess.run(
                ["c++filt"], input="\n".join(candidates) + "\n",
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
            )
            commands.append((os.getcwd(), "c++filt <candidate-symbols>", demangle.returncode,
                             demangle.stderr))
            decoded = demangle.stdout.splitlines()
            mangled = [symbol for symbol, text in zip(candidates, decoded) if text != symbol]
        archive_rows.append({
            "archive_path": "/" + str(archive.relative_to(out)),
            "nm_exit_code": str(probe.returncode),
            "cpp_mangled_symbol": "YES" if mangled else "NO",
            "cpp_symbol_count": str(len(mangled)),
            "cpp_symbol_examples": ";".join(mangled[:20]) or "NONE",
        })
    return index, row, archive_rows, commands, "PASS"


with status_path.open(newline="", encoding="utf-8") as stream:
    input_rows = list(csv.DictReader(stream, delimiter="\t"))
extract_root.mkdir(parents=True, exist_ok=True)
results = []
ledgers = []
with ThreadPoolExecutor(max_workers=jobs) as pool:
    futures = [pool.submit(scan, item) for item in enumerate(input_rows, 1)]
    for complete, future in enumerate(as_completed(futures), 1):
        index, package, archives, commands, result = future.result()
        for archive in archives:
            results.append({
                "source_name": package["sourcerpm"], "binary_name": package["name"],
                "arch": package["arch"], "rpm_sha256": package["checksum"],
                **archive, "scan_result": result,
            })
        if not archives:
            results.append({
                "source_name": package["sourcerpm"], "binary_name": package["name"],
                "arch": package["arch"], "rpm_sha256": package["checksum"],
                "archive_path": "NONE", "nm_exit_code": "NOT_RUN",
                "cpp_mangled_symbol": "NOT_OBSERVED", "cpp_symbol_count": "0",
                "cpp_symbol_examples": "NONE", "scan_result": result,
            })
        ledgers.append((index, commands))
        if complete % 20 == 0 or complete == len(input_rows):
            print(f"PROGRESS={complete}/{len(input_rows)}", flush=True)

fields = ["source_name", "binary_name", "arch", "rpm_sha256", "archive_path",
          "nm_exit_code", "cpp_mangled_symbol", "cpp_symbol_count",
          "cpp_symbol_examples", "scan_result"]
with output_path.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(sorted(results, key=lambda row: tuple(row[key] for key in fields[:5])))
with ledger_path.open("w", encoding="utf-8") as stream:
    for index, commands in sorted(ledgers):
        for sub, (cwd, command, rc, stderr) in enumerate(commands, 1):
            stream.write(f"LABEL=package_{index:03d}_{sub:03d}\nPWD={cwd}\nCOMMAND={command}\n")
            if stderr:
                stream.write("STDERR=" + stderr.replace("\n", "\\n") + "\n")
            stream.write(f"EXIT_CODE={rc}\n\n")
print(f"archive_rows={len(results)}")
print(f"cpp_archive_rows={sum(row['cpp_mangled_symbol'] == 'YES' for row in results)}")
if any(row["scan_result"] != "PASS" for row in results):
    raise SystemExit(1)
