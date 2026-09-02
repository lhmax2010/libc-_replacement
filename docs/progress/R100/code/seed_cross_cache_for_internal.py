#!/usr/bin/env python3
"""Hard-link verified cross-scan RPMs into the internal-edge cache."""

import csv
import os
from pathlib import Path


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


cross = {row["checksum"]: row for row in read("progress/R100/tables/armv7l_cross_download_status.tsv") if row["result"] == "PASS"}
count = 0
for row in read("progress/R100/tables/armv7l_internal_rpm_manifest.tsv"):
    prior = cross.get(row["checksum"])
    if not prior:
        continue
    source = Path(prior["target"])
    target = Path("progress/R100/downloads/armv7l_internal") / "base" / Path(row["location"]).name
    target.parent.mkdir(parents=True, exist_ok=True)
    if not target.exists():
        os.link(source, target)
    print(f"LINKED\t{row['checksum']}\t{source}\t{target}")
    count += 1
print(f"linked={count}")
