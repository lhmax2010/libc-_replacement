#!/usr/bin/env python3
import csv
from pathlib import Path

root = Path("/home/toolchain/development/libc++_replacement/progress/R51")

def load(path):
    with path.open() as stream:
        return list(csv.DictReader(stream, delimiter="\t"))

def write(path, columns, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w") as stream:
        writer = csv.DictWriter(stream, fieldnames=columns, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)

matrix = load(root / "tables/x86_64_matrices.tsv") + load(root / "tables/armv7l_matrices.tsv")

upstream_rows = []
for arch in ("x86_64", "armv7l"):
    tests = sorted({row["test"] for row in matrix if row["arch"] == arch and row["matrix"] == "upstream"})
    for test in tests:
        by_suite = {row["suite"]: row for row in matrix if row["arch"] == arch and row["matrix"] == "upstream" and row["test"] == test}
        upstream_rows.append({
            "arch": arch,
            "test": test,
            "unpatched_exit": by_suite["unpatched"]["exit_code"],
            "prestrip_exit": by_suite["prestrip"]["exit_code"],
            "product_exit": by_suite["product"]["exit_code"],
            "libstdcxx_exit": by_suite["libstdcxx_control"]["exit_code"],
            "product_result": "PASS" if by_suite["product"]["exit_code"] == "0" else "FAIL",
        })
write(root / "tables/upstream_test_matrix.tsv",
      ["arch", "test", "unpatched_exit", "prestrip_exit", "product_exit", "libstdcxx_exit", "product_result"], upstream_rows)

regression_rows = []
for arch in ("x86_64", "armv7l"):
    tests = sorted({row["test"] for row in matrix if row["arch"] == arch and row["matrix"] == "regression" and row["suite"] == "product"})
    for test in tests:
        by_suite = {row["suite"]: row for row in matrix if row["arch"] == arch and row["matrix"] == "regression" and row["test"] == test}
        prestrip = by_suite["prestrip"]
        product = by_suite["product"]
        control = by_suite["libstdcxx_control"]
        regression_rows.append({
            "arch": arch,
            "test": test,
            "prestrip_exit": prestrip["exit_code"],
            "product_exit": product["exit_code"],
            "libstdcxx_exit": control["exit_code"],
            "product_join": product["join"] or "N/A",
            "libstdcxx_join": control["join"] or "N/A",
            "product_vs_libstdcxx": "MATCH" if (product["exit_code"], product["join"]) == (control["exit_code"], control["join"]) else "DIFFER",
            "product_vs_prestrip": "MATCH" if (product["exit_code"], product["join"]) == (prestrip["exit_code"], prestrip["join"]) else "DIFFER",
        })
write(root / "tables/regression_matrix.tsv",
      ["arch", "test", "prestrip_exit", "product_exit", "libstdcxx_exit", "product_join", "libstdcxx_join", "product_vs_libstdcxx", "product_vs_prestrip"], regression_rows)

print(f"UPSTREAM_ROWS={len(upstream_rows)}")
print(f"REGRESSION_ROWS={len(regression_rows)}")
