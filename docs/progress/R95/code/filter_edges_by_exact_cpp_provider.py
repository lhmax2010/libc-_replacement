#!/usr/bin/env python3
"""Filter Unified->Base package edges using the exact Base ELF C++ scan."""

import argparse
import csv
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)
TOOLCHAIN_SOURCES = {
    "gcc", "gcc-aarch64", "gcc-armv7l", "gcc-riscv64", "llvm", "compiler-rt",
    "binutils", "binutils-aarch64", "binutils-armv7l", "binutils-riscv64",
}


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--edges", type=Path, required=True)
    ap.add_argument("--cpp-binaries", type=Path, required=True)
    ap.add_argument("--output-all", type=Path, required=True)
    ap.add_argument("--output-non-toolchain", type=Path, required=True)
    args = ap.parse_args()
    cpp = read(args.cpp_binaries)
    source_by_sha = {r["rpm_sha256"]: r["source_name"] for r in cpp}
    fields = list(read(args.edges)[0]) + ["provider_source_name", "provider_scope"]
    rows = []
    for row in read(args.edges):
        if ".so" not in row["requirement"] or row["provider_rpm_sha256"] not in source_by_sha:
            continue
        source_name = source_by_sha[row["provider_rpm_sha256"]]
        scope = "TOOLCHAIN_RUNTIME_OR_TOOL" if source_name in TOOLCHAIN_SOURCES else "NON_TOOLCHAIN_BASE_CPP_PROVIDER"
        rows.append({**row, "provider_source_name": source_name, "provider_scope": scope})
    key = lambda r: tuple(r.get(field, "") for field in fields)
    rows = sorted({key(r): r for r in rows}.values(), key=key)
    non = [r for r in rows if r["provider_scope"] == "NON_TOOLCHAIN_BASE_CPP_PROVIDER"]
    write(args.output_all, fields, rows)
    write(args.output_non_toolchain, fields, non)
    print(f"all_exact_cpp_provider_dependency_records={len(rows)}")
    print(f"non_toolchain_cpp_provider_dependency_records={len(non)}")
    print(f"all_provider_rpms={len({r['provider_rpm_sha256'] for r in rows})}")
    print(f"non_toolchain_provider_rpms={len({r['provider_rpm_sha256'] for r in non})}")
    print(f"non_toolchain_consumer_rpms={len({r['consumer_rpm_sha256'] for r in non})}")
    print(f"non_toolchain_consumer_source_rpms={len({r['consumer_sourcerpm'] for r in non})}")


if __name__ == "__main__":
    main()
