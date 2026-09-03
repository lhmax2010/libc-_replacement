#!/usr/bin/env python3
import csv
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
R101 = ROOT / "docs/progress/R101/tables/base_all_source_packages.tsv"
R100 = ROOT / "docs/progress/R100/tables/base_cpp_package_decisions.tsv"


def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


inventory = read_tsv(R101)
decisions = read_tsv(R100)
by_name = {row["source_package"]: row for row in decisions}
print(f"inventory_rows={len(inventory)} unique={len({r['源码包名'] for r in inventory})}")
print("decision_counts", dict(Counter(r["判定结果"] for r in inventory)))
print("cpp_counts", dict(Counter(r["是否含C++及依据"].split("：", 1)[0] for r in inventory)))
print("relation_counts", dict(Counter(r["与libstdc++的关系"] for r in inventory)))
print("\nR100 matched-rule groups:")
groups = defaultdict(list)
for row in inventory:
    r100 = by_name.get(row["源码包名"])
    if r100:
        groups[(row["判定结果"], r100["matched_rules"])].append(row["源码包名"])
for key, names in sorted(groups.items()):
    print(f"{key[0]}\t{key[1]}\t{len(names)}\t{';'.join(names)}")

print("\nNon-C++ image/tool split:")
non_cpp = [r for r in inventory if r["是否含C++及依据"].startswith("否：")]
for key, rows in sorted(
    defaultdict(list, {
        (image, tool): [r for r in non_cpp if r["是否进入设备镜像"].startswith(image) and r["是否为测试诊断基准类工具"].startswith(tool)]
        for image in ("是", "否") for tool in ("是", "否")
    }).items()
):
    print(key, len(rows))

print("\nNeed rows evidence:")
for row in inventory:
    if row["判定结果"] == "需要改":
        r100 = by_name[row["源码包名"]]
        print(
            row["源码包名"],
            "unified=", r100["unified_runtime_edge_classes"],
            "consumers=", r100["unified_buildrequire_consumers"],
            "internal=", r100["base_internal_edge_classes_as_consumer"],
            sep="\t",
        )
