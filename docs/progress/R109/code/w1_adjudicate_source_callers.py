#!/usr/bin/env python3
import argparse
import csv
import re
from collections import Counter
from pathlib import Path


NON_PRODUCTION = re.compile(
    r"/(test|tests|testsuite|example|examples|performance|benchmark|benchmarks)/|"
    r"_test\.|unittest", re.IGNORECASE
)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()
    with args.input.open(newline="") as stream:
        rows = [r for r in csv.DictReader(stream, delimiter="\t")
                if r["source_rpm"] != "POSITIVE_CONTROL"
                and r["attribution"] == "DIRECT_STD_DECLARATION"]
    output = []
    for row in rows:
        name = Path(row["path"]).name
        scope = "TEST_EXAMPLE_BENCH" if NON_PRODUCTION.search(row["path"]) else "PRODUCTION_OR_SHIPPED_HEADER"
        assessment = row["ownership_assessment"]
        basis = row["basis"]
        if name == "time2_demo.cpp" and row["line"] in {"210", "211"}:
            final = "NOT_BUILDABLE_EXAMPLE"
            final_basis = "passes std::mutex where the standard API requires std::unique_lock<std::mutex>&"
        elif assessment == "POSSIBLY_NOT_HELD" and name in {"latch.hpp", "thread_barrier.hpp", "barrier.hpp"}:
            final = "SOURCE_INDICATES_HELD"
            final_basis = "unlock occurs only in a branch that returns before the wait; waiting branch retains the lock"
        elif assessment == "NOT_DETERMINABLE" and name == "std_event.hpp":
            final = "SOURCE_INDICATES_HELD"
            final_basis = "entry asserts lock.locked(); adapter constructs unique_lock with adopt_lock and releases ownership on exit"
        elif assessment == "NOT_DETERMINABLE" and name == "buffered_channel.hpp":
            final = "SOURCE_INDICATES_HELD"
            final_basis = "enclosing branch constructs std::unique_lock<std::mutex> directly from mtx_ before wait"
        elif assessment == "NOT_DETERMINABLE" and name in {
            "perf_condition_variable.cpp", "yield_to.hpp", "103382.cc", "3.cc"
        }:
            final = "SOURCE_INDICATES_HELD"
            final_basis = "manual control-flow review found direct owning unique_lock construction before wait"
        elif assessment == "LIKELY_HELD":
            final = "SOURCE_INDICATES_HELD"
            final_basis = basis
        else:
            final = "NOT_DETERMINABLE"
            final_basis = basis
        output.append({**row, "source_scope": scope,
                       "final_ownership_assessment": final,
                       "final_basis": final_basis})

    fields = list(output[0]) if output else []
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(output)
    counts = Counter((r["source_scope"], r["final_ownership_assessment"]) for r in output)
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source_scope", "final_ownership_assessment", "calls", "files", "source_rpms"))
        for key in sorted(counts):
            selected = [r for r in output if (r["source_scope"], r["final_ownership_assessment"]) == key]
            writer.writerow((*key, len(selected), len({r["path"] for r in selected}),
                             len({r["source_rpm"] for r in selected})))
    print(f"DIRECT_CALLS={len(output)}")
    print(f"PRODUCTION_CALLS={sum(r['source_scope'] == 'PRODUCTION_OR_SHIPPED_HEADER' for r in output)}")
    print(f"NOT_DETERMINABLE={sum(r['final_ownership_assessment'] == 'NOT_DETERMINABLE' for r in output)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
