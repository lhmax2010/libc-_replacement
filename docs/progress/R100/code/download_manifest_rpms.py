#!/usr/bin/env python3
"""Download an exact RPM manifest sequentially and verify every SHA256."""

import csv
import hashlib
import shlex
import subprocess
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)
manifest = Path(sys.argv[1])
target_root = Path(sys.argv[2])
status_path = Path(sys.argv[3])
base_urls = {
    "base": "https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/repos/standard/packages/",
    "unified": "https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247/repos/standard/packages/",
}


def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            value.update(block)
    return value.hexdigest()


with manifest.open(newline="", encoding="utf-8") as stream:
    rows = list(csv.DictReader(stream, delimiter="\t"))

results = []
target_root.mkdir(parents=True, exist_ok=True)
for index, row in enumerate(rows, 1):
    target = target_root / row["repo_id"] / Path(row["location"]).name
    target.parent.mkdir(parents=True, exist_ok=True)
    url = base_urls[row["repo_id"]] + row["location"]
    command = [
        "curl", "--fail", "--location", "--retry", "2",
        "--connect-timeout", "15", "--max-time", "120",
        "--output", str(target), url,
    ]
    print(f"LABEL=download_{index:03d}")
    print(f"COMMAND={shlex.join(command)}")
    if target.exists() and digest(target) == row["checksum"]:
        rc = 0
        print("REUSED_VERIFIED_FILE=YES")
    else:
        run = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        rc = run.returncode
        if run.stdout:
            print("STDOUT=" + run.stdout.replace("\n", "\\n"))
        if run.stderr:
            print("STDERR=" + run.stderr.replace("\n", "\\n"))
    actual = digest(target) if target.exists() else "NOT_AVAILABLE"
    result = "PASS" if rc == 0 and actual == row["checksum"] else "FAIL"
    print(f"EXIT_CODE={rc}")
    print(f"EXPECTED_SHA256={row['checksum']}")
    print(f"ACTUAL_SHA256={actual}")
    print(f"RESULT={result}\n")
    results.append({**row, "target": str(target), "actual_sha256": actual,
                    "exit_code": str(rc), "result": result, "url": url})
    if result != "PASS":
        break

fields = list(rows[0]) + ["target", "actual_sha256", "exit_code", "result", "url"]
with status_path.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(results)
print(f"completed={len(results)}/{len(rows)}")
print(f"pass={sum(row['result'] == 'PASS' for row in results)}")
if len(results) != len(rows) or any(row["result"] != "PASS" for row in results):
    raise SystemExit(1)
