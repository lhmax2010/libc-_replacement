#!/usr/bin/env python3
import csv
import re
from collections import Counter
from pathlib import Path

root = Path(__file__).resolve().parents[3]
path = root / "docs/progress/R101/tables/base_all_source_packages.tsv"
with path.open(newline="", encoding="utf-8") as stream:
    rows = list(csv.DictReader(stream, delimiter="\t"))

missing = []
for row in rows:
    match = re.search(r"(?:^|；)Summary=(.*)$", row["证据"])
    if not match or not match.group(1).strip():
        missing.append(row["源码包名"])

print(f"rows={len(rows)}")
print(f"summary_present={len(rows) - len(missing)}")
print(f"summary_missing={len(missing)}")
print("missing_names=" + (";".join(missing) or "NONE"))
print(
    "direct_libstdcpp_by_decision="
    + str(
        dict(
            Counter(
                row["判定结果"]
                for row in rows
                if row["与libstdc++的关系"] == "直接链接 libstdc++"
            )
        )
    )
)
print(
    "no_decision_image_yes="
    + str(
        sum(
            row["是否进入设备镜像"].startswith("是：")
            for row in rows
            if row["判定结果"] == "不需要改"
        )
    )
)
