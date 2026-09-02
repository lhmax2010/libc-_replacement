#!/usr/bin/env python3
"""Parse the frozen, actual Tizen-Base and Tizen-Unified RPM metadata."""

import argparse
import csv
import gzip
import xml.etree.ElementTree as ET
from collections import defaultdict
from pathlib import Path

COMMON = "{http://linux.duke.edu/metadata/common}"
RPM = "{http://linux.duke.edu/metadata/rpm}"


def text(node, name, default=""):
    child = node.find(COMMON + name)
    return child.text if child is not None and child.text is not None else default


def parse_primary(path):
    rows = []
    with gzip.open(path, "rb") as stream:
        for event, node in ET.iterparse(stream, events=("end",)):
            if node.tag != COMMON + "package":
                continue
            ver = node.find(COMMON + "version")
            loc = node.find(COMMON + "location")
            checksum = node.find(COMMON + "checksum")
            size = node.find(COMMON + "size")
            fmt = node.find(COMMON + "format")
            requires = []
            provides = []
            sourcerpm = ""
            if fmt is not None:
                sr = fmt.find(RPM + "sourcerpm")
                if sr is not None and sr.text:
                    sourcerpm = sr.text
                req = fmt.find(RPM + "requires")
                if req is not None:
                    requires = [entry.get("name", "") for entry in req.findall(RPM + "entry")]
                prov = fmt.find(RPM + "provides")
                if prov is not None:
                    provides = [entry.get("name", "") for entry in prov.findall(RPM + "entry")]
            rows.append({
                "name": text(node, "name"),
                "arch": text(node, "arch"),
                "summary": text(node, "summary").replace("\t", " ").replace("\n", " "),
                "description": text(node, "description").replace("\t", " ").replace("\n", " "),
                "package_size": size.get("package", "") if size is not None else "",
                "epoch": ver.get("epoch", "") if ver is not None else "",
                "version": ver.get("ver", "") if ver is not None else "",
                "release": ver.get("rel", "") if ver is not None else "",
                "vcs": ver.get("vcs", "") if ver is not None else "",
                "checksum": checksum.text if checksum is not None and checksum.text else "",
                "location": loc.get("href", "") if loc is not None else "",
                "sourcerpm": sourcerpm,
                "requires": requires,
                "provides": provides,
            })
            node.clear()
    return rows


