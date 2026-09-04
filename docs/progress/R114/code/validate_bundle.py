#!/usr/bin/env python3
import csv
import re
from collections import Counter
from pathlib import Path

BUNDLE = Path("/tmp/r114_output_20260904/hq_20260904")


def rows(name):
    with (BUNDLE / name).open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        return list(reader.fieldnames or []), list(reader)


fields, packages = rows("base_packages_final.tsv")
assert len(fields) == 9
assert len(packages) == 254
assert len({r["Source package"] for r in packages}) == 254
assert Counter(r["Decision"] for r in packages) == {
    "requires adaptation": 11,
    "does not require adaptation": 237,
    "currently cannot be adapted": 6,
}
assert Counter(r["Current implementation status"].split(" (")[0] for r in packages) == {
    "not applicable": 243,
    "completed and pushed": 8,
    "paused": 3,
}
md_lines = (BUNDLE / "base_packages_final.md").read_text(encoding="utf-8").splitlines()
assert len(md_lines) == 6 + 254
assert md_lines[4].startswith("| Source package |")
assert md_lines[6].startswith("| abseil-cpp |")

fields50, rows50 = rows("attach_50_row_audit.tsv")
assert len(rows50) == 50
assert fields50[-1] == "containing_handler_propagation"
assert "r32_status" not in fields50
assert Counter(r["row_nature"] for r in rows50) == {"ACTUAL_CATCH_CLAUSE": 46, "NON_CLAUSE_TOKEN_LINE": 4}
assert {r["containing_handler_propagation"] for r in rows50} == {"RETHROW_CONFIRMED"}

fields172, rows172 = rows("attach_172_row_sites.tsv")
assert len(rows172) == 172
assert "source_kind" not in fields172
logical = {(r["package_name"], r["file"], r["line"], r["clause_text"], r["rethrow_kind"]) for r in rows172}
assert len(logical) == 104
assert Counter(r["rethrow_kind"] for r in rows172) == {"LIBSTDCXX_RETHROW_MACRO": 126, "LITERAL_THROW": 46}

weekly = (BUNDLE / "weekly_report_20260904.md").read_text(encoding="utf-8")
base = (BUNDLE / "base_adaptation_report.md").read_text(encoding="utf-8")
method = (BUNDLE / "base_method_explained.md").read_text(encoding="utf-8")

required = {
    "weekly S1": "`8dfebafe` 是该提交之前\n已在基线中的独立 ARM EHABI 编译修正，不属于这四层改动",
    "weekly S3": "该分类不表示 213 项均已逐项完成根因分析",
    "weekly S6": "代码修复及两架构官方测试对照已完成；产品级稳定性验证仍待进行",
    "weekly M7": "尚未观察到两个组件运行时的实际取消落点",
    "weekly M8": "待裁决选项",
    "weekly S2": "75 个去重\n`(架构, consumer ELF 路径)` 记录",
    "base M1": "armv7l GCC 对 `elementwise.cc` 的实际编译仍为\n`NOT_OBSERVED`",
    "base M2": "每个架构生成 33 个 RPM；这些 RPM 中合计包含 39 个 ELF",
    "base M3": "四类机械证据识别出的候选并集为 73 个",
    "base S4": "若选项实际到达 GCC driver",
    "base M4": "这不是对未来快照或其他仓库的永久断言",
    "method S5a": "六份镜像清单和关键派生表的 SHA256 可按需一并提供",
    "method S5b": "上述原始材料、生成脚本及其 SHA256 可按需一并提供",
}
for label, token in required.items():
    target = weekly if label.startswith("weekly") else base if label.startswith("base") else method
    assert token in target, label

forbidden_phrases = [
    "已完成的包均通过**三架构 × 两种工具链共六种组合**",
    "`boost` 单一架构下即有 39 个产物",
    "254 个包中含 C++ 的有 73 个",
    "两仓之间不存在被遗漏",
    "该形态可直接借鉴",
    "对这两个组件不产生帮助",
    "随附证据索引",
]
joined = "\n".join(path.read_text(encoding="utf-8", errors="replace") for path in BUNDLE.iterdir() if path.is_file())
for phrase in forbidden_phrases:
    assert phrase not in joined, phrase

internal = re.compile(r"R(?:32|100|102|103|109)(?![0-9])|N1_|C1_|NO_[A-Z]|R32_FROZEN")
hits = internal.findall(joined)
assert not hits, hits[:20]

han = re.compile(r"[\u3400-\u9fff]")
for row in packages:
    assert not any(han.search(value) for value in row.values()), row["Source package"]

print("package_rows=254")
print("package_decisions=requires_adaptation:11,does_not_require_adaptation:237,currently_cannot_be_adapted:6")
print("implementation_status=completed:8,paused:3,not_applicable:243")
print("attachment_50_rows=50 actual_catch=46 non_clause=4")
print("attachment_172_rows=172 logical_sites=104 literal_throw=46 macro_rethrow=126")
print("r113_required_replacements=PASS")
print("r113_old_phrases_absent=PASS")
print("internal_markers=0")
print("english_table_chinese_cells=0")
