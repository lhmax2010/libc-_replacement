#!/usr/bin/env python3
"""Aggregate checkpointed lit results and compare patched with baseline."""

import argparse
import collections
import json
import re
from pathlib import Path


RESULT_RE = re.compile(
    r"^(PASS|FAIL|XFAIL|XPASS|UNSUPPORTED|UNRESOLVED|TIMEOUT): "
    r".*? :: (.+?) \(([0-9]+) of [0-9]+\)$"
)


def normalize(name: str) -> str:
    if " :: " in name:
        name = name.split(" :: ", 1)[1]
    for marker in ("/libcxx/test/", "/libcxxabi/test/"):
        if marker in name:
            name = name.split(marker, 1)[1]
    return name


def add(target: dict, name: str, code: str, output: str, source: str) -> None:
    name = normalize(name)
    if name in target:
        raise RuntimeError(f"duplicate result for {name}: {target[name]['source']} / {source}")
    target[name] = {"name": name, "code": code, "output": output, "source": source}


def add_stdout(target: dict, path: Path) -> None:
    for line in path.read_text(errors="replace").splitlines():
        match = RESULT_RE.match(line)
        if match:
            code, name, _ = match.groups()
            add(target, name, code, "NOT_AVAILABLE_IN_INTERRUPTED_STDOUT", str(path))


def add_json(target: dict, path: Path) -> None:
    data = json.loads(path.read_text())
    for item in data["tests"]:
        add(target, item["name"], item["code"], item.get("output", ""), str(path))


def write_result(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps({"tests": [data[k] for k in sorted(data)]}, indent=2) + "\n")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--expected-tests", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--old-stdout", type=Path)
    parser.add_argument("--gate-json", type=Path)
    parser.add_argument("--batch-root", type=Path, required=True)
    parser.add_argument("--compare", type=Path)
    parser.add_argument("--comparison-output", type=Path)
    args = parser.parse_args()

    expected = [line for line in args.expected_tests.read_text().splitlines() if line]
    if len(expected) != len(set(expected)):
        raise RuntimeError("expected test list contains duplicates")
    results = {}
    if args.old_stdout:
        add_stdout(results, args.old_stdout)
    if args.gate_json:
        add_json(results, args.gate_json)
    complete_dirs = sorted(p.parent for p in args.batch_root.glob("batch-*/COMPLETE.tsv"))
    for directory in complete_dirs:
        add_json(results, directory / "result.json")

    missing = sorted(set(expected) - set(results))
    extra = sorted(set(results) - set(expected))
    counts = collections.Counter(item["code"] for item in results.values())
    summary = {
        "expected": len(expected),
        "observed": len(results),
        "complete_batches": len(complete_dirs),
        "missing": len(missing),
        "extra": len(extra),
        "codes": dict(sorted(counts.items())),
    }
    write_result(args.output, results)
    (args.output.parent / "aggregate_summary.json").write_text(
        json.dumps(summary, indent=2) + "\n"
    )
    (args.output.parent / "missing_tests.txt").write_text("\n".join(missing) + ("\n" if missing else ""))
    (args.output.parent / "extra_tests.txt").write_text("\n".join(extra) + ("\n" if extra else ""))
    print(json.dumps(summary, sort_keys=True))

    if args.compare:
        other_data = json.loads(args.compare.read_text())
        other = {normalize(item["name"]): item for item in other_data["tests"]}
        names = sorted(set(results) | set(other))
        rows = ["test\tpatched\tbaseline\trelation\n"]
        relations = collections.Counter()
        fail_codes = {"FAIL", "XPASS", "UNRESOLVED", "TIMEOUT"}
        for name in names:
            patched = results.get(name)
            baseline = other.get(name)
            pc = patched["code"] if patched else "MISSING"
            bc = baseline["code"] if baseline else "MISSING"
            if pc == bc:
                relation = "SAME"
            elif bc == "PASS" and pc in fail_codes:
                relation = "INTRODUCED_CANDIDATE"
            elif pc in fail_codes and bc in fail_codes:
                relation = "BOTH_FAIL_DIFFERENT_CODE"
            else:
                relation = "DIFFERENT_OTHER"
            relations[relation] += 1
            rows.append(f"{name}\t{pc}\t{bc}\t{relation}\n")
        if not args.comparison_output:
            raise RuntimeError("--comparison-output is required with --compare")
        args.comparison_output.write_text("".join(rows))
        print(json.dumps({"relations": dict(sorted(relations.items()))}, sort_keys=True))

    if missing or extra:
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
