#!/usr/bin/env python3
import csv
import hashlib
import os
import shutil
import sys
import time
import urllib.error
import urllib.request
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path(__file__).resolve().parents[3]
manifest = ROOT / "progress/R103/tables/consumer_binary_candidates.tsv"
download_dir = ROOT / "progress/R103/downloads/consumers"
status_path = ROOT / "progress/R103/tables/consumer_download_status.tsv"
base_url = (
    "https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified-Toolchain/"
    "tizen-unified-toolchain_20260829.015247/repos/standard/packages/"
)


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


with manifest.open(newline="", encoding="utf-8") as stream:
    rows = list(csv.DictReader(stream, delimiter="\t"))

existing = {}
reuse_root = ROOT / "progress/R100/downloads"
for current, _, names in os.walk(reuse_root):
    for name in names:
        if name.endswith(".rpm"):
            existing.setdefault(name, []).append(Path(current) / name)

download_dir.mkdir(parents=True, exist_ok=True)
status_rows = []
failures = 0
for index, row in enumerate(rows, 1):
    filename = Path(row["location"]).name
    destination = download_dir / filename
    expected = row["checksum"]
    source = ""
    error = ""
    if destination.exists() and sha256(destination) == expected:
        source = "EXISTING_R103"
    else:
        if destination.exists():
            destination.unlink()
        for candidate in existing.get(filename, []):
            if sha256(candidate) == expected:
                shutil.copy2(candidate, destination)
                source = f"REUSED:{candidate.relative_to(ROOT)}"
                break
    if not source:
        url = base_url + row["location"]
        part = destination.with_suffix(destination.suffix + ".part")
        for attempt in range(1, 4):
            try:
                if part.exists():
                    part.unlink()
                with urllib.request.urlopen(url, timeout=120) as response, part.open("wb") as output:
                    shutil.copyfileobj(response, output, 1024 * 1024)
                actual = sha256(part)
                if actual != expected:
                    raise RuntimeError(f"sha256 mismatch expected={expected} actual={actual}")
                part.replace(destination)
                source = "DOWNLOADED"
                break
            except Exception as exc:
                error = f"attempt={attempt}:{type(exc).__name__}:{exc}"
                if part.exists():
                    part.unlink()
                if attempt < 3:
                    time.sleep(2)
        if not source:
            failures += 1
    actual = sha256(destination) if destination.exists() else "NOT_AVAILABLE"
    status_rows.append(
        {
            "source_name": row["source_name"],
            "binary_name": row["name"],
            "arch": row["arch"],
            "location": row["location"],
            "expected_sha256": expected,
            "actual_sha256": actual,
            "bytes": destination.stat().st_size if destination.exists() else 0,
            "source": source or "NOT_AVAILABLE",
            "error": error if not source else "",
        }
    )
    print(f"[{index}/{len(rows)}] {row['source_name']} {row['name']} {row['arch']} {source or 'FAILED'}")

fields = [
    "source_name", "binary_name", "arch", "location", "expected_sha256",
    "actual_sha256", "bytes", "source", "error",
]
with status_path.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, delimiter="\t", fieldnames=fields, lineterminator="\n")
    writer.writeheader()
    writer.writerows(status_rows)

print(f"packages={len(rows)}")
print(f"bytes={sum(int(row['bytes']) for row in status_rows)}")
print(f"failures={failures}")
raise SystemExit(1 if failures else 0)
