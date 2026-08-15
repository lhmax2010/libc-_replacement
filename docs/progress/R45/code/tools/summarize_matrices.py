#!/usr/bin/env python3
import csv
from pathlib import Path

root = Path("/home/toolchain/development/libc++_replacement/progress/R45")

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
            "r42_exit": by_suite["r42"]["exit_code"],
            "r45_exit": by_suite["r45"]["exit_code"],
            "libstdcxx_exit": by_suite["libstdcxx_control"]["exit_code"],
            "r45_result": "PASS" if by_suite["r45"]["exit_code"] == "0" else "FAIL",
        })
write(root / "tables/upstream_test_matrix.tsv",
      ["arch", "test", "unpatched_exit", "r42_exit", "r45_exit", "libstdcxx_exit", "r45_result"], upstream_rows)

regression_rows = []
for arch in ("x86_64", "armv7l"):
    tests = sorted({row["test"] for row in matrix if row["arch"] == arch and row["matrix"] == "regression" and row["suite"] == "r45"})
    for test in tests:
        by_suite = {row["suite"]: row for row in matrix if row["arch"] == arch and row["matrix"] == "regression" and row["test"] == test}
        r42 = by_suite.get("r42", {})
        r45 = by_suite["r45"]
        control = by_suite["libstdcxx_control"]
        regression_rows.append({
            "arch": arch,
            "test": test,
            "r42_exit": r42.get("exit_code", "N/A"),
            "r45_exit": r45["exit_code"],
            "libstdcxx_exit": control["exit_code"],
            "r45_join": r45["join"] or "N/A",
            "libstdcxx_join": control["join"] or "N/A",
            "exit_and_join_relation": "MATCH" if (r45["exit_code"], r45["join"]) == (control["exit_code"], control["join"]) else "DIFFER",
        })
write(root / "tables/regression_matrix.tsv",
      ["arch", "test", "r42_exit", "r45_exit", "libstdcxx_exit", "r45_join", "libstdcxx_join", "exit_and_join_relation"], regression_rows)

arm_aggregation = [row for row in matrix if row["arch"] == "armv7l" and row["matrix"] == "aggregation"]
x86_aggregation = load(root / "tables/x86_64_aggregation.tsv")
aggregation_rows = []
for row in arm_aggregation + x86_aggregation:
    functional = "JOIN_CANCELED" if "JOIN_IS_CANCELED=1" in row["join"] else ("ABORT_BEFORE_JOIN" if row["exit_code"] == "134" else "OTHER")
    aggregation_rows.append({
        "arch": row["arch"],
        "suite": row["suite"],
        "repeat": row["repeat"],
        "exit_code": row["exit_code"],
        "signal": row["signal"],
        "join": row["join"] or "N/A",
        "functional_observation": functional,
    })
write(root / "tables/aggregation_matrix.tsv",
      ["arch", "suite", "repeat", "exit_code", "signal", "join", "functional_observation"], aggregation_rows)

print(f"UPSTREAM_ROWS={len(upstream_rows)}")
print(f"REGRESSION_ROWS={len(regression_rows)}")
print(f"AGGREGATION_ROWS={len(aggregation_rows)}")
