#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path


ROOT = Path("/home/toolchain/development/libc++_replacement")
SOURCE = ROOT / "docs/progress/R102/tables/base_all_source_packages_rewritten.tsv"
OUTDIR = ROOT / "docs/progress/R109/tables"

COMPLETED = {
    "abseil-cpp": "7a4f04dad6d30ba09c406837cdd23ad779a69210",
    "boost": "4168e873584ae50a12733262026c007dce18ec89",
    "icu": "dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330",
    "jsoncpp": "9cbab147f44097d5b8da2b3ef38f75376a282d01",
    "libsigc++": "4f2f592d2f88eb6a3a949cec8970a4cd5d40165b",
    "pcre": "09a5bfc9a96c4235628ce800db3b95bf28ef2f23",
    "taglib": "8b552bdf27c913a93bca664c41c3fafb214962a1",
    "tensorflow2": "6d76139029ca4162249667f0db65a73387a94cfe",
}
PAUSED = {"llvm", "bcc-tools", "bpftrace"}

GMP_REASON = (
    "`gmp` 是提供多精度整数、浮点和有理数运算的基础库，并另有 C++ 包装库 "
    "`libgmpxx`。R103 对已知消费方的实际链接过程和最终产物作了反查："
    "`eigen` 只安装头文件和 CMake 配置、没有最终 ELF；`python-pycrypto` "
    "三架构的实际链接命令使用 `-lgmp` 而非 `-lgmpxx`，生成的 `_fastmath.so` "
    "只依赖 `libgmp.so.3`，不依赖 `libgmpxx` 或 `libstdc++.so.6`，其 18 个扩展与 "
    "`libgmpxx.a` 的强符号交集也为 0。因此在已核查的消费范围内没有 C++ 静态代码"
    "被带入将迁移到 libc++ 的产物，不需要修改该包。"
)
GMP_EVIDENCE = (
    "R103 `tables/consumer_linkage_summary.tsv` 与 `tables/provider_observation_summary.tsv`："
    "eigen 最终产物无 ELF；python-pycrypto 三架构链接 `-lgmp`、"
    "`_fastmath.so` 的 DT_NEEDED 为 libgmp.so.3；18 个扩展与 libgmpxx.a "
    "强符号交集 0；Base VCS=product/upstream/gmp#490fe651469987ccb00b5cba908d69609345e226"
)
TF_REASON = (
    "`tensorflow2` 提供 TensorFlow Lite/XLA AOT 静态归档和 C++ 开发接口。R103 从三类"
    "真实消费方反查确认：`inference-engine-tflite`、`nnstreamer`、`nntrainer` 在"
    "三架构的实际链接命令中均使用 `libtensorflow2-lite.a`，20 个消费 ELF/架构组合"
    "共命中 5,312 个归档强符号，至少涉及 546 个归档成员；接口还持有并传递 "
    "`std::unique_ptr<tflite::Interpreter>`、"
    "`std::unique_ptr<tflite::FlatBufferModel>` 等布局敏感 C++ 类型。静态归档代码会"
    "直接复制进将迁移到 libc++ 的消费方，若继续按 libstdc++ 构建会形成实际的混合"
    "运行时 C++ 边界，因此需要修改。"
)
TF_EVIDENCE = (
    "R103 `tables/consumer_linkage_summary.tsv`、`tables/archive_member_attribution.tsv` 与 "
    "`tables/provider_observation_summary.tsv`：3 个消费方×3 架构实际链接 "
    "libtensorflow2-lite.a；20 个 ELF/架构组合，5,312 个不同强符号、至少 546 个归档"
    "成员；Base 与 Unified 归档在该批消费 ELF 上 20/20 命中集合相同；"
    "Base VCS=platform/upstream/tensorflow2#1cdba73549f741720f11dd53da1bb516b03763a2"
)


def md_escape(value: str) -> str:
    return value.replace("|", "\\|").replace("\n", "<br>")


with SOURCE.open(encoding="utf-8", newline="") as stream:
    reader = csv.DictReader(stream, delimiter="\t")
    original_fields = list(reader.fieldnames or [])
    original = list(reader)

