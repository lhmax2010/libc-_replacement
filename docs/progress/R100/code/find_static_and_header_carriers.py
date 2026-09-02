#!/usr/bin/env python3
"""Map Base Toolchain static archives and explicit C++ headers to source RPMs."""

import csv
import gzip
import xml.etree.ElementTree as ET
from collections import defaultdict
from pathlib import Path

RECORDS = Path("progress/R98/tables/base_binary_records.tsv")
FILELISTS = Path("progress/R98/inputs/llvm_base_packages_filelists.xml.gz")
OUTPUT = Path("progress/R100/tables/base_filelist_cpp_carriers.tsv")

by_checksum = {}
source_name_by_srpm = {}
with RECORDS.open(newline="", encoding="utf-8") as handle:
    for row in csv.DictReader(handle, delimiter="\t"):
        by_checksum[row["checksum"]] = row
        srpm = row["sourcerpm"]
        # Longest matching source name is reconstructed from exact source table later.
        source_name_by_srpm.setdefault(srpm, "")

source_table = Path("progress/R98/tables/base_source_records.tsv")
with source_table.open(newline="", encoding="utf-8") as handle:
    for row in csv.DictReader(handle, delimiter="\t"):
        exact = f'{row["name"]}-{row["version"]}-{row["release"]}.src.rpm'
        source_name_by_srpm[exact] = row["name"]

observed = defaultdict(lambda: {"archives": [], "headers": []})
with gzip.open(FILELISTS, "rb") as handle:
    for _event, elem in ET.iterparse(handle, events=("end",)):
        if not elem.tag.endswith("package"):
            continue
        checksum = elem.attrib.get("pkgid", "")
        row = by_checksum.get(checksum)
        if row:
            key = (row["sourcerpm"], row["name"], row["arch"], row["checksum"], row["location"])
            for child in elem:
                if not child.tag.endswith("file") or not child.text:
                    continue
                path = child.text
                lower = path.lower()
                if lower.endswith(".a"):
                    observed[key]["archives"].append(path)
                if (lower.endswith((".hpp", ".hxx", ".hh", ".ipp", ".tcc"))
                        or "/include/c++/" in lower):
                    observed[key]["headers"].append(path)
        elem.clear()

OUTPUT.parent.mkdir(parents=True, exist_ok=True)
fields = ["source_name", "sourcerpm", "binary_name", "arch", "rpm_sha256", "location",
          "static_archive_count", "cpp_header_count", "static_archive_examples", "cpp_header_examples"]
with OUTPUT.open("w", newline="", encoding="utf-8") as handle:
    writer = csv.DictWriter(handle, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    for key in sorted(observed):
        srpm, binary, arch, checksum, location = key
        archives = sorted(observed[key]["archives"])
        headers = sorted(observed[key]["headers"])
        if not archives and not headers:
            continue
        writer.writerow({
            "source_name": source_name_by_srpm.get(srpm) or "NOT_AVAILABLE",
            "sourcerpm": srpm,
            "binary_name": binary,
            "arch": arch,
            "rpm_sha256": checksum,
            "location": location,
            "static_archive_count": len(archives),
            "cpp_header_count": len(headers),
            "static_archive_examples": ";".join(archives[:20]) or "NONE",
            "cpp_header_examples": ";".join(headers[:20]) or "NONE",
        })

rows = list(csv.DictReader(OUTPUT.open(newline="", encoding="utf-8"), delimiter="\t"))
archive_sources = {r["source_name"] for r in rows if int(r["static_archive_count"]) > 0}
header_sources = {r["source_name"] for r in rows if int(r["cpp_header_count"]) > 0}
print(f"carrier_rows={len(rows)} static_archive_sources={len(archive_sources)} cpp_header_sources={len(header_sources)}")
