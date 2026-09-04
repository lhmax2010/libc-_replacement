#!/usr/bin/env python3
import argparse
import csv
from collections import Counter
from pathlib import Path


PRIOR_PATTERNS = (
    ("boost-filesystem", ("filesystem6status", "read_symlink"), "status/read_symlink"),
    ("boost-iostreams", ("mapped_file_source9open_impl",), "mapped_file_source::open_impl"),
    ("boost-program-options", ("variables_mapC1", "variables_mapC2"), "variables_map constructor"),
)

PRIOR_OBSERVED_ARCHES = {
    "boost-filesystem": {"x86_64", "armv7l"},
    "boost-iostreams": {"x86_64", "armv7l"},
    "boost-program-options": {"x86_64"},
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()
    with args.input.open(newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    if len(rows) != 90:
        raise RuntimeError("expected 90 Boost edges")
    output = []
    for row in rows:
        old = row["classification"]
        if old in {"HIDDEN_STD_LAYOUT", "HIDDEN_STD_LAYOUT+DIRECT_STD_SIGNATURE", "DIRECT_STD_SIGNATURE"}:
            requested = "LAYOUT_SENSITIVE"
        elif old == "NO_STD_LAYOUT_EVIDENCE":
            requested = "CPP_ABI_COUPLED_LAYOUT_UNPROVEN"
        else:
            raise RuntimeError(f"unknown classification: {old}")
        symbols = row["cpp_symbols"]
        status = "UNMEASURED"
        tested_symbol = "-"
        reason = "not selected within this task's time budget"
        for provider, patterns, label in PRIOR_PATTERNS:
            if row["provider_binary"] == provider and any(pattern in symbols for pattern in patterns):
                status = ("PRIOR_SAMPLE_MEASURED" if row["arch"] in PRIOR_OBSERVED_ARCHES[provider]
                          else "PRIOR_SYMBOL_GROUP_ARCH_NOT_OBSERVED")
                tested_symbol = label
                reason = ("R82 representative mixed-runtime probe" if status == "PRIOR_SAMPLE_MEASURED"
                          else "same symbol group, but R82 did not execute this architecture")
                break
        if status == "UNMEASURED" and row["provider_binary"] == "boost-test" and "normalize_test_case_name" in symbols:
            status = "SELECTED_W3" if row["arch"] == "x86_64" else "W3_SYMBOL_GROUP_ARCH_NOT_OBSERVED"
            tested_symbol = "ut_detail::normalize_test_case_name"
            reason = ("provider returns libstdc++ std::string by value; consumer reads and destroys it"
                      if status == "SELECTED_W3" else "same selected symbol group; W3 executed x86_64 only")
        elif status == "UNMEASURED" and row["provider_binary"] == "boost-thread" and all(p in symbols for p in ("enter_once_region", "commit_once_region", "rollback_once_region")):
            status = "SELECTED_W3" if row["arch"] == "x86_64" else "W3_SYMBOL_GROUP_ARCH_NOT_OBSERVED"
            tested_symbol = "enter/rollback/commit_once_region"
            reason = ("covers x86_64 layout-unproven Boost.Thread edges with concrete state transitions"
                      if status == "SELECTED_W3" else "same selected symbol group; W3 executed x86_64 only")
        output.append({
            "edge_id": row["edge_id"], "consumer_source_rpm": row["consumer_source_rpm"],
            "arch": row["arch"], "consumer_binary": row["consumer_binary"],
            "consumer_elf": row["consumer_elf"], "provider_binary": row["provider_binary"],
            "provider_elf": row["provider_elf"], "requested_class": requested,
            "r82_class": old, "runtime_status": status, "tested_symbol_group": tested_symbol,
            "selection_reason": reason,
        })
    fields = list(output[0])
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(output)
    counts = Counter((r["requested_class"], r["runtime_status"]) for r in output)
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("requested_class", "runtime_status", "edges"))
        for key in sorted(counts):
            writer.writerow((*key, counts[key]))
        writer.writerow(("C_ONLY", "ALL", 0))
        writer.writerow(("NO_INTERSECTION", "ALL", 0))
    print(f"edges={len(output)}")
    print(f"prior={sum(r['runtime_status'] == 'PRIOR_SAMPLE_MEASURED' for r in output)}")
    print(f"selected={sum(r['runtime_status'] == 'SELECTED_W3' for r in output)}")
    print(f"unmeasured={sum(r['runtime_status'] == 'UNMEASURED' for r in output)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
