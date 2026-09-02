#!/usr/bin/env python3
"""Retry transient build-log fetch failures and update the evidence table."""

import csv
import re
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
from urllib.request import urlopen

PATH = Path("progress/R98/tables/buildlog_compiler_evidence.tsv")
LIMIT = 2 * 1024 * 1024
EXPORT = re.compile(rb"\+ export CC=([^\s]+) CXX=([^\s]+)")
CID = re.compile(rb"The C compiler identification is ([^\r\n]+)")
CXXID = re.compile(rb"The CXX compiler identification is ([^\r\n]+)")

with PATH.open(encoding="utf-8", newline="") as stream:
    rows = list(csv.DictReader(stream, delimiter="\t"))
fields = list(rows[0])


def retry(index):
    row = dict(rows[index])
    errors = []
    for attempt in range(1, 4):
        try:
            with urlopen(row["url"], timeout=180) as response:
                data = response.read(LIMIT)
            export = EXPORT.search(data)
            cid = CID.search(data)
            cxxid = CXXID.search(data)
            if export:
                cc = export.group(1).decode("utf-8", "replace")
                cxx = export.group(2).decode("utf-8", "replace")
                if "clang" in cc and "clang" in cxx:
                    compiler = "CLANG"
                elif re.search(r"(^|-)gcc$", cc) and re.search(r"(^|-)g\+\+$", cxx):
                    compiler = "GCC"
                else:
                    compiler = "OTHER"
                evidence = export.group(0).decode("utf-8", "replace")
            else:
                cc = cxx = evidence = "NOT_OBSERVED"
                compiler = "NOT_OBSERVED"
            row.update({
                "compiler_selection": compiler, "cc": cc, "cxx": cxx,
                "c_compiler_id": cid.group(1).decode("utf-8", "replace") if cid else "NOT_OBSERVED",
                "cxx_compiler_id": cxxid.group(1).decode("utf-8", "replace") if cxxid else "NOT_OBSERVED",
                "evidence": evidence, "bytes_scanned": str(len(data)), "fetch_result": "OK_RETRY",
            })
            return index, row, attempt
        except Exception as exc:
            errors.append(f"attempt{attempt}:{type(exc).__name__}:{exc}")
            time.sleep(attempt)
    row["evidence"] = "; ".join(errors)
    row["fetch_result"] = "ERROR_AFTER_3_RETRIES"
    return index, row, 3


targets = [i for i, row in enumerate(rows) if row["compiler_selection"] == "NOT_AVAILABLE"]
with ThreadPoolExecutor(max_workers=4) as pool:
    futures = [pool.submit(retry, index) for index in targets]
    for future in as_completed(futures):
        index, row, attempts = future.result()
        rows[index] = row
        print(f"{row['fetch_result']}\t{row['project']}\t{row['arch']}\t{row['source_package']}\t"
              f"{row['compiler_selection']}\tattempts={attempts}")

with PATH.open("w", encoding="utf-8", newline="") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)

print(f"retried={len(targets)}")
print(f"remaining_not_available={sum(r['compiler_selection'] == 'NOT_AVAILABLE' for r in rows)}")
