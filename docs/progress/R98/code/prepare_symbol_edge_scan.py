#!/usr/bin/env python3
"""Select device-architecture RPMs for exact symbol intersections around GCC sources."""

import csv
import re
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)
TABLES = Path("progress/R98/tables")
ARCHES = {"armv7l", "aarch64"}
SYSTEM = re.compile(
    r"^(ld-linux|lib(c|m|dl|rt|pthread|gcc_s|stdc\+\+|atomic|gomp|itm|asan|lsan|tsan|ubsan|hwasan|quadmath|gfortran)\.so)"
)


def read(name):
    with (TABLES / name).open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    with path.open("w", encoding="utf-8", newline="") as stream:
        out = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        out.writeheader()
        out.writerows(rows)


edges = []
for row in read("gcc_runtime_dependency_edges.tsv"):
    if row["arch"] not in ARCHES or row["shared_library_requirement"] != "YES":
        continue
    soname = row["requirement"].split("(", 1)[0]
    if SYSTEM.match(soname):
        continue
    edges.append({
        "direction": row["direction"], "arch": row["arch"],
        "consumer_sourcerpm": row["consumer_source"],
        "consumer_binary": row["consumer_binary"],
        "consumer_rpm_sha256": row["consumer_checksum"],
        "consumer_project": row["consumer_project"],
        "consumer_location": row["consumer_location"],
        "requirement": row["requirement"],
        "provider_sourcerpm": row["provider_source"],
        "provider_binary": row["provider_binary"],
        "provider_rpm_sha256": row["provider_checksum"],
        "provider_project": row["provider_project"],
        "provider_location": row["provider_location"],
    })
edge_fields = list(edges[0])
write(TABLES / "gcc_non_system_symbol_edges.tsv", edge_fields, edges)

binary_by_checksum = {}
for project in ("base", "unified"):
    for row in read(f"{project}_binary_records.tsv"):
        binary_by_checksum[(project, row["checksum"])] = row
wanted = set()
for row in edges:
    wanted.add((row["consumer_project"], row["consumer_rpm_sha256"]))
    wanted.add((row["provider_project"], row["provider_rpm_sha256"]))
manifest = []
for project, checksum in sorted(wanted):
    row = binary_by_checksum[(project, checksum)]
    manifest.append({
        "repo_id": project, "kind": "binary", "name": row["name"], "arch": row["arch"],
        "epoch": row["epoch"], "version": row["version"], "release": row["release"],
        "sourcerpm": row["sourcerpm"], "location": row["location"],
        "checksum_type": "sha256", "checksum": row["checksum"],
        "package_size": row["package_size"], "build_time": "NOT_AVAILABLE",
    })
manifest_fields = list(manifest[0])
write(TABLES / "symbol_scan_rpm_manifest.tsv", manifest_fields, manifest)

print(f"selected_edge_records={len(edges)}")
print(f"selected_source_pairs={len({(r['direction'], r['consumer_sourcerpm'], r['provider_sourcerpm']) for r in edges})}")
print(f"selected_rpms={len(manifest)}")
print(f"selected_bytes={sum(int(r['package_size']) for r in manifest)}")
print("selected_sources=" + ";".join(sorted({r["consumer_sourcerpm"] for r in edges} |
                                                  {r["provider_sourcerpm"] for r in edges})))
