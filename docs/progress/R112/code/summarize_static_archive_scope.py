#!/usr/bin/env python3
import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
TABLE = ROOT / "docs/progress/R108/tables/w2_static_inventory.tsv"

def main() -> int:
    with TABLE.open(newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    published = [row for row in rows if row["class"] == "PUBLISHED_STATIC_ARCHIVE"]
    libcxx = [row for row in published if Path(row["path"]).name == "libc++.a"]
    libstdcxx = [row for row in published if Path(row["path"]).name == "libstdc++.a"]
    print("published_static_archives", len(published))
    print("cpp_indicator_yes", sum(row["cpp_indicator"] == "YES" for row in published))
    print("libcxx_a_paths", len(libcxx))
    print("libstdcxx_a_positive_control_paths", len(libstdcxx))
    for row in libstdcxx[:3]:
        print("positive_control", row["package"], row["arch"], row["path"])
    assert len(published) == 8911
    assert libstdcxx
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
