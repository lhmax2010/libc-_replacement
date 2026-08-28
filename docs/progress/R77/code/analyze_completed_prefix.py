#!/usr/bin/env python3
"""Assess whether the interrupted prefix completed any stdin-dependent test."""

import re
from pathlib import Path


ROOT = Path("/home/toolchain/development/libc++_replacement")
ORDER = ROOT / "progress/R77/raw/034_patched_test_order.stdout"
STDIN_SEARCH = ROOT / "progress/R77/raw/033_stdin_dependent_tests_search.stdout"
OLD_STDOUT = (
    ROOT
    / "progress/R68/raw/full_run/results/patched_armv7l/libcxx/lit.stdout"
)


def main() -> int:
    ordered = []
    for line in ORDER.read_text(errors="replace").splitlines():
        if " :: " not in line:
            continue
        name = line.split(" :: ", 1)[1].strip()
        marker = "/libcxx/test/"
        if marker in name:
            name = name.split(marker, 1)[1]
        ordered.append(name)

    stdin_tests = []
    prefix = "tmp/R69/replay/libcxx/test/"
    for line in STDIN_SEARCH.read_text(errors="replace").splitlines():
        path = line.split(":", 1)[0]
        if prefix not in path:
            raise RuntimeError(f"unexpected stdin-search record: {line}")
        stdin_tests.append(path.split(prefix, 1)[1])

    result_pattern = re.compile(
        r"^(PASS|FAIL|XFAIL|XPASS|UNSUPPORTED|UNRESOLVED|TIMEOUT): "
        r".*? :: (.+?) \(([0-9]+) of 11321\)$"
    )
    completed = {}
    max_completion_ordinal = 0
    for line in OLD_STDOUT.read_text(errors="replace").splitlines():
        match = result_pattern.match(line)
        if not match:
            continue
        status, name, ordinal = match.groups()
        completed[name] = status
        max_completion_ordinal = max(max_completion_ordinal, int(ordinal))

    print(f"discovered_tests\t{len(ordered)}")
    print(f"completed_result_lines\t{len(completed)}")
    print(f"max_completion_ordinal\t{max_completion_ordinal}")
    print(f"stdin_dependent_tests\t{len(stdin_tests)}")
    print("test\tlexical_position\told_completed_status")
    for test in stdin_tests:
        position = ordered.index(test) + 1
        print(f"{test}\t{position}\t{completed.get(test, 'NOT_COMPLETED')}")

    completed_stdin = [test for test in stdin_tests if test in completed]
    if completed_stdin:
        print("validity\tUNTRUSTWORTHY_STDIN_TEST_COMPLETED")
        return 1
    if len(completed) != 5877 or max_completion_ordinal != 5877:
        print("validity\tCOUNT_MISMATCH")
        return 2
    print("validity\tTRUSTWORTHY_WITH_RESPECT_TO_STDIN_DEFECT")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
