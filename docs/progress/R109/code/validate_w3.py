#!/usr/bin/env python3
import csv
import re
from collections import Counter
from pathlib import Path


ROOT = Path("docs/progress/R109")


def rows(path):
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def check(value, message):
    if not value:
        raise RuntimeError(message)
    print(f"PASS\t{message}")


def main():
    check((ROOT / "raw/200_resource_gate_w3_resource_gate_w3.exitcode").read_text().strip() == "0", "W3 resource gate passed")
    edges = rows(ROOT / "tables/w3_edges_90_results.tsv")
    classes = Counter(r["requested_class"] for r in edges)
    check(len(edges) == 90 and classes == Counter({"LAYOUT_SENSITIVE": 63, "CPP_ABI_COUPLED_LAYOUT_UNPROVEN": 27}), "all 90 edges have the required classification")
    check(sum(r["representative_symbol_measured_on_arch"] == "YES" for r in edges) == 27, "27 edge rows have representative-symbol measurements on their actual architecture")
    check(len(rows(ROOT / "tables/w3_unmeasured_edges.tsv")) == 63, "63 edge rows remain unmeasured on architecture")
    matrix = rows(ROOT / "tables/w3_runtime_matrix_retry.tsv")
    check(len(matrix) == 10, "new authoritative matrix has two five-run cells")
    test = [r for r in matrix if r["case"] == "boost_test_normalize"]
    thread = [r for r in matrix if r["case"] == "boost_thread_once"]
    check(len(test) == 5 and all(r["returncode"] == "41" and r["concrete_value"] == "0" and r["consumer_destroy_or_scope_exit"] == "YES" for r in test), "Boost.Test mixed cell is wrong-value with consumer destruction 5/5")
    check(all(re.search(r"observed_size=24 expected_size=16 value_ok=0", r["stdout"]) for r in test), "Boost.Test mixed cell asserts observed size 24 against expected 16")
    check(len(thread) == 5 and all(r["returncode"] == "0" and r["concrete_value"] == "1,1,0" and r["consumer_destroy_or_scope_exit"] == "YES" for r in thread), "Boost.Thread mixed cell asserts exact state sequence and scope exit 5/5")
    check(all(r["runtime_maps"] == "libcxx:1,libstdcxx:1,provider:1" for r in matrix), "all mixed runs map both runtimes and provider")
    reference = rows(ROOT / "tables/w3_libstdcxx_reference.tsv")
    check(len(reference) == 5 and all(r["returncode"] == "0" and r["value_ok"] == "YES" and r["consumer_destroy_completed"] == "YES" for r in reference), "libstdc++ same-provider reference passes value and destruction 5/5")
    check((ROOT / "W3_REPORT.md").is_file(), "W3 report exists")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
