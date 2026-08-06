#!/usr/bin/env python3
import argparse
import concurrent.futures
import csv
import hashlib
import os
import pathlib
import shlex
import subprocess
import sys
import threading


def digest(path):
    h = hashlib.sha256()
    with open(path, "rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--manifest", required=True)
    ap.add_argument("--repositories", required=True)
    ap.add_argument("--output-root", required=True)
    ap.add_argument("--status", required=True)
    ap.add_argument("--ledger", required=True)
    ap.add_argument("--jobs", type=int, default=16)
    args = ap.parse_args()

    repos = {}
    with open(args.repositories, encoding="utf-8") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            repos[row["repo_id"]] = row["base_url"].rstrip("/")
    with open(args.manifest, encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    root = pathlib.Path(args.output_root)
    root.mkdir(parents=True, exist_ok=False)
    counter = 0
    lock = threading.Lock()

    def one(row):
        target = root / row["repo_id"] / row["location"]
        target.parent.mkdir(parents=True, exist_ok=True)
        part = pathlib.Path(str(target) + ".part")
        url = repos[row["repo_id"]] + "/" + row["location"]
        cmd = [
            "curl", "-fsSL", "--retry", "3", "--retry-all-errors",
            "--retry-delay", "2", "--connect-timeout", "20",
            "--output", str(part), url,
        ]
        proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        actual = ""
        result = "DOWNLOAD_FAILED"
        if proc.returncode == 0:
            actual = digest(part)
            if actual == row["checksum"]:
                os.replace(part, target)
                result = "PASS"
            else:
                result = "CHECKSUM_MISMATCH"
        with lock:
            nonlocal counter
            counter += 1
            if counter % 250 == 0 or counter == len(rows):
                print(f"PROGRESS={counter}/{len(rows)}", flush=True)
        return {
            **row,
            "target": str(target),
            "url": url,
            "command": shlex.join(cmd),
            "exit_code": str(proc.returncode),
            "actual_sha256": actual,
            "result": result,
            "stderr": proc.stderr.replace("\t", " ").replace("\r", " ").replace("\n", "\\n"),
        }

    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        results = list(pool.map(one, rows))
    results.sort(key=lambda r: (r["repo_id"], r["arch"], r["name"], r["version"], r["release"]))
    fields = [
        "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm",
        "location", "package_size", "checksum", "target", "actual_sha256",
        "exit_code", "result", "url",
    ]
    with open(args.status, "w", newline="", encoding="utf-8") as out:
        w = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        w.writeheader()
        w.writerows(results)
    with open(args.ledger, "w", encoding="utf-8") as out:
        for i, row in enumerate(results, 1):
            out.write(f"LABEL=download_rpm_{i:05d}\n")
            out.write(f"PWD={os.getcwd()}\n")
            out.write(f"COMMAND={row['command']}\n")
            if row["stderr"]:
                out.write(f"STDERR={row['stderr']}\n")
            out.write(f"EXIT_CODE={row['exit_code']}\n")
            out.write(f"RESULT={row['result']}\n\n")
        failures = sum(r["result"] != "PASS" for r in results)
        out.write(f"TOTAL={len(results)}\nFAILURES={failures}\nOVERALL_EXIT_CODE={0 if failures == 0 else 1}\n")
    failures = sum(r["result"] != "PASS" for r in results)
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
