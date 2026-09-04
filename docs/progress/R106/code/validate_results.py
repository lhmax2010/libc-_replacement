#!/usr/bin/env python3
"""Mechanical consistency checks for R106 result tables."""

import csv
import hashlib
import sys
from pathlib import Path


def rows(path):
    with open(path, encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def sha256(path):
    digest = hashlib.sha256()
    with open(path, "rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def main():
    root = Path(sys.argv[1])
    tables = root / "tables"
    checks = []

    def check(name, condition, detail):
        checks.append((name, "PASS" if condition else "FAIL", detail))

    summary = {r["repository"]: r for r in rows(tables / "repository_summary.tsv")}
    check("base_unique_sources", summary["Tizen-Base"]["unique_source_names"] == "251", summary["Tizen-Base"]["unique_source_names"])
    check("toolchain_unique_sources", summary["Tizen-Base-Toolchain"]["unique_source_names"] == "254", summary["Tizen-Base-Toolchain"]["unique_source_names"])
    check("base_unique_binaries", summary["Tizen-Base"]["unique_binary_names"] == "646", summary["Tizen-Base"]["unique_binary_names"])
    check("toolchain_unique_binaries", summary["Tizen-Base-Toolchain"]["unique_binary_names"] == "650", summary["Tizen-Base-Toolchain"]["unique_binary_names"])
    check("source_only_base", [r["source_package"] for r in rows(tables / "source_only_tizen_base.tsv")] == ["toybox"], "expected=toybox")
    check("source_only_toolchain", {r["source_package"] for r in rows(tables / "source_only_base_toolchain.tsv")} == {"binutils-armv7hl", "gcc-armv7hl", "libcxx-runtimes", "tensorflow2"}, "expected=four known sources")
    check("binary_only_base_count", len(rows(tables / "binary_only_tizen_base.tsv")) == 8, str(len(rows(tables / "binary_only_tizen_base.tsv"))))
    check("binary_only_toolchain_count", len(rows(tables / "binary_only_base_toolchain.tsv")) == 12, str(len(rows(tables / "binary_only_base_toolchain.tsv"))))
    normalized = rows(tables / "normalized_binary_records.tsv")
    check("binary_source_mapping_complete", all(r["source_name"] != "NOT_AVAILABLE" for r in normalized), f"rows={len(normalized)}")
    check("toolchain_binary_primary_sha", sha256(root / "inputs/toolchain_binary_primary.xml.gz") == "ba2bf090e651196a558fe76d2329ea8e236b785cff2657cd3858943bd81355ed", sha256(root / "inputs/toolchain_binary_primary.xml.gz"))
    check("toolchain_source_primary_sha", sha256(root / "inputs/toolchain_source_primary.xml.gz") == "eccde3e46dcba072b65538935eb3707f95a35d2d85feaa75b492a23669a72f53", sha256(root / "inputs/toolchain_source_primary.xml.gz"))
    check("toybox_source_rpm_sha", sha256(root / "inputs/toybox-0.6.0-1.1.src.rpm") == "e5894647c5cc2005dfbc53cb712f0f3068768edabb5251f7c0295573173a217e", sha256(root / "inputs/toybox-0.6.0-1.1.src.rpm"))
    for name, status, detail in checks:
        print(f"{name}\t{status}\t{detail}")
    if any(status != "PASS" for _, status, _ in checks):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
