#!/usr/bin/env python3
"""Hard-link exact R98 downloads into the bounded R100 cache when possible."""

import csv
import os
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def rows(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


old = {
    row["checksum"]: row
    for row in rows("progress/R98/tables/symbol_scan_download_status.tsv")
    if row["result"] == "PASS"
}
count = 0
for row in rows("progress/R100/tables/armv7l_cross_rpm_manifest.tsv"):
    prior = old.get(row["checksum"])
    if not prior:
        continue
    source = Path(prior["target"])
    target = Path("progress/R100/downloads/armv7l_cross") / row["repo_id"] / Path(row["location"]).name
    target.parent.mkdir(parents=True, exist_ok=True)
    if not target.exists():
        os.link(source, target)
    print(f"LINKED\t{row['checksum']}\t{source}\t{target}")
    count += 1
print(f"linked={count}")
