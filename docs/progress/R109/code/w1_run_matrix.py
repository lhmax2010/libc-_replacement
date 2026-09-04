#!/usr/bin/env python3
import argparse
import csv
import json
import os
import subprocess
from collections import Counter
from pathlib import Path


def classify(returncode, stdout, timed_out):
    if timed_out:
        return "TIMEOUT"
    if returncode == 86 and "event=terminate" in stdout:
        return "TERMINATE"
    if returncode == 42 and "event=system_error" in stdout:
        return "SYSTEM_ERROR"
    if returncode == 0 and "event=returned" in stdout:
        return "RETURNED"
    return "OTHER"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--cell", action="append", required=True,
                        help="name,binary,runtime")
    parser.add_argument("--injector", required=True)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    parser.add_argument("--repeat", type=int, default=5)
    args = parser.parse_args()
    rows = []
    counts = Counter()
    for cell_spec in args.cell:
        cell, binary, runtime, overlay = cell_spec.split(",", 3)
        for mode in ("unowned", "backend"):
            for iteration in range(1, args.repeat + 1):
                env = os.environ.copy()
                if runtime != "SYSTEM":
                    env["LD_LIBRARY_PATH"] = runtime
                preloads = []
                if mode == "backend":
                    preloads.append(args.injector)
                if overlay != "NONE":
                    preloads.append(overlay)
                if preloads:
                    env["LD_PRELOAD"] = ":".join(preloads)
                else:
                    env.pop("LD_PRELOAD", None)
                command = [binary, mode]
                timed_out = False
                try:
                    proc = subprocess.run(command, env=env, text=True,
                                          stdout=subprocess.PIPE,
                                          stderr=subprocess.PIPE, timeout=3)
                    returncode, stdout, stderr = proc.returncode, proc.stdout, proc.stderr
                except subprocess.TimeoutExpired as error:
                    timed_out = True
                    returncode = "TIMEOUT"
                    stdout = error.stdout or ""
                    stderr = error.stderr or ""
                    if isinstance(stdout, bytes): stdout = stdout.decode(errors="replace")
                    if isinstance(stderr, bytes): stderr = stderr.decode(errors="replace")
                outcome = classify(returncode, stdout, timed_out)
                injector_seen = "YES" if "injector=called" in stderr else "NO"
                rows.append((cell, mode, iteration, json.dumps(command),
                             runtime, "YES" if mode == "backend" else "NO",
                             returncode, outcome, injector_seen,
                             stdout.replace("\n", "\\n"), stderr.replace("\n", "\\n")))
                counts[(cell, mode, outcome)] += 1
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("cell", "mode", "iteration", "command_json", "runtime",
                         "injection_requested", "exit", "outcome", "injector_seen",
                         "stdout_escaped", "stderr_escaped"))
        writer.writerows(rows)
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("cell", "mode", "outcome", "count"))
        for key, count in sorted(counts.items()):
            writer.writerow((*key, count))
            print(".".join(key) + f"={count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
