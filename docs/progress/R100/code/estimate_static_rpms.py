#!/usr/bin/env python3
import csv

with open("progress/R98/tables/base_binary_records.tsv", newline="", encoding="utf-8") as handle:
    binaries = {r["checksum"]: r for r in csv.DictReader(handle, delimiter="\t")}
with open("progress/R100/tables/base_filelist_cpp_carriers.tsv", newline="", encoding="utf-8") as handle:
    rows = [r for r in csv.DictReader(handle, delimiter="\t")
            if int(r["static_archive_count"]) > 0]
print(f"rpm_records={len(rows)} bytes={sum(int(binaries[r['rpm_sha256']]['package_size']) for r in rows)}")
for row in sorted(rows, key=lambda r: int(binaries[r["rpm_sha256"]]["package_size"]), reverse=True)[:30]:
    binary = binaries[row["rpm_sha256"]]
    print(binary["package_size"], row["source_name"], row["binary_name"], row["arch"], row["location"], sep="\t")
