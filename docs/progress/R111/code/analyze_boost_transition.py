#!/usr/bin/env python3
from __future__ import annotations

import csv
import subprocess
from collections import defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
EDGES = ROOT / "docs/progress/R82/tables/boost_edges_90.tsv"
CLASSIFIED = ROOT / "docs/progress/R110/tables/w3_edges_90_results.tsv"
MIGRATION = ROOT / "docs/progress/R24/tables/migration_packages_483.tsv"
OUT = ROOT / "docs/progress/R111/tables"
ARTIFACT_ROOT = Path(
    "/home/toolchain/development/libc++_replacement/progress/R105/artifacts/boost_aarch64_libcxx"
)
RELEASE = "105.10.5"
LIB_BY_PACKAGE = {
    "boost-filesystem": "libboost_filesystem.so.1.83.0",
    "boost-iostreams": "libboost_iostreams.so.1.83.0",
    "boost-program-options": "libboost_program_options.so.1.83.0",
    "boost-test": "libboost_unit_test_framework.so.1.83.0",
    "boost-thread": "libboost_thread.so.1.83.0",
}


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path: Path, fieldnames: list[str], rows: list[dict[str, object]]) -> None:
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fieldnames, delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)


def artifact_path(package: str) -> Path:
    lib = LIB_BY_PACKAGE[package]
    return ARTIFACT_ROOT / f"{package}-1.83.0-{RELEASE}.aarch64/usr/lib64/{lib}"


def defined_dynamic_symbols(path: Path) -> set[str]:
    completed = subprocess.run(
        ["nm", "-D", "--defined-only", str(path)],
        check=True,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    symbols: set[str] = set()
    for line in completed.stdout.splitlines():
        fields = line.split()
        if fields:
            symbols.add(fields[-1].split("@", 1)[0])
    return symbols


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    edges = read_tsv(EDGES)
    classified = {row["edge_id"]: row for row in read_tsv(CLASSIFIED)}
    migration = {row["source_rpm_identity"]: row for row in read_tsv(MIGRATION)}

    assert len(edges) == 90
    assert set(LIB_BY_PACKAGE) == {row["provider_binary"] for row in edges}
    consumers = sorted({row["consumer_source_rpm"] for row in edges})
    assert len(consumers) == 13

    by_consumer: dict[str, list[dict[str, str]]] = defaultdict(list)
    for row in edges:
        by_consumer[row["consumer_source_rpm"]].append(row)

    consumer_rows: list[dict[str, object]] = []
    for source_identity in consumers:
        rows = by_consumer[source_identity]
        plan = migration.get(source_identity)
        consumer_rows.append(
            {
                "consumer_source_rpm": source_identity,
                "edge_count": len(rows),
                "architectures": ";".join(sorted({row["arch"] for row in rows})),
                "boost_provider_packages": ";".join(sorted({row["provider_binary"] for row in rows})),
                "migration_plan_membership": "IN_SCOPE" if plan else "NOT_FOUND",
                "migration_stage": plan["stage"] if plan else "NOT_AVAILABLE",
                "source_modification_required": plan["source_modification_required"] if plan else "NOT_AVAILABLE",
                "build_config_modification_required": plan["build_config_modification_required"] if plan else "NOT_AVAILABLE",
                "actual_migrated_artifact_observed_in_this_check": "NOT_OBSERVED",
            }
        )

    write_tsv(
        OUT / "item2_consumer_migration.tsv",
        list(consumer_rows[0]),
        consumer_rows,
    )

    provider_symbols = {package: defined_dynamic_symbols(artifact_path(package)) for package in LIB_BY_PACKAGE}
    aarch_rows: list[dict[str, object]] = []
    for row in edges:
        if row["arch"] != "aarch64":
            continue
        old_symbols = {symbol for symbol in row["cpp_symbols"].split(";") if symbol}
        matched = sorted(old_symbols & provider_symbols[row["provider_binary"]])
        aarch_rows.append(
            {
                "edge_id": row["edge_id"],
                "consumer_source_rpm": row["consumer_source_rpm"],
                "provider_binary": row["provider_binary"],
                "requested_class": classified[row["edge_id"]]["requested_class"],
                "old_recorded_cpp_symbol_count": len(old_symbols),
                "same_mangled_symbols_in_adapted_provider": len(matched),
                "old_recorded_intersection_status": (
                    "PARTIAL_OR_FULL_INTERSECTION_REMAINS" if matched else "NO_RECORDED_SYMBOL_INTERSECTION_REMAINS"
                ),
                "matched_symbols": ";".join(matched),
            }
        )

    write_tsv(
        OUT / "item2_aarch64_old_consumer_symbol_overlap.tsv",
        list(aarch_rows[0]),
        aarch_rows,
    )

    summary: dict[tuple[str, str], int] = defaultdict(int)
    for row in aarch_rows:
        summary[(str(row["requested_class"]), str(row["old_recorded_intersection_status"]))] += 1
    summary_rows = [
        {"requested_class": key[0], "old_recorded_intersection_status": key[1], "edge_count": count}
        for key, count in sorted(summary.items())
    ]
    write_tsv(
        OUT / "item2_aarch64_overlap_summary.tsv",
        ["requested_class", "old_recorded_intersection_status", "edge_count"],
        summary_rows,
    )

    assert all(row["migration_plan_membership"] == "IN_SCOPE" for row in consumer_rows)
    assert len(aarch_rows) == 30
    print(
        "ASSERTIONS=PASS edges=90 consumers=13 providers_are_boost=90 "
        "migration_plan_members=13 aarch64_edges_compared=30"
    )
    for row in summary_rows:
        print(
            f"SUMMARY class={row['requested_class']} "
            f"status={row['old_recorded_intersection_status']} edges={row['edge_count']}"
        )


if __name__ == "__main__":
    main()
