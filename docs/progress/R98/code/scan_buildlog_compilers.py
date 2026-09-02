#!/usr/bin/env python3
"""Extract the effective project-level CC/CXX selection from all published logs."""

import csv
import re
from concurrent.futures import ThreadPoolExecutor, as_completed
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import urljoin
from urllib.request import urlopen

ROOT = "https://download.tizen.org/snapshots/TIZEN/Tizen"
PROJECTS = {
    "base": ("Tizen-Base-Toolchain", "tizen-base-toolchain_20260828.101647"),
    "unified": ("Tizen-Unified-Toolchain", "tizen-unified-toolchain_20260829.015247"),
}
ARCHES = ("armv7l", "aarch64", "x86_64")
LIMIT = 2 * 1024 * 1024
EXPORT = re.compile(rb"\+ export CC=([^\s]+) CXX=([^\s]+)")
CID = re.compile(rb"The C compiler identification is ([^\r\n]+)")
CXXID = re.compile(rb"The CXX compiler identification is ([^\r\n]+)")


class Links(HTMLParser):
    def __init__(self):
        super().__init__()
        self.hrefs = []

    def handle_starttag(self, tag, attrs):
        if tag == "a":
            href = dict(attrs).get("href")
            if href and href.endswith(".buildlog.txt"):
                self.hrefs.append(href)


def get(url, limit=None):
    with urlopen(url, timeout=90) as response:
        return response.read() if limit is None else response.read(limit)


jobs = []
for label, (project, snapshot) in PROJECTS.items():
    for arch in ARCHES:
        directory = f"{ROOT}/{project}/{snapshot}/builddata/buildlogs/standard/{arch}/succeeded/"
        parser = Links()
        parser.feed(get(directory).decode("utf-8", "replace"))
        for href in parser.hrefs:
            jobs.append((label, snapshot, arch, href.removesuffix(".buildlog.txt"), urljoin(directory, href)))


def inspect(job):
    label, snapshot, arch, package, url = job
    try:
        data = get(url, LIMIT)
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
        return {
            "project": label, "snapshot": snapshot, "arch": arch,
            "source_package": package, "compiler_selection": compiler,
            "cc": cc, "cxx": cxx,
            "c_compiler_id": cid.group(1).decode("utf-8", "replace") if cid else "NOT_OBSERVED",
            "cxx_compiler_id": cxxid.group(1).decode("utf-8", "replace") if cxxid else "NOT_OBSERVED",
            "evidence": evidence, "bytes_scanned": str(len(data)), "url": url,
            "fetch_result": "OK",
        }
    except Exception as exc:
        return {
            "project": label, "snapshot": snapshot, "arch": arch,
            "source_package": package, "compiler_selection": "NOT_AVAILABLE",
            "cc": "NOT_AVAILABLE", "cxx": "NOT_AVAILABLE",
            "c_compiler_id": "NOT_AVAILABLE", "cxx_compiler_id": "NOT_AVAILABLE",
            "evidence": f"{type(exc).__name__}: {exc}", "bytes_scanned": "0", "url": url,
            "fetch_result": "ERROR",
        }


rows = []
with ThreadPoolExecutor(max_workers=8) as pool:
    futures = [pool.submit(inspect, job) for job in jobs]
    for index, future in enumerate(as_completed(futures), 1):
        rows.append(future.result())
        if index % 250 == 0:
            print(f"progress={index}/{len(jobs)}")

rows.sort(key=lambda row: (row["project"], row["source_package"], row["arch"]))
fields = list(rows[0])
output = Path("progress/R98/tables/buildlog_compiler_evidence.tsv")
with output.open("w", encoding="utf-8", newline="") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)

print(f"jobs={len(jobs)}")
for project in PROJECTS:
    subset = [row for row in rows if row["project"] == project]
    print(f"{project}_records={len(subset)}")
    for status in ("CLANG", "GCC", "OTHER", "NOT_OBSERVED", "NOT_AVAILABLE"):
        names = sorted({row["source_package"] for row in subset if row["compiler_selection"] == status})
        print(f"{project}_{status}_records={sum(row['compiler_selection'] == status for row in subset)}")
        print(f"{project}_{status}_names={len(names)}")
        if status != "CLANG":
            print(f"{project}_{status}_name_list={';'.join(names)}")
