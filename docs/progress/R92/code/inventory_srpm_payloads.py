#!/usr/bin/env python3
import csv
import subprocess
from collections import Counter
from pathlib import Path

manifest = list(csv.DictReader(open("progress/R92/tables/cpp_gap_140.tsv", newline=""), delimiter="\t"))
rows = []
suffixes = Counter()
failures = 0
for item in manifest:
    proc = subprocess.run(["rpm", "-qpl", item["local_path"]], text=True,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if proc.returncode:
        failures += 1
    for name in proc.stdout.splitlines():
        lower = name.lower()
        suffix = "OTHER"
        for candidate in (".tar.zst", ".tar.lz", ".tar.lzma", ".tar.gz", ".tar.bz2", ".tar.xz", ".tgz", ".tbz2", ".txz", ".zip", ".crate", ".tar"):
            if lower.endswith(candidate):
                suffix = candidate
                break
        suffixes[suffix] += 1
        rows.append({"source_rpm": item["source_rpm"], "payload_path": name, "archive_suffix": suffix})
with open("progress/R92/tables/srpm_payload_inventory.tsv", "w", newline="") as stream:
    writer = csv.DictWriter(stream, fieldnames=("source_rpm", "payload_path", "archive_suffix"), delimiter="\t", lineterminator="\n")
    writer.writeheader(); writer.writerows(rows)
print(f"source_rpms={len(manifest)}")
print(f"rpm_query_failures={failures}")
for suffix, count in sorted(suffixes.items()):
    print(f"suffix={suffix} count={count}")
raise SystemExit(0 if not failures else 2)
