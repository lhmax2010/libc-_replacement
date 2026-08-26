#!/usr/bin/env python3
"""Fail closed on R73 package integrity and key count invariants."""

from __future__ import annotations

import csv
import hashlib
from pathlib import Path


ROOT = Path("docs/progress/R73")


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    errors: list[str] = []
    with (ROOT / "INDEX.tsv").open(newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    for row in rows:
        path = Path(row["uploaded_path"])
        if not path.is_file():
            errors.append(f"missing:{path}")
            continue
        actual_hash = sha256(path)
        actual_size = path.stat().st_size
        if actual_hash != row["sha256"]:
            errors.append(f"sha256:{path}:{actual_hash}:{row['sha256']}")
        if actual_size != int(row["size_bytes"]):
            errors.append(f"size:{path}:{actual_size}:{row['size_bytes']}")

    all_files = {path.relative_to(ROOT).as_posix() for path in ROOT.rglob("*") if path.is_file()}
    indexed = {Path(row["uploaded_path"]).relative_to(ROOT).as_posix() for row in rows}
    expected_unindexed = {"INDEX.tsv"}
    unexpected = all_files - indexed - expected_unindexed
    if unexpected:
        errors.append(f"unindexed:{','.join(sorted(unexpected))}")

    oversized = [str(path) for path in ROOT.rglob("*") if path.is_file() and path.stat().st_size > 5 * 1024 * 1024]
    if oversized:
        errors.append(f"oversized:{','.join(oversized)}")

    with (ROOT / "tables/explicit_std_facility_summary.tsv").open(newline="") as stream:
        summary = {row["facility"]: row for row in csv.DictReader(stream, delimiter="\t")}
    if summary["ANY"]["source_rpm_count"] != "11" or summary["ANY"]["package_name_count"] != "9":
        errors.append("explicit_count_not_11_9")

    with (ROOT / "tables/package_risk_review.tsv").open(newline="") as stream:
        package_rows = list(csv.DictReader(stream, delimiter="\t"))
    if len(package_rows) != 9:
        errors.append(f"package_review_rows:{len(package_rows)}")

    print(f"indexed_rows={len(rows)}")
    print(f"all_files={len(all_files)}")
    print(f"max_file_bytes={max(Path(row['uploaded_path']).stat().st_size for row in rows)}")
    print(f"errors={len(errors)}")
    for error in errors:
        print(error)
    return 1 if errors else 0


if __name__ == "__main__":
    raise SystemExit(main())
