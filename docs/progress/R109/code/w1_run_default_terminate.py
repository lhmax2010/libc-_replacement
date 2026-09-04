#!/usr/bin/env python3
import argparse
import csv
import os
import subprocess
from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--probe", required=True, type=Path)
    parser.add_argument("--lib-dir", required=True, type=Path)
    parser.add_argument("--injector", required=True, type=Path)
    parser.add_argument("--repeat", required=True, type=int)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    rows = []
    for mode in ("unowned", "backend"):
        for run in range(1, args.repeat + 1):
            env = os.environ.copy()
            env["LD_LIBRARY_PATH"] = str(args.lib_dir)
            if mode == "backend":
                env["LD_PRELOAD"] = str(args.injector)
            completed = subprocess.run(
                [str(args.probe), mode], env=env, text=True,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=10,
            )
            rows.append((mode, run, completed.returncode,
                         completed.stdout.replace("\n", "\\n"),
                         completed.stderr.replace("\n", "\\n")))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("mode", "run", "returncode", "stdout", "stderr"))
        writer.writerows(rows)
    print(f"ROWS={len(rows)}")
    print(f"SIGABRT={sum(row[2] == -6 for row in rows)}")
    return 0 if len(rows) == 2 * args.repeat and all(row[2] == -6 for row in rows) else 2


if __name__ == "__main__":
    raise SystemExit(main())