assert len(original) == 254
assert len({row["源码包名"] for row in original}) == 254
assert Counter(row["判定结果"] for row in original) == {
    "需要改": 10,
    "不需要改": 236,
    "当前不能改": 6,
    "未定": 2,
}
assert {row["源码包名"] for row in original if row["判定结果"] == "未定"} == {
    "gmp",
    "tensorflow2",
}

rows = [dict(row) for row in original]
for row in rows:
    name = row["源码包名"]
    if name == "gmp":
        row["判定结果"] = "不需要改"
        row["理由"] = GMP_REASON
        row["证据"] = GMP_EVIDENCE
    elif name == "tensorflow2":
        row["判定结果"] = "需要改"
        row["理由"] = TF_REASON
        row["证据"] = TF_EVIDENCE

    if name in COMPLETED:
        row["当前实施状态"] = f"已完成并推送（{COMPLETED[name]}）"
    elif name in PAUSED:
        row["当前实施状态"] = "暂停（等待 MLGO/XLA AOT 资产重建）"
    else:
        row["当前实施状态"] = "不适用"

new_fields = original_fields + ["当前实施状态"]
OUTDIR.mkdir(parents=True, exist_ok=True)
tsv_path = OUTDIR / "base_all_source_packages_updated.tsv"
with tsv_path.open("w", encoding="utf-8", newline="") as stream:
    writer = csv.DictWriter(stream, fieldnames=new_fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)

md_path = OUTDIR / "base_all_source_packages_updated.md"
with md_path.open("w", encoding="utf-8") as stream:
    stream.write("# Tizen-Base-Toolchain 全部源码包 libc++ 判定清单（R109）\n\n")
    stream.write(
        "本表以 R102 的 254 项表为基线，只吸收 R103 对 `gmp` 与 `tensorflow2` "
        "的闭合结论，并新增当前实施状态；其余 252 项的原有字段保持不变。\n\n"
    )
    stream.write("| " + " | ".join(new_fields) + " |\n")
    stream.write("|" + "|".join(["---"] * len(new_fields)) + "|\n")
    for row in rows:
        stream.write("| " + " | ".join(md_escape(row[field]) for field in new_fields) + " |\n")

counts = Counter(row["判定结果"] for row in rows)
assert counts == {"需要改": 11, "不需要改": 237, "当前不能改": 6}
assert not any(row["判定结果"] == "未定" for row in rows)
assert set(COMPLETED) | PAUSED == {
    row["源码包名"] for row in rows if row["判定结果"] == "需要改"
}

original_by_name = {row["源码包名"]: row for row in original}
for row in rows:
    name = row["源码包名"]
    changed = {
        field for field in original_fields if row[field] != original_by_name[name][field]
    }
    if name in {"gmp", "tensorflow2"}:
        assert changed == {"判定结果", "理由", "证据"}, (name, changed)
    else:
        assert not changed, (name, changed)

audit_path = OUTDIR / "source_table_change_audit.tsv"
with audit_path.open("w", encoding="utf-8", newline="") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(["检查项", "结果"])
    writer.writerow(["R102 输入行数", len(original)])
    writer.writerow(["R109 输出行数", len(rows)])
    writer.writerow(["原有字段完全不变的包数", 252])
    writer.writerow(["更新原有字段的包", "gmp;tensorflow2"])
    writer.writerow(["两包允许变化的原有字段", "判定结果;理由;证据"])
    writer.writerow(["新增字段", "当前实施状态"])
    writer.writerow(["最终计数", "需要改=11;不需要改=237;当前不能改=6;未定=0"])

status_path = OUTDIR / "implementation_status.tsv"
with status_path.open("w", encoding="utf-8", newline="") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(["源码包名", "判定结果", "当前实施状态", "提交SHA"])
    for row in rows:
        name = row["源码包名"]
        if row["判定结果"] == "需要改":
            status = "已完成并推送" if name in COMPLETED else "暂停（等待 MLGO/XLA AOT 资产重建）"
            writer.writerow([name, row["判定结果"], status, COMPLETED.get(name, "NOT_APPLICABLE")])

print(f"source_rows={len(original)}")
print("final_counts=" + ";".join(f"{key}={counts.get(key, 0)}" for key in ["需要改", "不需要改", "当前不能改", "未定"]))
print(f"unchanged_original_fields=252")
print(f"completed={len(COMPLETED)} paused={len(PAUSED)}")
print(tsv_path)
print(md_path)
