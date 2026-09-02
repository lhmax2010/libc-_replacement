#!/usr/bin/env python3
"""Summarize the exact actual-Base runtime ELF scan and image intersection."""

import argparse
import csv
import sys
from collections import defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        writer.writeheader()
        writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--elf", type=Path, nargs="+", required=True)
    ap.add_argument("--needed", type=Path, nargs="+", required=True)
    ap.add_argument("--binary", type=Path, required=True)
    ap.add_argument("--source", type=Path, required=True)
    ap.add_argument("--image-membership", type=Path, required=True)
    ap.add_argument("--output-dir", type=Path, required=True)
    args = ap.parse_args()
    elfs = [row for path in args.elf for row in read(path)]
    needed = [row for path in args.needed for row in read(path)]
    binary = read(args.binary)
    source = read(args.source)
    images = read(args.image_membership)
    binary_by_sha = {row["checksum"]: row for row in binary}
    source_names = {row["location"]: row["name"] for row in source}
    needed_by_elf = defaultdict(set)
    for row in needed:
        needed_by_elf[(row["rpm_sha256"], row["path"])].add(row["soname"])

    runtime = [row for row in elfs if row["runtime_elf"] == "YES"]
    cpp_elf = [row for row in runtime if row["cpp_indicator"] == "YES"]
    cpp_by_rpm = defaultdict(list)
    for row in cpp_elf:
        cpp_by_rpm[row["rpm_sha256"]].append(row)
    rpm_rows = []
    for sha, rows in sorted(cpp_by_rpm.items()):
        meta = binary_by_sha[sha]
        linked = [row for row in rows if any(s.startswith("libstdc++.so") for s in needed_by_elf[(sha, row["path"])])]
        rpm_rows.append({
            "binary_name": meta["name"], "arch": meta["arch"], "version": meta["version"],
            "release": meta["release"], "sourcerpm": meta["sourcerpm"],
            "source_name": source_names.get(meta["sourcerpm"], "NOT_AVAILABLE"),
            "rpm_sha256": sha, "cpp_runtime_elf_count": str(len(rows)),
            "itanium_symbol_elf_count": str(sum(r["itanium_mangled_symbol"] == "YES" for r in rows)),
            "cpp_runtime_needed_elf_count": str(sum(r["cpp_runtime_needed"] == "YES" for r in rows)),
            "libstdcpp_needed_elf_count": str(len(linked)),
            "cpp_runtime_elf_paths": ";".join(sorted(r["path"] for r in rows)),
        })
    rpm_fields = ["binary_name", "arch", "version", "release", "sourcerpm", "source_name", "rpm_sha256",
                  "cpp_runtime_elf_count", "itanium_symbol_elf_count", "cpp_runtime_needed_elf_count",
                  "libstdcpp_needed_elf_count", "cpp_runtime_elf_paths"]
    write(args.output_dir / "base_cpp_binary_records_exact.tsv", rpm_fields,
          sorted(rpm_rows, key=lambda r: (r["source_name"], r["sourcerpm"], r["binary_name"], r["arch"])))
    write(args.output_dir / "base_cpp_runtime_elfs_exact.tsv", list(cpp_elf[0]),
          sorted(cpp_elf, key=lambda r: (r["sourcerpm"], r["name"], r["arch"], r["path"])))

    source_groups = defaultdict(list)
    for row in rpm_rows:
        source_groups[row["sourcerpm"]].append(row)
    source_rows = []
    for srpm, rows in sorted(source_groups.items()):
        source_rows.append({
            "source_name": source_names.get(srpm, "NOT_AVAILABLE"), "sourcerpm": srpm,
            "binary_names": ";".join(sorted({r["binary_name"] for r in rows})),
            "arches": ";".join(sorted({r["arch"] for r in rows})),
            "cpp_runtime_elf_count": str(sum(int(r["cpp_runtime_elf_count"]) for r in rows)),
            "libstdcpp_linked": "YES" if any(int(r["libstdcpp_needed_elf_count"]) for r in rows) else "NO",
        })
    source_fields = ["source_name", "sourcerpm", "binary_names", "arches", "cpp_runtime_elf_count", "libstdcpp_linked"]
    write(args.output_dir / "base_cpp_source_records_exact.tsv", source_fields, source_rows)

    image_rows = []
    for row in images:
        if row["rpm_sha256"] not in cpp_by_rpm:
            continue
        meta = binary_by_sha[row["rpm_sha256"]]
        image_rows.append({
            "image": row["image"], "binary_name": row["binary_name"], "arch": row["arch"],
            "version_release": row["version_release"], "sourcerpm": row["sourcerpm"],
            "source_name": source_names.get(row["sourcerpm"], "NOT_AVAILABLE"),
            "rpm_sha256": row["rpm_sha256"], "summary": row["summary"],
            "cpp_runtime_elf_paths": ";".join(sorted(r["path"] for r in cpp_by_rpm[row["rpm_sha256"]])),
        })
    image_fields = ["image", "binary_name", "arch", "version_release", "sourcerpm", "source_name",
                    "rpm_sha256", "summary", "cpp_runtime_elf_paths"]
    write(args.output_dir / "base_cpp_image_membership_exact.tsv", image_fields,
          sorted(image_rows, key=lambda r: (r["source_name"], r["binary_name"], r["image"])))

    print(f"runtime_elf_records={len(runtime)}")
    print(f"cpp_runtime_elf_records={len(cpp_elf)}")
    print(f"cpp_binary_rpm_records={len(rpm_rows)}")
    print(f"cpp_binary_names={len({r['binary_name'] for r in rpm_rows})}")
    print(f"cpp_source_rpm_records={len(source_rows)}")
    print(f"cpp_source_names={len({r['source_name'] for r in source_rows})}")
    linked_rpms = [r for r in rpm_rows if int(r["libstdcpp_needed_elf_count"])]
    print(f"libstdcpp_linked_binary_rpm_records={len(linked_rpms)}")
    print(f"libstdcpp_linked_binary_names={len({r['binary_name'] for r in linked_rpms})}")
    print(f"libstdcpp_linked_source_rpm_records={len({r['sourcerpm'] for r in linked_rpms})}")
    print(f"libstdcpp_linked_source_names={len({r['source_name'] for r in linked_rpms})}")
    print(f"image_cpp_membership_records={len(image_rows)}")
    print(f"image_cpp_binary_names={len({r['binary_name'] for r in image_rows})}")
    print(f"image_cpp_source_rpm_records={len({r['sourcerpm'] for r in image_rows})}")
    print(f"image_cpp_source_names={len({r['source_name'] for r in image_rows})}")


if __name__ == "__main__":
    main()
