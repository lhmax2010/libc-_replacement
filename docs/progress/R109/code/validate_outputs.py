#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path

root = Path("/home/toolchain/development/libc++_replacement")
old_path = root / "docs/progress/R102/tables/base_all_source_packages_rewritten.tsv"
new_path = root / "docs/progress/R109/tables/base_all_source_packages_updated.tsv"

with old_path.open(encoding="utf-8", newline="") as stream:
    old = list(csv.DictReader(stream, delimiter="\t"))
with new_path.open(encoding="utf-8", newline="") as stream:
    new = list(csv.DictReader(stream, delimiter="\t"))

assert len(old) == len(new) == 254
assert [row["源码包名"] for row in old] == [row["源码包名"] for row in new]
old_fields = list(old[0])
assert list(new[0]) == old_fields + ["当前实施状态"]

for before, after in zip(old, new):
    changed = {field for field in old_fields if before[field] != after[field]}
    if before["源码包名"] in {"gmp", "tensorflow2"}:
        assert changed == {"判定结果", "理由", "证据"}
    else:
        assert changed == set()

counts = Counter(row["判定结果"] for row in new)
assert counts == {"不需要改": 237, "需要改": 11, "当前不能改": 6}
status_counts = Counter(
    "已完成并推送" if row["当前实施状态"].startswith("已完成并推送")
    else "暂停" if row["当前实施状态"].startswith("暂停")
    else row["当前实施状态"]
    for row in new
)
assert status_counts == {"不适用": 243, "已完成并推送": 8, "暂停": 3}

markdown = (root / "docs/progress/R109/tables/base_all_source_packages_updated.md").read_text(encoding="utf-8")
markdown_rows = sum(1 for line in markdown.splitlines() if line.startswith("| ")) - 1
assert markdown_rows == 254

required_docs = [
    "REPORT.md",
    "README.md",
    "EVIDENCE_REUSE.md",
    "REPOSITORY_SCOPE_RELATION.md",
    "EXECUTIVE_SUMMARY.md",
    "PAUSED_PACKAGES.md",
]
for name in required_docs:
    path = root / "docs/progress/R109" / name
    assert path.is_file() and path.stat().st_size > 0, name

print("rows=254 unique=254")
print("counts=需要改:11,不需要改:237,当前不能改:6,未定:0")
print("original_fields_unchanged_for_252=PASS")
print("gmp_tensorflow2_changed_fields_exact=PASS")
print("implementation_status=completed:8,paused:3,not_applicable:243")
print("markdown_rows=254")
print("required_documents=PASS")
