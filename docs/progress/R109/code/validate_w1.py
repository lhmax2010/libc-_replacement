#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path


ROOT = Path("docs/progress/R109")


def rows(path):
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def check(condition, message):
    if not condition:
        raise RuntimeError(message)
    print(f"PASS\t{message}")


def main():
    check((ROOT / "raw/002_resource_gate_w1.exitcode").read_text().strip() == "0", "resource gate passed")
    check((ROOT / "raw/007_make_w1_removed_overlay.exitcode").read_text().strip() == "1", "initial guarded overlay attempt retained as expected failure")
    check((ROOT / "raw/007r_make_w1_removed_overlay_retry.exitcode").read_text().strip() == "0", "corrected overlay generation passed")
    behavior = rows(ROOT / "tables/w1_behavior_summary.tsv")
    got = Counter((r["cell"], r["mode"], r["outcome"]) for r in behavior for _ in range(int(r["count"])))
    expected = {
        ("retained", "unowned", "TERMINATE"), ("retained", "backend", "TERMINATE"),
        ("removed", "unowned", "SYSTEM_ERROR"), ("removed", "backend", "SYSTEM_ERROR"),
        ("oldcaller_removedimpl", "unowned", "TERMINATE"),
        ("oldcaller_removedimpl", "backend", "TERMINATE"),
        ("libstdcxx", "unowned", "RETURNED"), ("libstdcxx", "backend", "RETURNED"),
    }
    check(set(got) == expected and all(got[key] == 5 for key in expected), "behavior matrix has eight expected 5-run cells")
    default = rows(ROOT / "tables/w1_default_terminate_matrix.tsv")
    check(len(default) == 10 and all(r["returncode"] == "-6" for r in default), "default terminate matrix is SIGABRT 10/10")
    binary_summary = {r["metric"]: r["value"] for r in rows(ROOT / "tables/w1_binary_callers_summary.tsv")}
    check(binary_summary["elf_records"] == "39166" and binary_summary["parse_failures"] == "0", "binary audit covered 39166 ELF with zero parser failures")
    consumers = {r["metric"]: r["value"] for r in rows(ROOT / "tables/w1_binary_consumers_summary.tsv")}
    check(consumers["consumer_elfs"] == "601" and consumers["consumer_elfs_R24_REBUILD_PLAN"] == "601", "all 601 repository/architecture/ELF consumers map to R24 rebuild plan")
    source = rows(ROOT / "tables/w1_source_callers.tsv")
    check(len(source) == 92, "source adjudication contains 92 directly attributable calls")
    check(sum(r["source_scope"] == "PRODUCTION_OR_SHIPPED_HEADER" for r in source) == 8, "source adjudication contains eight production/header calls")
    check(sum(r["final_ownership_assessment"] == "NOT_DETERMINABLE" for r in source) == 0, "all directly attributable calls adjudicated")
    for stem in ("W1_REPORT.md",):
        check((ROOT / stem).is_file(), f"{stem} exists")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
