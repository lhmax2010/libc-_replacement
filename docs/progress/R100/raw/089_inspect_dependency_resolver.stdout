#!/usr/bin/env python3
"""Build source/build and binary/runtime dependency topology around GCC-selected sources."""

import csv
import sys
from collections import defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path("progress/R98")
TABLES = ROOT / "tables"
GCC_SOURCES = {
    ("base", "gcc"), ("base", "gcc-aarch64"), ("base", "gcc-armv7hl"),
    ("base", "gcc-armv7l"), ("base", "gcc-riscv64"), ("base", "glibc"),
    ("unified", "lapack"), ("unified", "nntrainer"),
    ("unified", "onnxruntime"), ("unified", "openblas"), ("unified", "yaca"),
}


def read(name):
    with (TABLES / name).open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(name, fields, rows):
    with (TABLES / name).open("w", encoding="utf-8", newline="") as stream:
        out = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        out.writeheader()
        out.writerows(rows)


sources = {project: read(f"{project}_source_records.tsv") for project in ("base", "unified")}
binaries = {project: read(f"{project}_binary_records.tsv") for project in ("base", "unified")}
source_by_srpm = {
    project: {row["location"]: row["name"] for row in rows}
    for project, rows in sources.items()
}

for project, rows in binaries.items():
    for row in rows:
        row["source_name"] = source_by_srpm[project].get(row["sourcerpm"], "NOT_AVAILABLE")

# Image membership uses exact binary name, architecture and version-release.
image_membership = defaultdict(set)
for manifest in sorted((ROOT / "inputs/image_packages").glob("*.packages")):
    image = manifest.stem
    with manifest.open(encoding="utf-8") as stream:
        for line in stream:
            fields = line.rstrip("\n").split(" ", 2)
            if len(fields) < 2 or "." not in fields[0]:
                continue
            name, arch = fields[0].rsplit(".", 1)
            image_membership[(name, arch, fields[1])].add(image)

# Compiler evidence aggregated per source package.
compiler_rows = read("buildlog_compiler_evidence.tsv")
compiler_by_source = defaultdict(list)
for row in compiler_rows:
    compiler_by_source[(row["project"], row["source_package"])].append(row)

summary_rows = []
gcc_binary_checksums = set()
gcc_binary_keys = set()
for project, source_name in sorted(GCC_SOURCES):
    subset = [row for row in binaries[project] if row["source_name"] == source_name]
    gcc_binary_checksums.update(row["checksum"] for row in subset)
    gcc_binary_keys.update((project, row["checksum"]) for row in subset)
    runtime_cpp = [row for row in subset if any(
        req.startswith(("libstdc++.so", "libc++.so", "libc++abi.so"))
        for req in row["requires"].split(";") if req)]
    images = set()
    for row in subset:
        images.update(image_membership.get((row["name"], row["arch"], f"{row['version']}-{row['release']}"), set()))
    evidence = compiler_by_source[(project, source_name)]
    summary_rows.append({
        "project": project,
        "source_package": source_name,
        "compiler_statuses": ";".join(sorted({r["compiler_selection"] for r in evidence})),
        "compiler_arches": ";".join(sorted({r["arch"] for r in evidence if r["compiler_selection"] == "GCC"})),
        "binary_packages": ";".join(sorted({r["name"] for r in subset})),
        "binary_records": str(len(subset)),
        "cpp_runtime_binary_packages": ";".join(sorted({r["name"] for r in runtime_cpp})),
        "direct_runtime_signals": ";".join(sorted({req for r in runtime_cpp for req in r["requires"].split(";")
                                                      if req.startswith(("libstdc++.so", "libc++.so", "libc++abi.so"))})),
        "image_membership": ";".join(sorted(images)) or "NONE_IN_SIX_IMAGES",
    })
write("gcc_source_package_summary.tsv", list(summary_rows[0]), summary_rows)

# Capability indexes. Unified can see its own repository plus Base; Base sees Base only.
providers_by_arch = defaultdict(list)
providers_by_name = defaultdict(list)
for project, rows in binaries.items():
    for row in rows:
        for capability in filter(None, row["provides"].split(";")):
            providers_by_arch[(row["arch"], capability)].append((project, row))
            providers_by_name[capability].append((project, row))


def visible(consumer_project, provider_project):
    return provider_project == "base" or consumer_project == "unified"


