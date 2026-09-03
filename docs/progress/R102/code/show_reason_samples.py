#!/usr/bin/env python3
import csv
from pathlib import Path

root = Path(__file__).resolve().parents[3]
path = root / "progress/R102/tables/base_all_source_packages_rewritten.tsv"
with path.open(newline="", encoding="utf-8") as stream:
    rows = {row["源码包名"]: row for row in csv.DictReader(stream, delimiter="\t")}

for name in [
    "abseil-cpp", "bcc-tools", "boost", "icu", "jsoncpp", "libsigc++",
    "llvm", "pcre", "taglib", "bison", "cereal", "binutils", "elfutils",
    "db4", "ncurses", "golang", "libcxx-runtimes", "gcc", "glibc", "gmp",
    "tensorflow2", "acl", "rpm",
]:
    row = rows[name]
    print(f"[{name}] {row['判定结果']} / {row['与libstdc++的关系']}")
    print(row["理由"])
    print()
