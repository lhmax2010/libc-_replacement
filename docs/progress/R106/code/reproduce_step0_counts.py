#!/usr/bin/env python3
"""Evaluate plausible, explicit count variants against the documented step0 totals."""

import csv
import sys
from pathlib import Path


def read(path):
    with open(path, encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, rows, fields):
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    if len(sys.argv) != 5:
        raise SystemExit("usage: reproduce_step0_counts.py BASE_BINARY R95_EXACT TOOLCHAIN_BINARY OUTPUT")
    base = read(sys.argv[1])
    exact = read(sys.argv[2])
    toolchain = read(sys.argv[3])
    out = Path(sys.argv[4])

    def meta_rows(records, excluded=()):
        return [r for r in records if r["arch"] not in set(excluded)]

    def meta_counts(records, excluded=()):
        rows = meta_rows(records, excluded)
        return len({r["name"] for r in rows}), len({r["name"] for r in rows if "libstdc++.so.6" in r["requires"]})

    variants = []
    for repo, records in [("Tizen-Base", base), ("Tizen-Base-Toolchain", toolchain)]:
        for label, excluded in [
            ("ALL_ARCH_RECORDS_UNIQUE_NAMES", ()),
            ("EXCLUDE_RISCV64_UNIQUE_NAMES", ("riscv64",)),
            ("THREE_TARGETS_PLUS_NOARCH", ("riscv64", "i586")),
        ]:
            total, dependent = meta_counts(records, excluded)
            variants.append({"repository": repo, "variant": label,
                             "unique_binary_names": total,
                             "unique_repodata_libstdcpp_names": dependent,
                             "input": "PRIMARY_XML_REQUIRES"})

    # R95's exact payload scan follows actual ELF DT_NEEDED entries rather than RPM Requires.
    filtered = [r for r in exact if r["arch"] not in {"riscv64", "noarch"}]
    payload_names = {r["binary_name"] for r in filtered if int(r["libstdcpp_needed_elf_count"]) > 0}
    variants.append({"repository": "Tizen-Base", "variant": "ARCH_SPECIFIC_PAYLOAD_DT_NEEDED_EXCLUDE_RISCV64",
                     "unique_binary_names": "NOT_APPLICABLE",
                     "unique_repodata_libstdcpp_names": len(payload_names),
                     "input": "R95_EXACT_RPM_PAYLOAD_ELF_SCAN"})
    write(out / "step0_count_variants.tsv", variants,
          ["repository", "variant", "unique_binary_names", "unique_repodata_libstdcpp_names", "input"])

    primary_names = {r["name"] for r in base if r["arch"] != "riscv64" and "libstdc++.so.6" in r["requires"]}
    delta = ([{"relation": "PAYLOAD_ONLY", "binary_package": name} for name in sorted(payload_names - primary_names)] +
             [{"relation": "REPODATA_ONLY", "binary_package": name} for name in sorted(primary_names - payload_names)])
    write(out / "step0_dependency_method_delta.tsv", delta, ["relation", "binary_package"])
    print("step0_variant_analysis_complete")
    print(f"payload_dt_needed_names={len(payload_names)}")
    print(f"repodata_requires_names={len(primary_names)}")


if __name__ == "__main__":
    main()