# Build-time source dependency records, aggregated across duplicate provider arches.
build = {}
for consumer_project, rows in sources.items():
    for consumer in rows:
        for requirement in filter(None, consumer["requires"].split(";")):
            for provider_project, provider in providers_by_name.get(requirement, []):
                provider_key = (provider_project, provider["source_name"])
                if provider_key not in GCC_SOURCES or not visible(consumer_project, provider_project):
                    continue
                if (consumer_project, consumer["name"]) == provider_key:
                    continue
                key = (consumer_project, consumer["name"], provider_project,
                       provider["source_name"], requirement)
                item = build.setdefault(key, {
                    "consumer_project": consumer_project, "consumer_source": consumer["name"],
                    "provider_project": provider_project, "gcc_provider_source": provider["source_name"],
                    "requirement": requirement, "provider_binaries": set(), "provider_arches": set(),
                })
                item["provider_binaries"].add(provider["name"])
                item["provider_arches"].add(provider["arch"])
build_rows = []
for key in sorted(build):
    item = build[key]
    build_rows.append({**{k: item[k] for k in ("consumer_project", "consumer_source", "provider_project",
                                                "gcc_provider_source", "requirement")},
                       "provider_binaries": ";".join(sorted(item["provider_binaries"])),
                       "provider_arches": ";".join(sorted(item["provider_arches"]))})
write("gcc_inbound_build_dependencies.tsv", list(build_rows[0]) if build_rows else [
    "consumer_project", "consumer_source", "provider_project", "gcc_provider_source",
    "requirement", "provider_binaries", "provider_arches"], build_rows)

# Runtime binary edges in both directions. Keep shared-library requirements separate from package capabilities.
runtime = {}
all_binaries = [(project, row) for project, rows in binaries.items() for row in rows]
for consumer_project, consumer in all_binaries:
    consumer_is_gcc = (consumer_project, consumer["source_name"]) in GCC_SOURCES
    for requirement in filter(None, consumer["requires"].split(";")):
        for provider_project, provider in providers_by_arch.get((consumer["arch"], requirement), []):
            if not visible(consumer_project, provider_project):
                continue
            provider_is_gcc = (provider_project, provider["source_name"]) in GCC_SOURCES
            if not (consumer_is_gcc or provider_is_gcc) or (consumer_is_gcc and provider_is_gcc):
                continue
            if consumer["checksum"] == provider["checksum"]:
                continue
            direction = "GCC_OUTBOUND" if consumer_is_gcc else "GCC_INBOUND"
            key = (direction, consumer_project, consumer["checksum"], provider_project,
                   provider["checksum"], requirement)
            runtime[key] = {
                "direction": direction, "arch": consumer["arch"],
                "consumer_project": consumer_project, "consumer_source": consumer["source_name"],
                "consumer_binary": consumer["name"], "consumer_checksum": consumer["checksum"],
                "consumer_location": consumer["location"], "requirement": requirement,
                "provider_project": provider_project, "provider_source": provider["source_name"],
                "provider_binary": provider["name"], "provider_checksum": provider["checksum"],
                "provider_location": provider["location"],
                "shared_library_requirement": "YES" if ".so" in requirement else "NO",
            }
runtime_rows = [runtime[key] for key in sorted(runtime)]
write("gcc_runtime_dependency_edges.tsv", list(runtime_rows[0]) if runtime_rows else [
    "direction", "arch", "consumer_project", "consumer_source", "consumer_binary",
    "consumer_checksum", "consumer_location", "requirement", "provider_project",
    "provider_source", "provider_binary", "provider_checksum", "provider_location",
    "shared_library_requirement"], runtime_rows)

print(f"gcc_source_packages={len(GCC_SOURCES)}")
print(f"gcc_source_packages_with_cpp_runtime_elf={sum(bool(r['cpp_runtime_binary_packages']) for r in summary_rows)}")
print("gcc_cpp_sources=" + ";".join(r["source_package"] for r in summary_rows if r["cpp_runtime_binary_packages"]))
print(f"gcc_sources_in_six_images={sum(r['image_membership'] != 'NONE_IN_SIX_IMAGES' for r in summary_rows)}")
print("gcc_image_sources=" + ";".join(r["source_package"] for r in summary_rows if r["image_membership"] != "NONE_IN_SIX_IMAGES"))
print(f"inbound_build_dependency_records={len(build_rows)}")
print(f"inbound_build_consumer_sources={len({(r['consumer_project'], r['consumer_source']) for r in build_rows})}")
print(f"runtime_edges={len(runtime_rows)}")
for direction in ("GCC_INBOUND", "GCC_OUTBOUND"):
    subset = [r for r in runtime_rows if r["direction"] == direction]
    print(f"{direction}_records={len(subset)}")
    print(f"{direction}_shared_library_records={sum(r['shared_library_requirement'] == 'YES' for r in subset)}")
    print(f"{direction}_source_pairs={len({(r['consumer_source'], r['provider_source']) for r in subset})}")
