#!/usr/bin/env python3
import csv
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
DOC = ROOT / "docs/progress/R101"


def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


inventory = read_tsv(DOC / "tables/base_all_source_packages.tsv")
assert len(inventory) == 254, len(inventory)
assert len({row["源码包名"] for row in inventory}) == 254
assert list(inventory[0]) == [
    "源码包名",
    "是否含C++及依据",
    "与libstdc++的关系",
    "判定结果",
    "理由",
    "证据",
    "是否进入设备镜像",
    "是否为测试诊断基准类工具",
]
cpp_rows = [row for row in inventory if row["是否含C++及依据"].startswith("是：")]
assert len(cpp_rows) == 73, len(cpp_rows)
counts = Counter(row["判定结果"] for row in inventory)
assert counts == {"需要改": 10, "不需要改": 236, "当前不能改": 6, "未定": 2}, counts
non_cpp = [row for row in inventory if row["是否含C++及依据"].startswith("否：")]
assert len(non_cpp) == 181
assert {row["判定结果"] for row in non_cpp} == {"不需要改"}

r100 = read_tsv(ROOT / "docs/progress/R100/tables/base_cpp_package_decisions.tsv")
assert len(r100) == 73
mapping = {
    "NEED_LIBCXX": "需要改",
    "NO_LIBCXX_NEEDED": "不需要改",
    "CANNOT_SWITCH_CURRENTLY": "当前不能改",
    "UNDETERMINED": "未定",
}
inventory_by_name = {row["源码包名"]: row for row in inventory}
assert {row["source_package"] for row in r100} == {row["源码包名"] for row in cpp_rows}
for row in r100:
    assert inventory_by_name[row["source_package"]]["判定结果"] == mapping[row["decision"]]

expected_need = {
    "abseil-cpp", "bcc-tools", "boost", "bpftrace", "icu",
    "jsoncpp", "libsigc++", "llvm", "pcre", "taglib",
}
need = {row["源码包名"] for row in inventory if row["判定结果"] == "需要改"}
assert need == expected_need

implementation = read_tsv(DOC / "tables/implementation_10_packages.tsv")
assert len(implementation) == 10
assert {row["源码包"] for row in implementation} == expected_need
for row in implementation:
    assert len(row["R100快照revision"]) == 40
    assert len(row["2026-09-03观测到的tizen_base_HEAD"]) == 40
    assert row["spec路径"].startswith("packaging/") and row["spec路径"].endswith(".spec")

source_edges = read_tsv(DOC / "tables/base_internal_layout_sensitive_edges.tsv")
elf_edges = read_tsv(DOC / "tables/base_internal_layout_sensitive_elf_edges.tsv")
assert len(source_edges) == 4
assert len(elf_edges) == 121
assert not [row for row in source_edges if row["need_consumer_to_no_provider"] != "NO"]
aggregates = defaultdict(lambda: [0, 0])
for row in elf_edges:
    key = (row["consumer_source"], row["provider_source"])
    aggregates[key][0] += 1
    aggregates[key][1] += int(row["cpp_symbol_count"])
for row in source_edges:
    key = (row["consumer_source"], row["provider_source"])
    assert aggregates[key] == [int(row["elf_edge_count"]), int(row["cpp_symbol_count"])]

markdown = (DOC / "tables/base_all_source_packages.md").read_text(encoding="utf-8")
markdown_rows = [line for line in markdown.splitlines() if line.startswith("| ")][1:]
assert len(markdown_rows) == 254

required = [
    "README.md", "REPORT.md", "SUMMARY_ONE_PAGE.md", "UNDETERMINED.md",
    "tables/base_all_source_packages.tsv", "tables/base_all_source_packages.md",
    "tables/implementation_10_packages.tsv", "tables/implementation_10_packages.md",
    "tables/base_internal_layout_sensitive_edges.tsv",
    "tables/base_internal_layout_sensitive_elf_edges.tsv",
]
for relative in required:
    assert (DOC / relative).is_file(), relative

print("source_packages=254")
print("cpp_packages=73")
print("decisions=需要改:10,不需要改:236,当前不能改:6,未定:2")
print("r100_cpp_decisions_preserved=73/73")
print("implementation_packages=10/10")
print("layout_sensitive_source_pairs=4")
print("layout_sensitive_elf_edges=121")
print("need_consumer_to_no_provider=0")
print("markdown_rows=254")
print("validation=PASS")
