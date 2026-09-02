#!/usr/bin/env python3
"""Recreate R90's exact gap and prove binary-corpus coverage for its C++ subset."""

from __future__ import annotations

import csv
from collections import defaultdict
from pathlib import Path


SOURCE_STATUS = Path("progress/R11/tables/source_download_status.tsv")
R32_STATUS = Path("docs/progress/R32/tables/scan_status.tsv")
ELF_INVENTORY = Path("progress/R11/tables/elf_inventory.tsv")
OUT = Path("progress/R92/tables")


def read(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path: Path, fields: tuple[str, ...], rows: list[dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main() -> int:
    sources = read(SOURCE_STATUS)
    r32 = read(R32_STATUS)
    elfs = read(ELF_INVENTORY)
    scanned = {row["source_rpm"] for row in r32 if row["status"] == "SCAN_OK"}
    frozen = {row["location"] for row in sources}
    exact_scanned = frozen & scanned
    gap = frozen - exact_scanned

    cpp_rows: dict[str, list[dict[str, str]]] = defaultdict(list)
    all_elf_rows: dict[str, list[dict[str, str]]] = defaultdict(list)
    for row in elfs:
        all_elf_rows[row["sourcerpm"]].append(row)
        if row["cpp_indicator"] == "YES":
            cpp_rows[row["sourcerpm"]].append(row)
    cpp_gap = sorted(name for name in gap if cpp_rows[name])
    if (len(sources), len(scanned), len(exact_scanned), len(gap), len(cpp_gap)) != (1450, 371, 345, 1105, 140):
        raise SystemExit(
            "denominator mismatch: "
            f"sources={len(sources)} scanned={len(scanned)} exact={len(exact_scanned)} "
            f"gap={len(gap)} cpp_gap={len(cpp_gap)}"
        )

    by_name = {row["location"]: row for row in sources}
    manifest: list[dict[str, object]] = []
    coverage: list[dict[str, object]] = []
    for name in cpp_gap:
        source = by_name[name]
        related = all_elf_rows[name]
        cpp_related = cpp_rows[name]
        manifest.append({
            "source_rpm": name,
            "package_name": source["name"],
            "compressed_bytes": source["package_size"],
            "sha256": source["actual_sha256"],
            "local_path": source["target"],
            "download_result": source["result"],
            "local_file_present": "YES" if Path(source["target"]).is_file() else "NO",
            "published_elf_rows": len(related),
            "cpp_indicator_elf_rows": len(cpp_related),
        })
        for row in cpp_related:
            coverage.append({
                "source_rpm": name,
                "repo_id": row["repo_id"],
                "binary_package": row["name"],
                "arch": row["arch"],
                "path": row["path"],
                "cpp_indicator": row["cpp_indicator"],
                "r87_elf_scan_scope": "INCLUDED",
                "basis": "R87 scanned every R11 ELF inventory row (39166/39166)",
            })

    write(OUT / "cpp_gap_140.tsv", (
        "source_rpm", "package_name", "compressed_bytes", "sha256", "local_path",
        "download_result", "local_file_present", "published_elf_rows", "cpp_indicator_elf_rows",
    ), manifest)
    write(OUT / "cpp_gap_binary_coverage.tsv", (
        "source_rpm", "repo_id", "binary_package", "arch", "path", "cpp_indicator",
        "r87_elf_scan_scope", "basis",
    ), coverage)
    summary = [
        {"metric": "frozen_source_records", "value": len(sources)},
        {"metric": "r87_scan_ok_records", "value": len(scanned)},
        {"metric": "exact_frozen_records_already_scanned", "value": len(exact_scanned)},
        {"metric": "exact_frozen_gap", "value": len(gap)},
        {"metric": "cpp_indicator_gap_records", "value": len(cpp_gap)},
        {"metric": "cpp_gap_source_files_present", "value": sum(row["local_file_present"] == "YES" for row in manifest)},
        {"metric": "cpp_gap_published_elf_rows", "value": len(coverage)},
        {"metric": "cpp_gap_elf_rows_in_r87_scope", "value": sum(row["r87_elf_scan_scope"] == "INCLUDED" for row in coverage)},
    ]
    write(OUT / "coverage_summary.tsv", ("metric", "value"), summary)
    for row in summary:
        print(f"{row['metric']}={row['value']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
