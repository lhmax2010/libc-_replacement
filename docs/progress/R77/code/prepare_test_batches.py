#!/usr/bin/env python3
"""Create exact lit filter batches, optionally excluding an interrupted prefix."""

import argparse
import json
import re
from pathlib import Path


RESULT_RE = re.compile(
    r"^(PASS|FAIL|XFAIL|XPASS|UNSUPPORTED|UNRESOLVED|TIMEOUT): "
    r".*? :: (.+?) \(([0-9]+) of [0-9]+\)$"
)


def normalize_name(name: str) -> str:
    for marker in ("/libcxx/test/", "/libcxxabi/test/"):
        if marker in name:
            return name.split(marker, 1)[1]
    return name


def read_order(path: Path) -> list[str]:
    tests = []
    for line in path.read_text(errors="replace").splitlines():
        if " :: " not in line:
            continue
        tests.append(normalize_name(line.split(" :: ", 1)[1].strip()))
    if len(tests) != len(set(tests)):
        raise RuntimeError("discovered test list contains duplicates")
    return tests


def read_old_results(path: Path) -> dict[str, str]:
    results = {}
    for line in path.read_text(errors="replace").splitlines():
        match = RESULT_RE.match(line)
        if match:
            status, name, _ = match.groups()
            results[normalize_name(name)] = status
    return results


def read_gate_results(path: Path) -> dict[str, str]:
    data = json.loads(path.read_text())
    results = {}
    for test in data["tests"]:
        name = normalize_name(test["name"].split(" :: ", 1)[1])
        results[name] = test["code"]
    return results


def write_lines(path: Path, lines) -> None:
    path.write_text("".join(f"{line}\n" for line in lines), encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--order", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--batch-size", type=int, default=500)
    parser.add_argument("--old-stdout", type=Path)
    parser.add_argument("--gate-json", type=Path)
    args = parser.parse_args()

    all_tests = read_order(args.order)
    old_results = read_old_results(args.old_stdout) if args.old_stdout else {}
    gate_results = read_gate_results(args.gate_json) if args.gate_json else {}
    nonpassing_gate = {name: code for name, code in gate_results.items() if code != "PASS"}
    if nonpassing_gate:
        raise RuntimeError(f"gate contains non-PASS tests: {nonpassing_gate}")
    unknown = (set(old_results) | set(gate_results)) - set(all_tests)
    if unknown:
        raise RuntimeError(f"prior results contain unknown tests: {sorted(unknown)}")
    overlap = set(old_results) & set(gate_results)
    if overlap:
        raise RuntimeError(f"gate reran completed tests: {sorted(overlap)}")

    excluded = set(old_results) | set(gate_results)
    pending = [test for test in all_tests if test not in excluded]
    args.output.mkdir(parents=True, exist_ok=True)
    write_lines(args.output / "all_tests.txt", all_tests)
    write_lines(args.output / "old_completed_tests.txt", sorted(old_results))
    write_lines(args.output / "gate_passed_tests.txt", sorted(gate_results))
    write_lines(args.output / "pending_tests.txt", pending)

    manifest = ["batch\tfirst_test\tlast_test\ttest_count\n"]
    for offset in range(0, len(pending), args.batch_size):
        tests = pending[offset : offset + args.batch_size]
        number = offset // args.batch_size + 1
        batch = args.output / f"batch-{number:03d}"
        batch.mkdir(exist_ok=True)
        write_lines(batch / "tests.txt", tests)
        expression = "(?:" + "|".join(re.escape(test) for test in tests) + ")$"
        (batch / "filter.regex").write_text(expression, encoding="utf-8")
        manifest.append(
            f"{number:03d}\t{tests[0]}\t{tests[-1]}\t{len(tests)}\n"
        )
    (args.output / "manifest.tsv").write_text("".join(manifest), encoding="utf-8")
    (args.output / "SUMMARY.tsv").write_text(
        "field\tvalue\n"
        f"discovered\t{len(all_tests)}\n"
        f"old_completed\t{len(old_results)}\n"
        f"gate_passed\t{len(gate_results)}\n"
        f"pending\t{len(pending)}\n"
        f"batch_size\t{args.batch_size}\n"
        f"batch_count\t{(len(pending) + args.batch_size - 1) // args.batch_size}\n",
        encoding="utf-8",
    )
    print((args.output / "SUMMARY.tsv").read_text(), end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
