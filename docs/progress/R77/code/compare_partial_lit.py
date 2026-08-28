#!/usr/bin/env python3
import argparse
import collections
import json
from pathlib import Path


def normalize(name: str) -> str:
    marker = " :: "
    if marker in name:
        name = name.split(marker, 1)[1]
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
    ap.add_argument("--summary", type=Path, required=True)
    args = ap.parse_args()
    patched = load(args.patched)
    baseline = load(args.baseline_partial)
    fail_codes = {"FAIL", "XPASS", "UNRESOLVED", "TIMEOUT"}
    counts = collections.Counter()
    rows = ["test\tpatched\tbaseline\trelation\n"]
    for name in sorted(baseline):
        pc = patched.get(name, {}).get("code", "MISSING")
        bc = baseline[name]["code"]
        if pc == bc:
            relation = "SAME"
        elif bc == "PASS" and pc in fail_codes:
            relation = "INTRODUCED_CANDIDATE"
        elif pc == "PASS" and bc in fail_codes:
            relation = "BASELINE_ONLY_FAILURE"
        elif pc in fail_codes and bc in fail_codes:
            relation = "BOTH_FAIL_DIFFERENT_CODE"
        else:
            relation = "DIFFERENT_OTHER"
        counts[relation] += 1
        rows.append(f"{name}\t{pc}\t{bc}\t{relation}\n")
    summary = {
        "baseline_observed": len(baseline),
        "patched_total": len(patched),
        "relations": dict(sorted(counts.items())),
    }
    args.output.write_text("".join(rows))
    args.summary.write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n")
    print(json.dumps(summary, sort_keys=True))
    return 0 if "MISSING" not in {row.split("\t")[1] for row in rows[1:]} else 2


if __name__ == "__main__":
    raise SystemExit(main())
