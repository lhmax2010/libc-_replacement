#!/usr/bin/env python3
"""Aggregate the completed source-compatibility hit table by production package."""

from __future__ import annotations

import argparse
import csv
import pathlib


def write_tsv(path: pathlib.Path, header: tuple[str, ...], rows) -> None:
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(header)
        writer.writerows(rows)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--hits", type=pathlib.Path, required=True)
    parser.add_argument("--tokens", type=pathlib.Path, required=True)
    parser.add_argument("--output-dir", type=pathlib.Path, required=True)
    args = parser.parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)

    token_peer = {}
    with args.tokens.open(newline="", encoding="utf-8") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            token_peer[(row["family"], row["token"])] = row["exact_token_in_libcxx"]

    packages = {}
    missing = {}
    with args.hits.open(newline="", encoding="utf-8") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["scope"] != "PRODUCTION_LIKE":
                continue
            package = row["source_rpm"]
            item = packages.setdefault(package, {"known": row["known45"], "hits": 0, "families": set(), "missing": set()})
            item["hits"] += 1
            item["families"].add(row["family"])
            key = (row["family"], row["token"])
            if token_peer.get(key) == "NO":
                item["missing"].add(key)
                location = (package, row["file"], row["line"])
                missing.setdefault(key, set()).add(location)

    package_rows = []
    for package, item in sorted(packages.items()):
        package_rows.append((
            package,
            item["known"],
            item["hits"],
            len(item["families"]),
            ";".join(sorted(item["families"])),
            len(item["missing"]),
            ";".join(f"{family}:{token}" for family, token in sorted(item["missing"])),
        ))
    write_tsv(
        args.output_dir / "source_compat_production_packages.tsv",
        ("source_rpm", "known45", "hit_count", "family_count", "families", "exact_missing_token_count", "exact_missing_tokens"),
        package_rows,
    )
    write_tsv(
        args.output_dir / "source_compat_new_packages.tsv",
        ("source_rpm", "hit_count", "family_count", "families", "exact_missing_token_count", "exact_missing_tokens"),
        (row[:1] + row[2:] for row in package_rows if row[1] == "NO"),
    )

    missing_rows = []
    for (family, token), locations in sorted(missing.items()):
        packages_for_token = sorted({location[0] for location in locations})
        missing_rows.append((
            family,
            token,
            len(packages_for_token),
            len(locations),
            ";".join(packages_for_token),
            ";".join(f"{pkg}:{file}:{line}" for pkg, file, line in sorted(locations)),
        ))
    write_tsv(
        args.output_dir / "source_compat_exact_missing_tokens.tsv",
        ("family", "token", "package_count", "location_count", "packages", "locations"),
        missing_rows,
    )

    known_hits = sum(1 for row in package_rows if row[1] == "YES")
    new_hits = sum(1 for row in package_rows if row[1] == "NO")
    write_tsv(
        args.output_dir / "source_compat_union_summary.tsv",
        ("denominator", "production_candidate_packages", "known45_candidate_packages", "outside_known45_candidate_packages", "exact_missing_tokens"),
        [(371, len(package_rows), known_hits, new_hits, len(missing_rows))],
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
