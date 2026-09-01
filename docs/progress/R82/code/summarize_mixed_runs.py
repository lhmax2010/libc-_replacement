#!/usr/bin/env python3
import csv
import re
from collections import Counter
from pathlib import Path

sources = {
    "x86_64": Path("progress/R82/raw/042_run_x86_three_cases.stdout"),
    "armv7l-qemu-user": Path("progress/R82/raw/034_run_arm_local_matrix.stdout"),
}
rows = []
for arch, path in sources.items():
    text = path.read_bytes().decode("utf-8", errors="replace")
    current = None
    records = {}
    for line in text.splitlines():
        match = re.match(r"CASE=(\S+) ITERATION=(\d+)", line)
        if match:
            current = (match.group(1), int(match.group(2)))
            records[current] = {
                "arch": arch, "case": current[0], "iteration": current[1],
                "status_ok": "NOT_OBSERVED", "value_ok": "NOT_OBSERVED",
                "destruction_completed": "NO", "exit_code": "NOT_OBSERVED",
            }
            continue
        if current is None:
            continue
        if line.startswith("sizeof_path="):
            for key in ("status_ok", "value_ok", "returned_size"):
                field = re.search(rf"(?:^| ){key}=([^ ]+)", line)
                if field:
                    records[current][key] = field.group(1)
        elif line.startswith("sizeof_params="):
            for key in ("open_ok", "size_ok", "value_ok", "observed_size"):
                field = re.search(rf"(?:^| ){key}=([^ ]+)", line)
                if field:
                    records[current][key] = field.group(1)
        elif line.startswith("sizeof_variables_map="):
            field = re.search(r"(?:^| )initial_size=([^ ]+)", line)
            if field:
                records[current]["initial_size"] = field.group(1)
        elif line.startswith("after_size="):
            for key in ("after_size", "value_ok"):
                field = re.search(rf"(?:^| ){key}=([^ ]+)", line)
                if field:
                    records[current][key] = field.group(1)
        elif line.startswith("destruction_completed="):
            records[current]["destruction_completed"] = "YES" if line.split()[0].endswith("=1") else "NO"
        elif line.startswith("EXIT="):
            records[current]["exit_code"] = line.split("=", 1)[1]
    rows.extend(records.values())

fields = [
    "arch", "case", "iteration", "status_ok", "open_ok", "size_ok",
    "value_ok", "returned_size", "observed_size", "destruction_completed",
    "initial_size", "after_size", "exit_code",
]
target = Path("progress/R82/tables/mixed_runtime_matrix.tsv")
with target.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
    writer.writeheader()
    for row in rows:
        writer.writerow({field: row.get(field, "NOT_OBSERVED") for field in fields})

for key, count in sorted(Counter((row["arch"], row["case"], row["exit_code"]) for row in rows).items()):
    print("arch=%s case=%s exit=%s count=%d" % (*key, count))
print("total_runs=" + str(len(rows)))
print("filesystem_value_failures=" + str(sum(row["case"] == "filesystem" and row["value_ok"] == "0" for row in rows)))
print("filesystem_destruction_completed=" + str(sum(row["case"] == "filesystem" and row["destruction_completed"] == "YES" for row in rows)))
print("program_options_value_failures=" + str(sum(row["case"] == "program_options" and row["value_ok"] == "0" for row in rows)))
print("program_options_destruction_completed=" + str(sum(row["case"] == "program_options" and row["destruction_completed"] == "YES" for row in rows)))
