#!/usr/bin/env python3
import argparse
import csv
import os
import subprocess
from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--probe", required=True, type=Path)
    parser.add_argument("--provider", required=True, type=Path)
    parser.add_argument("--repeat", type=int, default=5)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    rows = []
    for run in range(1, args.repeat + 1):
        proc = subprocess.run([str(args.probe), str(args.provider)], env=os.environ.copy(),
                              text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                              timeout=15)
        stdout = proc.stdout
        rows.append({
            "run": run, "returncode": proc.returncode,
            "value_ok": "YES" if "value_ok=1" in stdout else "NO",
            "provider_constructed": "YES" if "provider_constructed=YES" in stdout else "NO",
            "consumer_destroy_completed": "YES" if "consumer_destroy_completed=YES" in stdout else "NO",
            "stdout": stdout.replace("\n", "\\n"),
            "stderr": proc.stderr.replace("\n", "\\n"),
        })
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, list(rows[0]), delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    print(f"rows={len(rows)}")
    print(f"value_ok={sum(r['value_ok'] == 'YES' for r in rows)}")
    print(f"destroy_ok={sum(r['consumer_destroy_completed'] == 'YES' for r in rows)}")
    return 0 if all(r["returncode"] == 0 and r["value_ok"] == "YES" and r["consumer_destroy_completed"] == "YES" for r in rows) else 2


if __name__ == "__main__":
    raise SystemExit(main())
