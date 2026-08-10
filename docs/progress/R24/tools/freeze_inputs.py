#!/usr/bin/env python3
import csv
import hashlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "progress/R24/inputs/input_identity.tsv"

INPUTS = [
    "progress/R22/tables/source_edge_classification.tsv",
    "progress/R22/tables/migration_packages_full.tsv",
    "progress/R22/tables/special_cases.tsv",
    "progress/R22/tables/scc_topology.tsv",
    "progress/R22/tables/data_sources.tsv",
    "progress/R22/tables/download_time_ranges.tsv",
    "progress/R22/tools/classify_abi_edges.py",
    "progress/R22/tools/rephase_migration.py",
    "progress/R22/R22_migration_report.md",
    "progress/R19_21/R20/tables/cpp_abi_edges.tsv",
    "progress/R19_21/R20/tables/inverse_phase_dependencies.tsv",
    "progress/R19_21/R20/tables/phase_package_list.tsv",
    "progress/R19_21/R20/R20_migration_phases_report.md",
    "progress/R13/tables/plugin_host_closure_assignment.tsv",
    "progress/R13/tables/dependency_edge_resolution.tsv",
    "progress/R11/tables/binary_package_records.tsv",
    "progress/R11/tables/elf_inventory.tsv",
    "progress/R11/tables/snapshot_identity.tsv",
    "progress/R11/tables/repodata_inventory.tsv",
    "census/multiarch_v2/summary.md",
    "census/multiarch_v2/METHOD.md",
    "census/multiarch_v2/comparison/core_v2_arm_reference_counterparts.tsv",
    "census/multiarch_v2/comparison/package_presence.tsv",
    "census/multiarch_v2/comparison/snapshot_provenance.tsv",
    "ledger/overlay_armv7l_tier1.tsv",
    "ledger/overlay_aarch64_tier1.tsv",
    "ledger/overlay_x86_64_tier1.tsv",
]

OUT.parent.mkdir(parents=True, exist_ok=True)
with OUT.open("w", encoding="utf-8", newline="") as handle:
    writer = csv.DictWriter(
        handle,
        fieldnames=["path", "sha256", "size_bytes", "role"],
        delimiter="\t",
        lineterminator="\n",
    )
    writer.writeheader()
    for rel in INPUTS:
        path = ROOT / rel
        data = path.read_bytes()
        if rel.startswith("progress/R22"):
            role = "R22 corrected graph/staging input"
        elif rel.startswith("progress/R19_21/R20"):
            role = "R20 initial graph/staging input"
        elif rel.startswith("progress/R13"):
            role = "R13 dependency/plugin input"
        elif rel.startswith("progress/R11"):
            role = "R11 published RPM/ELF/snapshot input"
        elif rel.startswith("census/"):
            role = "aligned-0723 historical core input"
        else:
            role = "aligned-0723 per-arch core ledger"
        writer.writerow(
            {
                "path": rel,
                "sha256": hashlib.sha256(data).hexdigest(),
                "size_bytes": len(data),
                "role": role,
            }
        )

print(f"input_rows={len(INPUTS)}")
