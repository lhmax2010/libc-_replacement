#!/usr/bin/env python3
"""Hard-link already verified R100 RPMs into a new bounded cache."""

import csv
import os
from pathlib import Path


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


verified = {}
for status in (
    "progress/R100/tables/armv7l_cross_download_status.tsv",
    "progress/R100/tables/armv7l_internal_download_status.tsv",
    "progress/R100/tables/static_rpm_download_status.tsv",
):
    for row in read(status):
        if row["result"] == "PASS":
            verified[row["checksum"]] = row["target"]

count = 0
total = 0
for row in read("progress/R100/tables/undetermined_binary_manifest.tsv"):
    total += int(row["package_size"])
    source = verified.get(row["checksum"])
    if not source:
        continue
    target = Path("progress/R100/downloads/undetermined_binaries/base") / Path(row["location"]).name
    target.parent.mkdir(parents=True, exist_ok=True)
    if not target.exists():
        os.link(source, target)
    print(f"LINKED\t{row['checksum']}\t{source}\t{target}")
    count += 1
print(f"linked={count}")
