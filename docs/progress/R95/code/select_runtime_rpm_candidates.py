#!/usr/bin/env python3
"""Select RPMs that can contain runtime ELF or static archives from filelists metadata."""

import argparse
import csv
import gzip
import re
import xml.etree.ElementTree as ET
from collections import defaultdict
from pathlib import Path

FILELISTS = "{http://linux.duke.edu/metadata/filelists}"
RUNTIME_PREFIX = re.compile(r"^/(?:bin|sbin|usr/bin|usr/sbin|usr/libexec|opt/usr/bin|opt/usr/sbin)/")
LIB_BINARY = re.compile(r"^/(?:lib|lib64|usr/lib|usr/lib64|opt/usr/lib|opt/usr/lib64)/.*(?:\.so(?:\..*)?|\.a)$")


def read_tsv(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path, fields, rows):
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--records", type=Path, required=True)
    ap.add_argument("--filelists", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()
    records = {row["checksum"]: row for row in read_tsv(args.records)}
    paths = defaultdict(list)
    reasons = defaultdict(set)
    with gzip.open(args.filelists, "rb") as stream:
        for event, node in ET.iterparse(stream, events=("end",)):
            if node.tag != FILELISTS + "package":
                continue
            pkgid = node.get("pkgid", "")
            for file_node in node.findall(FILELISTS + "file"):
                path = file_node.text or ""
                if RUNTIME_PREFIX.match(path):
                    paths[pkgid].append(path)
                    reasons[pkgid].add("EXECUTABLE_PATH")
                elif LIB_BINARY.match(path):
                    paths[pkgid].append(path)
                    reasons[pkgid].add("LIBRARY_OR_ARCHIVE_PATH")
            node.clear()
    output = []
    for pkgid in sorted(paths):
        if pkgid not in records:
            raise KeyError(f"filelists pkgid absent from primary metadata: {pkgid}")
        row = records[pkgid]
        output.append({
            "name": row["name"], "arch": row["arch"], "version": row["version"],
            "release": row["release"], "sourcerpm": row["sourcerpm"],
            "rpm_sha256": pkgid, "package_size": row["package_size"], "location": row["location"],
            "selection_reasons": ";".join(sorted(reasons[pkgid])),
            "candidate_paths": ";".join(sorted(set(paths[pkgid]))),
        })
    fields = ["name", "arch", "version", "release", "sourcerpm", "rpm_sha256", "package_size",
              "location", "selection_reasons", "candidate_paths"]
    write_tsv(args.output, fields, output)
    print(f"candidate_rpm_records={len(output)}")
    print(f"candidate_binary_names={len({r['name'] for r in output})}")
    print(f"candidate_source_rpms={len({r['sourcerpm'] for r in output})}")
    print(f"candidate_bytes={sum(int(r['package_size']) for r in output)}")
    print(f"candidate_gib={sum(int(r['package_size']) for r in output)/1024/1024/1024:.3f}")


if __name__ == "__main__":
    main()
