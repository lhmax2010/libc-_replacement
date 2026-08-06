#!/usr/bin/env python3
import argparse
import concurrent.futures
import csv
import json
import os
import pathlib
import re
import shlex
import subprocess
import threading


CLANGXX_RE = re.compile(r"(?<![A-Za-z0-9_+.-])clang\+\+(?![A-Za-z0-9_+.-])")
CLANG_RE = re.compile(r"(?<![A-Za-z0-9_+.-])clang(?![A-Za-z0-9_+.-])")
TOKENS = {
    "clang++": CLANGXX_RE,
    "clang": CLANG_RE,
    "dlopen": re.compile(r"\bdlopen\b"),
    "RTLD_LOCAL": re.compile(r"\bRTLD_LOCAL\b"),
    "RTLD_GLOBAL": re.compile(r"\bRTLD_GLOBAL\b"),
}


def clean(value):
    return value.replace("\t", " ").replace("\r", " ").replace("\n", "\\n")


def one(row, extract_root, shard_root, sequence, total, lock):
    key = row["checksum"]
    out_dir = pathlib.Path(extract_root) / key[:2] / key
    out_dir.mkdir(parents=True, exist_ok=False)
    rpm = pathlib.Path(row["target"]).resolve()
    command = f"cd {shlex.quote(str(out_dir))} && rpm2cpio {shlex.quote(str(rpm))} | cpio -idm --quiet '*.spec'"
    proc = subprocess.run(
        ["bash", "-o", "pipefail", "-c", f"rpm2cpio {shlex.quote(str(rpm))} | cpio -idm --quiet '*.spec'"],
        cwd=out_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
    )
    result = {
        "package": row,
        "command": command,
        "exit_code": str(proc.returncode),
        "stderr": clean(proc.stderr),
        "specs": [],
        "hits": [],
    }
    if proc.returncode == 0:
        for spec in sorted(out_dir.rglob("*.spec")):
            relpath = str(spec.relative_to(out_dir))
            try:
                text = spec.read_text(encoding="utf-8", errors="replace")
            except OSError as error:
                result["specs"].append({"path": relpath, "read_result": f"FAILED:{clean(str(error))}"})
                continue
            result["specs"].append({"path": relpath, "read_result": "PASS"})
            for lineno, line in enumerate(text.splitlines(), 1):
                for token, pattern in TOKENS.items():
                    if pattern.search(line):
                        result["hits"].append({
                            "spec_path": relpath, "line": str(lineno), "token": token,
                            "original": line.replace("\t", "\\t"),
                        })
    shard = pathlib.Path(shard_root) / f"{key}.json"
    with open(shard, "w", encoding="utf-8") as stream:
        json.dump(result, stream, ensure_ascii=False, sort_keys=True)
        stream.write("\n")
    with lock:
        sequence[0] += 1
        if sequence[0] % 50 == 0 or sequence[0] == total:
            print(f"PROGRESS={sequence[0]}/{total}", flush=True)


def write_tsv(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        writer.writeheader(); writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--download-status", required=True)
    ap.add_argument("--extract-root", required=True)
    ap.add_argument("--shard-root", required=True)
    ap.add_argument("--table-root", required=True)
    ap.add_argument("--ledger", required=True)
    ap.add_argument("--jobs", type=int, default=12)
    args = ap.parse_args()
    with open(args.download_status, encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    extract_root = pathlib.Path(args.extract_root)
    shard_root = pathlib.Path(args.shard_root)
    extract_root.mkdir(parents=True, exist_ok=False)
    shard_root.mkdir(parents=True, exist_ok=False)
    sequence = [0]
    lock = threading.Lock()
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = [pool.submit(one, row, extract_root, shard_root, sequence, len(rows), lock) for row in rows]
        for future in concurrent.futures.as_completed(futures): future.result()
    specs, hits, status, commands = [], [], [], []
    pfx_fields = ["repo_id", "name", "arch", "epoch", "version", "release", "checksum", "location"]
    for shard in sorted(shard_root.glob("*.json")):
        data = json.loads(shard.read_text(encoding="utf-8"))
        pkg = data["package"]
        pfx = {field: pkg[field] for field in pfx_fields}
        result = "PASS" if data["exit_code"] == "0" and data["specs"] else ("NO_SPEC_FOUND" if data["exit_code"] == "0" else "EXTRACT_FAILED")
        status.append({**pfx, "exit_code": data["exit_code"], "result": result, "spec_count": len(data["specs"]), "stderr": data["stderr"]})
        specs.extend({**pfx, **row} for row in data["specs"])
        hits.extend({**pfx, **row} for row in data["hits"])
        commands.append({**pfx, "command": data["command"], "exit_code": data["exit_code"], "stderr": data["stderr"]})
    root = pathlib.Path(args.table_root)
    write_tsv(root / "source_spec_extraction_status.tsv", pfx_fields + ["exit_code", "result", "spec_count", "stderr"], status)
    write_tsv(root / "source_spec_inventory.tsv", pfx_fields + ["path", "read_result"], specs)
    write_tsv(root / "source_spec_text_hits.tsv", pfx_fields + ["spec_path", "line", "token", "original"], hits)
    hit_tokens = {}
    for row in hits:
        hit_tokens.setdefault(row["checksum"], set()).add(row["token"])
    summary = []
    for row in status:
        tokens = hit_tokens.get(row["checksum"], set())
        if row["result"] != "PASS": category = "NOT_AVAILABLE_SPEC_EXTRACTION"
        elif "clang++" in tokens and "clang" in tokens: category = "BOTH_CLANG_AND_CLANGXX_TEXT"
        elif "clang++" in tokens: category = "CLANGXX_TEXT_ONLY"
        elif "clang" in tokens: category = "CLANG_TEXT_ONLY"
        else: category = "NO_LITERAL_CLANG_DRIVER_TEXT"
        summary.append({**{field: row[field] for field in pfx_fields}, "category": category})
    write_tsv(root / "source_spec_link_driver_text_classification.tsv", pfx_fields + ["category"], summary)
    with open(args.ledger, "w", encoding="utf-8") as out:
        for i, row in enumerate(commands, 1):
            out.write(f"LABEL=source_spec_extract_{i:05d}\n")
            out.write(f"SOURCE_PACKAGE={row['name']}-{row['version']}-{row['release']}\nPWD={pathlib.Path.cwd()}\n")
            out.write(f"COMMAND={row['command']}\n")
            if row["stderr"]: out.write(f"STDERR={row['stderr']}\n")
            out.write(f"EXIT_CODE={row['exit_code']}\n\n")
        out.write(f"COMMAND_COUNT={len(commands)}\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