def write_tsv(path, fieldnames, rows):
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fieldnames, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def source_name(sourcerpm):
    # Resolve source names using the source metadata outside this helper.
    return sourcerpm


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--inputs", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)

    datasets = {}
    for project in ("base", "unified"):
        datasets[(project, "binary")] = parse_primary(args.inputs / f"{project}_packages_primary.xml.gz")
        datasets[(project, "source")] = parse_primary(args.inputs / f"{project}_source_primary.xml.gz")

    fields = ["project", "kind", "name", "arch", "epoch", "version", "release", "vcs",
              "summary", "description", "package_size",
              "checksum", "location", "sourcerpm", "requires", "provides"]
    for (project, kind), records in datasets.items():
        out_rows = []
        for row in records:
            out_rows.append({
                "project": project,
                "kind": kind,
                **{k: row[k] for k in ("name", "arch", "epoch", "version", "release", "vcs",
                                             "summary", "description", "package_size",
                                             "checksum", "location", "sourcerpm")},
                "requires": ";".join(row["requires"]),
                "provides": ";".join(row["provides"]),
            })
        write_tsv(args.output / f"{project}_{kind}_records.tsv", fields, out_rows)

    base_binary = datasets[("base", "binary")]
    cpp_rows = []
    cpp_sources = defaultdict(lambda: {"binary_names": set(), "arches": set(), "signals": set()})
    for row in base_binary:
        signals = sorted({req for req in row["requires"]
                          if req.startswith("libstdc++.so.6") or req.startswith("libc++.so.1")
                          or req.startswith("libc++abi.so.1")})
        if not signals:
            continue
        cpp_rows.append({
            "binary_name": row["name"], "arch": row["arch"], "version": row["version"],
            "release": row["release"], "sourcerpm": row["sourcerpm"],
            "rpm_sha256": row["checksum"], "location": row["location"],
            "runtime_dependency_signals": ";".join(signals),
        })
        item = cpp_sources[row["sourcerpm"]]
        item["binary_names"].add(row["name"])
        item["arches"].add(row["arch"])
        item["signals"].update(signals)

    cpp_binary_fields = ["binary_name", "arch", "version", "release", "sourcerpm",
                         "rpm_sha256", "location", "runtime_dependency_signals"]
    write_tsv(args.output / "base_cpp_binary_records.tsv", cpp_binary_fields,
              sorted(cpp_rows, key=lambda r: (r["sourcerpm"], r["binary_name"], r["arch"])))
    source_names = {}
    for row in datasets[("base", "source")]:
        source_names[row["location"]] = row["name"]
    cpp_source_rows = []
    for srpm, item in sorted(cpp_sources.items()):
        cpp_source_rows.append({
            "sourcerpm": srpm,
            "source_name": source_names.get(srpm, "NOT_AVAILABLE"),
            "binary_packages": ";".join(sorted(item["binary_names"])),
            "arches": ";".join(sorted(item["arches"])),
            "runtime_dependency_signals": ";".join(sorted(item["signals"])),
        })
    write_tsv(args.output / "base_cpp_source_records.tsv",
              ["sourcerpm", "source_name", "binary_packages", "arches", "runtime_dependency_signals"],
              cpp_source_rows)

    # Package-level Unified -> Base edges, matched through exact RPM provides/requires per architecture.
    base_providers = defaultdict(list)
    for provider in base_binary:
        for provide in provider["provides"]:
            base_providers[(provider["arch"], provide)].append(provider)
    edges = []
    for consumer in datasets[("unified", "binary")]:
        for requirement in consumer["requires"]:
            for provider in base_providers.get((consumer["arch"], requirement), []):
                if consumer["name"] == provider["name"] and consumer["checksum"] == provider["checksum"]:
                    continue
                edges.append({
                    "arch": consumer["arch"],
                    "consumer_sourcerpm": consumer["sourcerpm"],
                    "consumer_binary": consumer["name"],
                    "consumer_rpm_sha256": consumer["checksum"],
                    "requirement": requirement,
                    "provider_sourcerpm": provider["sourcerpm"],
                    "provider_binary": provider["name"],
                    "provider_rpm_sha256": provider["checksum"],
                    "provider_location": provider["location"],
                })
    edge_fields = ["arch", "consumer_sourcerpm", "consumer_binary", "consumer_rpm_sha256",
                   "requirement", "provider_sourcerpm", "provider_binary", "provider_rpm_sha256",
                   "provider_location"]
    unique = {tuple(row[k] for k in edge_fields): row for row in edges}
    write_tsv(args.output / "unified_to_base_package_edges.tsv", edge_fields,
              [unique[key] for key in sorted(unique)])

    cpp_binary_checksums = {row["rpm_sha256"] for row in cpp_rows}
    shared_cpp_edges = [row for row in unique.values()
                        if ".so" in row["requirement"]
                        and row["provider_rpm_sha256"] in cpp_binary_checksums]
    write_tsv(args.output / "unified_to_base_cpp_library_edges.tsv", edge_fields,
              sorted(shared_cpp_edges, key=lambda row: tuple(row[k] for k in edge_fields)))

    print(f"base_binary_records={len(datasets[('base', 'binary')])}")
    print(f"base_binary_names={len({r['name'] for r in datasets[('base', 'binary')]})}")
    print(f"base_source_records={len(datasets[('base', 'source')])}")
    print(f"base_source_names={len({r['name'] for r in datasets[('base', 'source')]})}")
    print(f"base_cpp_binary_records_by_runtime_dependency={len(cpp_rows)}")
    print(f"base_cpp_binary_names_by_runtime_dependency={len({r['binary_name'] for r in cpp_rows})}")
    print(f"base_cpp_source_rpm_records_by_runtime_dependency={len(cpp_sources)}")
    print(f"unified_binary_records={len(datasets[('unified', 'binary')])}")
    print(f"unified_source_records={len(datasets[('unified', 'source')])}")
    print(f"unified_to_base_package_edges={len(unique)}")
    print(f"unified_to_base_cpp_library_dependency_records={len(shared_cpp_edges)}")


if __name__ == "__main__":
    main()
