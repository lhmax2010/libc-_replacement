#!/usr/bin/env python3
"""量化本地可得的产品镜像清单，并与冻结仓记录做身份对照。"""

from __future__ import annotations

import csv
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
OUT = ROOT / "docs/progress/R110/tables"
R11 = Path("/home/toolchain/development/libc++_replacement/progress/R11")


def load_r11() -> tuple[set[tuple[str, str]], set[tuple[str, str, str]], dict[tuple[str, str, str], str]]:
    by_name_arch: set[tuple[str, str]] = set()
    exact: set[tuple[str, str, str]] = set()
    exact_source: dict[tuple[str, str, str], str] = {}
    path = R11 / "tables/binary_package_records.tsv"
    with path.open(newline="", encoding="utf-8") as handle:
        for row in csv.DictReader(handle, delimiter="\t"):
            key = (row["name"], row["arch"])
            by_name_arch.add(key)
            exact.add((*key, f'{row["version"]}-{row["release"]}'))
            exact_source[(*key, f'{row["version"]}-{row["release"]}')] = row["sourcerpm"]
    return by_name_arch, exact, exact_source


def load_unresolved_sources() -> dict[str, str]:
    unresolved: dict[str, str] = {}
    path = ROOT / "docs/progress/R109/tables/w2_partial_packages.tsv"
    with path.open(newline="", encoding="utf-8") as handle:
        for row in csv.DictReader(handle, delimiter="\t"):
            if row["wbuffer_coverage"] == "PARTIAL":
                unresolved[row["source_rpm"]] = "PARTIAL"
    path = ROOT / "docs/progress/R92/tables/source_completeness.tsv"
    with path.open(newline="", encoding="utf-8") as handle:
        for row in csv.DictReader(handle, delimiter="\t"):
            status = row["source_content_status"]
            if status.startswith("PARTIAL"):
                unresolved[row["source_rpm"]] = "PARTIAL"
            elif status == "NOT_AVAILABLE":
                unresolved[row["source_rpm"]] = "NOT_AVAILABLE"
    return unresolved


def parse_manifest(path: Path):
    for lineno, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        fields = line.split()
        if len(fields) < 3 or "." not in fields[0]:
            raise ValueError(f"{path}:{lineno}: unexpected line: {line!r}")
        name, arch = fields[0].rsplit(".", 1)
        yield name, arch, fields[1], fields[2]


def main() -> None:
    by_name_arch, exact, exact_source = load_r11()
    unresolved = load_unresolved_sources()
    rows: list[dict[str, object]] = []
    unmatched: list[dict[str, str]] = []
    unresolved_members: list[dict[str, str]] = []
    generations = {
        "R95_REFERENCE_STANDARD": ROOT / "docs/progress/R95/inputs/image_packages",
        "R98_TOOLCHAIN_PAIR": ROOT / "docs/progress/R98/inputs/image_packages",
    }
    overall: dict[str, list[tuple[str, str, str, str]]] = {}
    for generation, directory in generations.items():
        generation_entries: list[tuple[str, str, str, str]] = []
        for manifest in sorted(directory.glob("*.packages")):
            entries = list(parse_manifest(manifest))
            generation_entries.extend(entries)
            exact_count = sum((n, a, vr) in exact for n, a, vr, _ in entries)
            name_arch_count = sum((n, a) in by_name_arch for n, a, _, _ in entries)
            rows.append({
                "generation": generation,
                "manifest": manifest.name,
                "membership_rows": len(entries),
                "distinct_name_arch": len({(n, a) for n, a, _, _ in entries}),
                "r11_exact_name_arch_version_release": exact_count,
                "r11_same_name_arch_any_version": name_arch_count,
                "r11_no_same_name_arch": len(entries) - name_arch_count,
                "interpretation": "HISTORICAL_OR_DIFFERENT_SNAPSHOT_INSTALL_LIST_NOT_EXTERNAL_BOM",
            })
            for n, a, vr, vcs in entries:
                if (n, a) not in by_name_arch:
                    unmatched.append({
                        "generation": generation,
                        "manifest": manifest.name,
                        "name": n,
                        "arch": a,
                        "version_release": vr,
                        "vcs": vcs,
                    })
                source_rpm = exact_source.get((n, a, vr))
                if source_rpm in unresolved:
                    unresolved_members.append({
                        "generation": generation,
                        "manifest": manifest.name,
                        "binary_name": n,
                        "arch": a,
                        "version_release": vr,
                        "source_rpm": source_rpm,
                        "wbuffer_source_coverage": unresolved[source_rpm],
                    })
        overall[generation] = generation_entries

    with (OUT / "w1_image_manifest_summary.tsv").open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(rows[0]), delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    with (OUT / "w1_image_manifest_unmatched_r11.tsv").open("w", newline="", encoding="utf-8") as handle:
        fields = ["generation", "manifest", "name", "arch", "version_release", "vcs"]
        writer = csv.DictWriter(handle, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(unmatched)
    with (OUT / "w1_image_manifest_wbuffer_unresolved.tsv").open("w", newline="", encoding="utf-8") as handle:
        fields = ["generation", "manifest", "binary_name", "arch", "version_release", "source_rpm", "wbuffer_source_coverage"]
        writer = csv.DictWriter(handle, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(unresolved_members)
    with (OUT / "w1_image_manifest_generation_summary.tsv").open("w", newline="", encoding="utf-8") as handle:
        fields = ["generation", "manifest_files", "membership_rows", "distinct_name_arch_version_release", "distinct_name_arch", "r11_exact_rows", "r11_same_name_arch_rows", "r11_no_same_name_arch_rows"]
        writer = csv.DictWriter(handle, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        for generation, entries in overall.items():
            writer.writerow({
                "generation": generation,
                "manifest_files": sum(r["generation"] == generation for r in rows),
                "membership_rows": len(entries),
                "distinct_name_arch_version_release": len({(n, a, vr) for n, a, vr, _ in entries}),
                "distinct_name_arch": len({(n, a) for n, a, _, _ in entries}),
                "r11_exact_rows": sum((n, a, vr) in exact for n, a, vr, _ in entries),
                "r11_same_name_arch_rows": sum((n, a) in by_name_arch for n, a, _, _ in entries),
                "r11_no_same_name_arch_rows": sum((n, a) not in by_name_arch for n, a, _, _ in entries),
            })

    counts = Counter(row["generation"] for row in unmatched)
    print(f"r11_name_arch={len(by_name_arch)} r11_exact={len(exact)}")
    for generation, entries in overall.items():
        print(f"{generation}: files={sum(r['generation'] == generation for r in rows)} rows={len(entries)} unmatched_name_arch={counts[generation]}")
    print(f"image_rows_with_exact_r11_unresolved_wbuffer_source={len(unresolved_members)}")


if __name__ == "__main__":
    main()
