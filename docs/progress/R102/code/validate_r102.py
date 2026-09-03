#!/usr/bin/env python3
import csv
import re
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OLD = ROOT / "docs/progress/R101/tables/base_all_source_packages.tsv"
NEW = ROOT / "progress/R102/tables/base_all_source_packages_rewritten.tsv"
COMPARE = ROOT / "progress/R102/tables/reason_before_after.tsv"


def read(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


old = read(OLD)
new = read(NEW)
comparison = read(COMPARE)
assert len(old) == len(new) == len(comparison) == 254
assert list(old[0]) == list(new[0])
assert [row["源码包名"] for row in old] == [row["源码包名"] for row in new]

for before, after, comp in zip(old, new, comparison):
    assert before["源码包名"] == after["源码包名"] == comp["源码包名"]
    for field in before:
        if field != "理由":
            assert before[field] == after[field], (after["源码包名"], field)
    assert before["理由"] == comp["改写前理由"]
    assert after["理由"] == comp["改写后理由"]
    assert before["判定结果"] == comp["判定结果"]
    assert before["理由"] != after["理由"]
    assert "仓库摘要" in after["理由"]
    assert len(after["理由"]) >= 150, (after["源码包名"], len(after["理由"]))
    assert not re.search(r"(?:N1_|NO_|C1_|U_|NEED_LIBCXX|NO_LIBCXX_NEEDED|UNDETERMINED)", after["理由"]), after["源码包名"]
    assert "无证据" not in after["理由"]
    assert "未发现" not in after["理由"]

counts = Counter(row["判定结果"] for row in new)
assert counts == {"需要改": 10, "不需要改": 236, "当前不能改": 6, "未定": 2}

new_by_name = {row["源码包名"]: row for row in new}
for name in ["abseil-cpp", "boost", "icu", "jsoncpp", "pcre"]:
    assert "Unified" in new_by_name[name]["理由"]
    assert "armv7l" in new_by_name[name]["理由"]
for name in ["bcc-tools", "bpftrace", "llvm"]:
    assert "Base 内部" in new_by_name[name]["理由"]
for name in ["gmp", "tensorflow2"]:
    text = new_by_name[name]["理由"]
    assert "缺少" in text and "可能需要改" in text
for name in ["gcc", "gcc-aarch64", "gcc-armv7hl", "gcc-armv7l", "gcc-riscv64", "glibc"]:
    text = new_by_name[name]["理由"]
    assert "不是永久技术结论" in text
    assert "混合标准库" in text

markdown = (ROOT / "progress/R102/tables/base_all_source_packages_rewritten.md").read_text(encoding="utf-8")
compare_md = (ROOT / "progress/R102/tables/reason_before_after.md").read_text(encoding="utf-8")
assert len([line for line in markdown.splitlines() if line.startswith("| ")][1:]) == 254
assert len([line for line in compare_md.splitlines() if line.startswith("| ")][1:]) == 254

lengths = [len(row["理由"]) for row in new]
print("rows=254")
print("all_non_reason_columns_identical=YES")
print("decisions_identical=254/254")
print("reasons_changed=254/254")
print(f"reason_length_min={min(lengths)}")
print(f"reason_length_max={max(lengths)}")
print("internal_rule_tokens_in_reasons=0")
print("unsupported_no_evidence_phrasing=0")
print("markdown_rows=254")
print("validation=PASS")
