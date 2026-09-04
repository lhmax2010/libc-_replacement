#!/usr/bin/env python3
import argparse
import csv
import os
import subprocess
from pathlib import Path


def value(text, key):
    for token in text.replace("\n", " ").split():
        if token.startswith(key + "="):
            return token.split("=", 1)[1]
    return "NOT_OBSERVED"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--boost-test-probe", required=True, type=Path)
    parser.add_argument("--boost-test-provider", required=True, type=Path)
    parser.add_argument("--boost-thread-probe", required=True, type=Path)
    parser.add_argument("--runtime-path", required=True)
    parser.add_argument("--repeat", type=int, default=5)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()
    rows = []
    cases = (
        ("boost_test_normalize", [str(args.boost_test_probe), str(args.boost_test_provider)]),
        ("boost_thread_once", [str(args.boost_thread_probe)]),
    )
    for case, command in cases:
        for run in range(1, args.repeat + 1):
            env = os.environ.copy()
            env["LD_LIBRARY_PATH"] = args.runtime_path
            try:
                proc = subprocess.run(command, env=env, text=True, stdout=subprocess.PIPE,
                                      stderr=subprocess.PIPE, timeout=15)
                rc = proc.returncode
                stdout = proc.stdout
                stderr = proc.stderr
            except subprocess.TimeoutExpired as error:
                rc = 124
                stdout = error.stdout or ""
                stderr = error.stderr or ""
            if case == "boost_test_normalize":
                concrete_value = value(stdout, "value_ok")
                destroy = value(stdout, "consumer_destroy_completed")
                provider_constructed = value(stdout, "provider_constructed")
                if rc == 0 and concrete_value == "1" and destroy == "YES":
                    outcome = "VALUE_AND_DESTROY_OK"
                elif rc == 41 and concrete_value == "0" and destroy == "YES":
                    outcome = "WRONG_VALUE_DESTROY_RETURNED"
                elif rc < 0:
                    outcome = f"SIGNAL_{-rc}"
                else:
                    outcome = "OTHER_FAILURE"
            else:
                concrete_value = value(stdout, "states")
                destroy = value(stdout, "consumer_scope_exit_completed")
                provider_constructed = "NOT_APPLICABLE_CALLER_CONSTRUCTED_FLAG"
                if rc == 0 and concrete_value == "1,1,0" and destroy == "YES":
                    outcome = "STATE_AND_SCOPE_EXIT_OK"
                elif rc < 0:
                    outcome = f"SIGNAL_{-rc}"
                else:
                    outcome = "OTHER_FAILURE"
            rows.append({
                "case": case, "run": run, "returncode": rc, "outcome": outcome,
                "concrete_value": concrete_value, "provider_constructed": provider_constructed,
                "consumer_destroy_or_scope_exit": destroy,
                "runtime_maps": value(stdout, "runtime_maps"),
                "stdout": stdout.replace("\n", "\\n"),
                "stderr": stderr.replace("\n", "\\n"),
                "command": repr(command),
            })
    fields = list(rows[0])
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    counts = {}
    for row in rows:
        key = (row["case"], row["outcome"])
        counts[key] = counts.get(key, 0) + 1
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("case", "outcome", "count"))
        for key, count in sorted(counts.items()):
            writer.writerow((*key, count))
    print(f"rows={len(rows)}")
    for key, count in sorted(counts.items()):
        print(f"{key[0]} {key[1]} {count}")
    return 0 if len(rows) == 2 * args.repeat else 2


if __name__ == "__main__":
    raise SystemExit(main())
