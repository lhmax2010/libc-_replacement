#!/usr/bin/env python3
"""Print bounded diagnostics for failed static-archive batches."""

import csv
from pathlib import Path


rows = list(csv.reader(
    Path("progress/R87/tables/static_archive_summary.tsv").open(), delimiter="\t"
))
paths = Path("progress/R87/tables/static_archive_paths.txt").read_text().splitlines()
failed_batch_paths = []
for key, value in rows:
    if not key.startswith("FAILURE:BATCH:"):
        continue
    start = int(key.rsplit(":", 1)[1])
    batch = paths[start:start + 20]
    failed_batch_paths.extend(batch)
    print(f"=== {key} batch_paths={len(batch)} stderr_chars={len(value)} ===")
    print("\n".join(batch))
    print("--- first diagnostic lines ---")
    print("\n".join(value.splitlines()[:12]))

Path("progress/R87/tables/static_archive_failed_batch_paths.txt").write_text(
    "\n".join(failed_batch_paths) + "\n"
)
