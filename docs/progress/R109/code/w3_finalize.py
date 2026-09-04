#!/usr/bin/env python3
import argparse
import csv
from collections import Counter
from pathlib import Path


def read(path):
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--edges", required=True, type=Path)
    parser.add_argument("--new-summary", required=True, type=Path)
    parser.add_argument("--reference", required=True, type=Path)
    parser.add_argument("--prior-matrix", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--unmeasured", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()
    edges = read(args.edges)
    new = {(r["case"], r["outcome"]): int(r["count"]) for r in read(args.new_summary)}
    if new != {
        ("boost_test_normalize", "WRONG_VALUE_DESTROY_RETURNED"): 5,
        ("boost_thread_once", "STATE_AND_SCOPE_EXIT_OK"): 5,
    }:
        raise RuntimeError("unexpected W3 matrix result")
    reference = read(args.reference)
    if len(reference) != 5 or not all(r["returncode"] == "0" and r["value_ok"] == "YES" and r["consumer_destroy_completed"] == "YES" for r in reference):
        raise RuntimeError("libstdc++ reference control failed")
    prior = read(args.prior_matrix)
    if len(prior) != 25:
        raise RuntimeError("unexpected R82 prior matrix size")
    output = []
    for row in edges:
        status = row["runtime_status"]
        if status == "PRIOR_SAMPLE_MEASURED":
            if row["provider_binary"] == "boost-filesystem":
                result = "PRIOR_WRONG_VALUE"
            elif row["provider_binary"] == "boost-iostreams":
                result = "PRIOR_TERMINATED_BEFORE_VALUE"
            else:
                result = "PRIOR_TERMINATED_DURING_ACCESS"
            measured = "YES"
            risk = "OBSERVED_INCOMPATIBLE"
        elif status == "SELECTED_W3" and row["provider_binary"] == "boost-test":
            result = "W3_WRONG_VALUE_DESTROY_RETURNED_5_OF_5"
            measured = "YES"
            risk = "OBSERVED_SILENT_WRONG_VALUE"
        elif status == "SELECTED_W3" and row["provider_binary"] == "boost-thread":
            result = "W3_STATE_AND_SCOPE_EXIT_OK_5_OF_5"
            measured = "YES"
            risk = "SAMPLED_PATH_OK_NOT_GLOBAL_SAFETY_PROOF"
        elif status in {"PRIOR_SYMBOL_GROUP_ARCH_NOT_OBSERVED", "W3_SYMBOL_GROUP_ARCH_NOT_OBSERVED"}:
            result = "NOT_OBSERVED_FOR_ARCHITECTURE"
            measured = "NO"
            risk = ("HIGH_LAYOUT_EVIDENCE_SAME_SYMBOL_FAMILY_FAILED_ELSEWHERE"
                    if row["requested_class"] == "LAYOUT_SENSITIVE"
                    else "UNKNOWN_CPP_ABI_COUPLING_ARCHITECTURE_UNTESTED")
        else:
            result = "NOT_OBSERVED_SYMBOL_GROUP"
            measured = "NO"
            risk = ("HIGH_LAYOUT_EVIDENCE_AND_PROVIDER_FAMILY_FAILURE"
                    if row["requested_class"] == "LAYOUT_SENSITIVE"
                    else "UNKNOWN_CPP_ABI_COUPLING_LAYOUT_UNPROVEN")
        output.append({
            **row, "representative_symbol_measured_on_arch": measured,
            "observed_result": result, "unmeasured_risk": risk,
            "measurement_extent": "ONE_REPRESENTATIVE_SYMBOL_GROUP_ON_EDGE_NOT_ALL_CPP_SYMBOLS",
        })
    fields = list(output[0])
    write(args.output, fields, output)
    missing = [r for r in output if r["representative_symbol_measured_on_arch"] == "NO"]
    write(args.unmeasured, fields, missing)
    counts = Counter((r["requested_class"], r["observed_result"]) for r in output)
    summary = []
    for key in sorted(counts):
        summary.append({"requested_class": key[0], "observed_result": key[1], "edges": counts[key]})
    summary.extend([
        {"requested_class": "ALL", "observed_result": "TOTAL", "edges": len(output)},
        {"requested_class": "ALL", "observed_result": "REPRESENTATIVE_SYMBOL_MEASURED_ON_ARCH", "edges": len(output) - len(missing)},
        {"requested_class": "ALL", "observed_result": "NOT_MEASURED_ON_ARCH", "edges": len(missing)},
    ])
    write(args.summary, ("requested_class", "observed_result", "edges"), summary)
    print(f"edges={len(output)}")
    print(f"measured_on_arch={len(output)-len(missing)}")
    print(f"not_measured_on_arch={len(missing)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
