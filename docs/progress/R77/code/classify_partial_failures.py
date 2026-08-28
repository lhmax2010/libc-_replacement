#!/usr/bin/env python3
import argparse
import json
from pathlib import Path


FAIL_CODES = {"FAIL", "XPASS", "UNRESOLVED", "TIMEOUT"}


def normalize(name: str) -> str:
    name = name.split(" :: ", 1)[-1]
    for prefix in (
        "/home/toolchain/development/libc++_replacement/tmp/R68/source-baseline/libcxx/test/",
        "/home/toolchain/development/libc++_replacement/tmp/R69/replay/libcxx/test/",
    ):
        if name.startswith(prefix):
            return name[len(prefix):]
    return name


def load(path: Path):
    data = json.loads(path.read_text())
    return {normalize(item["name"]): item for item in data["tests"]}


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--patched", type=Path, required=True)
    ap.add_argument("--baseline-partial", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    ap.add_argument("--baseline-only", type=Path, required=True)
    args = ap.parse_args()
    patched = load(args.patched)
    baseline = load(args.baseline_partial)
    rows = ["test\tpatched_code\tbaseline_code\tclassification\tevidence\n"]
    for name in sorted(patched):
        pc = patched[name]["code"]
        if pc not in FAIL_CODES:
            continue
        if name not in baseline:
            bc = "NOT_OBSERVED"
            classification = "INDETERMINATE"
            evidence = "baseline batch was not reached before SDB connection failure"
        else:
            bc = baseline[name]["code"]
            if bc in FAIL_CODES:
                classification = "UPSTREAM_EXISTING"
                evidence = "baseline and patched both fail; exact codes recorded"
            elif bc == "PASS":
                classification = "INTRODUCED_CANDIDATE"
                evidence = "baseline PASS and patched failure"
            else:
                classification = "INDETERMINATE"
                evidence = "baseline status is not PASS or a failure status"
        rows.append(f"{name}\t{pc}\t{bc}\t{classification}\t{evidence}\n")
    args.output.write_text("".join(rows))

    base_rows = ["test\tpatched_code\tbaseline_code\trelation\n"]
    for name in sorted(baseline):
        bc = baseline[name]["code"]
        pc = patched.get(name, {}).get("code", "MISSING")
        if bc in FAIL_CODES and pc == "PASS":
            base_rows.append(f"{name}\t{pc}\t{bc}\tBASELINE_ONLY_FAILURE\n")
    args.baseline_only.write_text("".join(base_rows))
    print(f"PATCHED_FAILURE_ROWS={len(rows)-1}")
    print(f"BASELINE_ONLY_FAILURE_ROWS={len(base_rows)-1}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
