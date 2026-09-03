#!/usr/bin/env python3
import csv
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
CODE_ROOT = ROOT / "codes" / "R104"
OUT = ROOT / "progress" / "R104" / "tables" / "remote_heads_before_branch.tsv"

rows = []
for path in sorted(p for p in CODE_ROOT.iterdir() if (p / ".git").is_dir()):
    url = subprocess.check_output(
        ["git", "-C", str(path), "remote", "get-url", "origin"], text=True
    ).strip()
    output = subprocess.check_output(["git", "ls-remote", "--heads", url], text=True)
    for line in output.splitlines():
        sha, ref = line.split("\t", 1)
        rows.append({"package": path.name, "remote_url": url, "ref": ref, "sha": sha})

OUT.parent.mkdir(parents=True, exist_ok=True)
with OUT.open("w", newline="", encoding="utf-8") as f:
    writer = csv.DictWriter(
        f, fieldnames=["package", "remote_url", "ref", "sha"],
        delimiter="\t", lineterminator="\n",
    )
    writer.writeheader()
    writer.writerows(rows)

for package in sorted({r["package"] for r in rows}):
    count = sum(r["package"] == package for r in rows)
    print(f"{package}\tremote_heads={count}")
print(f"total_remote_heads={len(rows)}")
