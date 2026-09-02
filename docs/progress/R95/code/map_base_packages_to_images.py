#!/usr/bin/env python3
"""Map exact Base binary records to the frozen Unified standard image manifests."""

import argparse
import csv
from collections import defaultdict
from pathlib import Path


def read_tsv(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path, fields, rows):
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def parse_manifest(path):
    rows = []
    for line_number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        parts = line.split(maxsplit=2)
        if len(parts) != 3 or "." not in parts[0]:
            raise ValueError(f"{path}:{line_number}: unexpected line: {line!r}")
        name, arch = parts[0].rsplit(".", 1)
        rows.append((name, arch, parts[1], parts[2]))
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tables", type=Path, required=True)
    ap.add_argument("--manifests", type=Path, required=True)
    args = ap.parse_args()

    base = read_tsv(args.tables / "base_binary_records.tsv")
    cpp = read_tsv(args.tables / "base_cpp_binary_records.tsv")
    cpp_sources = read_tsv(args.tables / "base_cpp_source_records.tsv")
    source_names = {row["sourcerpm"]: row["source_name"] for row in cpp_sources}
    cpp_sha = {row["rpm_sha256"] for row in cpp}
    index = defaultdict(list)
    for row in base:
        key = (row["name"], row["arch"], f"{row['version']}-{row['release']}", row["vcs"])
        index[key].append(row)

    output = []
    unmatched = []
    for manifest in sorted(args.manifests.glob("*.packages")):
        image = manifest.stem
        for name, arch, vr, vcs in parse_manifest(manifest):
            matches = index.get((name, arch, vr, vcs), [])
            if not matches:
                continue
            if len(matches) != 1:
                raise ValueError(f"ambiguous Base match: {image} {name}.{arch} {vr} {vcs}")
            row = matches[0]
            output.append({
                "image": image,
                "binary_name": name,
                "arch": arch,
                "version_release": vr,
                "vcs": vcs,
                "sourcerpm": row["sourcerpm"],
                "rpm_sha256": row["checksum"],
                "cpp_runtime_dependency": "YES" if row["checksum"] in cpp_sha else "NO",
                "summary": row["summary"],
            })

    fields = ["image", "binary_name", "arch", "version_release", "vcs", "sourcerpm",
              "rpm_sha256", "cpp_runtime_dependency", "summary"]
    write_tsv(args.tables / "base_binary_image_membership.tsv", fields,
              sorted(output, key=lambda r: (r["image"], r["binary_name"], r["arch"])))
    cpp_output = [row for row in output if row["cpp_runtime_dependency"] == "YES"]
    write_tsv(args.tables / "base_cpp_binary_image_membership.tsv", fields,
              sorted(cpp_output, key=lambda r: (r["sourcerpm"], r["binary_name"], r["image"])))

    all_manifest_records = sum(len(parse_manifest(path)) for path in args.manifests.glob("*.packages"))
    print(f"image_count={len(list(args.manifests.glob('*.packages')))}")
    print(f"image_package_records={all_manifest_records}")
    print(f"base_image_membership_records={len(output)}")
    print(f"base_image_binary_names={len({r['binary_name'] for r in output})}")
    print(f"base_image_source_rpms={len({r['sourcerpm'] for r in output})}")
    print(f"base_cpp_image_membership_records={len(cpp_output)}")
    print(f"base_cpp_image_binary_names={len({r['binary_name'] for r in cpp_output})}")
    print(f"base_cpp_image_source_rpms={len({r['sourcerpm'] for r in cpp_output})}")
    print(f"base_cpp_image_source_names={len({source_names[r['sourcerpm']] for r in cpp_output})}")


if __name__ == "__main__":
    main()
