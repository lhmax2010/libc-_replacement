#!/usr/bin/env python3
"""Fetch selected primary QuickBuild logs for compiler-marker inspection."""

from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import urljoin
from urllib.request import urlopen

ROOT = "https://download.tizen.org/snapshots/TIZEN/Tizen"
PROJECTS = {
    "base": ("Tizen-Base-Toolchain", "tizen-base-toolchain_20260828.101647"),
    "unified": ("Tizen-Unified-Toolchain", "tizen-unified-toolchain_20260829.015247"),
}
TARGETS = {
    "base": {
        "abseil-cpp", "bcc-tools", "binutils", "boost", "bpftrace", "ccache",
        "cmake", "elfutils", "gcc", "gdb", "jsoncpp", "llvm", "openfst",
    },
    "unified": {"angle", "boost", "bpftrace", "gdb", "llvm", "openfst", "re2"},
}
ARCHES = ("armv7l", "aarch64", "x86_64")


class Links(HTMLParser):
    def __init__(self):
        super().__init__()
        self.hrefs = []

    def handle_starttag(self, tag, attrs):
        if tag == "a":
            value = dict(attrs).get("href")
            if value:
                self.hrefs.append(value)


def read(url):
    with urlopen(url, timeout=60) as response:
        return response.read()


out_root = Path("progress/R98/inputs/buildlogs")
for label, (project, snapshot) in PROJECTS.items():
    for arch in ARCHES:
        base = f"{ROOT}/{project}/{snapshot}/builddata/buildlogs/standard/{arch}/succeeded/"
        parser = Links()
        parser.feed(read(base).decode("utf-8", "replace"))
        available = {href.removesuffix(".buildlog.txt"): href for href in parser.hrefs
                     if href.endswith(".buildlog.txt")}
        for target in sorted(TARGETS[label]):
            if target not in available:
                print(f"NOT_AVAILABLE\t{label}\t{arch}\t{target}")
                continue
            destination = out_root / label / arch / available[target]
            destination.parent.mkdir(parents=True, exist_ok=True)
            payload = read(urljoin(base, available[target]))
            destination.write_bytes(payload)
            print(f"FETCHED\t{label}\t{arch}\t{target}\t{len(payload)}\t{destination}")
