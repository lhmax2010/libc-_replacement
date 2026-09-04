#!/usr/bin/env python3
import csv
import shutil
from pathlib import Path

SRC = Path("/tmp/r114_input_20260904/hq_20260904")
DST = Path("/tmp/r114_output_20260904/hq_20260904")


def replace_once(text: str, old: str, new: str, label: str) -> str:
    count = text.count(old)
    if count != 1:
        raise RuntimeError(f"{label}: expected one occurrence, found {count}")
    return text.replace(old, new)


if DST.exists():
    raise RuntimeError(f"refusing to overwrite {DST}")
shutil.copytree(SRC, DST)

# Base adaptation report: M1-M4 and S4.
p = DST / "base_adaptation_report.md"
t = p.read_text(encoding="utf-8")
t = replace_once(
    t,
    "254 个包中含 C++ 的有 73 个。**含 C++ 并不等于需要适配**",
    "254 个包中，按成功构建日志、C++ runtime 依赖、静态归档外部 C++ 符号和\n"
    "C++ 头文件载体四类机械证据识别出的候选并集为 73 个。这个数字是已声明口径\n"
    "下的候选数，不是“绝对完整的 C++ 源码包全集”；普通 `.h`、未覆盖 ELF 符号、\n"
    "动态加载及预编译私有代码等仍是方法边界。**含 C++ 并不等于需要适配**",
    "M3 candidate scope",
)
t = replace_once(
    t,
    "| 显式走 GCC 路径的包 | 6 | **构建直接失败** |\n"
    "| 已判定不需要适配的 C++ 包 | 55 | **标准库被静默切换** |",
    "| 显式走 GCC 路径的包 | 6 | 若选项实际到达 GCC driver：已测 armv7l/x86_64 会直接失败；aarch64 未观测 |\n"
    "| 已判定不需要适配的 C++ 包 | 55 | 若选项实际到达 Clang++ driver：三架构会实际切换到 libc++；逐包传播未验证 |",
    "S4 driver table",
)
t = replace_once(
    t,
    "已完成的包均通过**三架构 × 两种工具链共六种组合**的构建与产物核查：\n"
    "libc++ 路径下确实生效，GCC 路径下构建不受影响。",
    "其中 7 个包完成三架构 × 两种工具链六格完整构建与产物核查。\n"
    "`tensorflow2` 的 libc++ 三格及 aarch64/x86_64 GCC 两格完整通过；armv7l\n"
    "GCC 候选版与未修改基线均在 XNNPACK 的第 737/4213 步以完全相同的 48 条\n"
    "汇编错误失败，且均未到达本次修改的 `elementwise.cc`，因此该格按“相对基线\n"
    "无回归”通过。armv7l GCC 对 `elementwise.cc` 的实际编译仍为\n"
    "`NOT_OBSERVED`，不能表述为六格完整构建全部成功。",
    "M1 tensorflow grid",
)
t = replace_once(
    t,
    "一个源码包产出多个二进制包 —— `boost` 单一架构下即有 39 个产物。",
    "一个源码包可以产出多个二进制 RPM。以本轮 armv7l/x86_64 的 Boost 验证构建\n"
    "为例，每个架构生成 33 个 RPM；这些 RPM 中合计包含 39 个 ELF。RPM 数与 ELF\n"
    "数是不同单位，不能混用。",
    "M2 boost units",
)
t = replace_once(
    t,
    "**结论**：两组数据描述不同范围与不同单位，各自成立；两仓之间不存在被遗漏\n"
    "的包。",
    "**结论**：两组数据描述不同仓库与不同统计单位，不能直接比较；step0 的 639/91 因原始\n"
    "输入未固定，现不能严格复现。就本次比较的两个固定快照及已声明的 C++ 候选与\n"
    "边界判定口径而言，未发现因两个 Base 仓集合差异而漏掉的 libc++ 迁移对象；\n"
    "这不是对未来快照或其他仓库的永久断言。",
    "M4 repository scope",
)
p.write_text(t, encoding="utf-8")

# Weekly report: M7-M8 and S1-S3, S6-S7.
p = DST / "weekly_report_20260904.md"
t = p.read_text(encoding="utf-8")
t = replace_once(
    t,
    "| 1 | 线程取消被 `catch (...)` 吞掉，导致进程终止 | **已解决** | 补丁已合入 `tizen_base`，待稳定性验证；影响面数据有一处更正，见第六节 |\n"
    "| 2 | libc++ 标准库内部缺少防护 | **已解决** | 同上，属同一补丁集 |",
    "| 1 | 线程取消被 `catch (...)` 吞掉，导致进程终止 | **代码与测试已完成** | 代码修复及两架构官方测试对照已完成；产品级稳定性验证仍待进行；影响面数据有一处更正，见第六节 |\n"
    "| 2 | libc++ 标准库内部缺少防护 | **代码与测试已完成** | 同上，属同一补丁集 |",
    "S6 status table",
)
t = replace_once(
    t,
    "| 6 | 异步取消模式下不经过任何防护 | **影响面已查清** | 本周完成产品使用情况检索，见第四节 |",
    "| 6 | 异步取消模式下不经过任何防护 | **冻结源码的静态形态已查清** | 冻结 371 个源码 RPM 的静态使用形态已查清；2 个 C++ 风险形态的运行时实际取消落点仍未观测，见第四节 |",
    "S7 async scope",
)
t = replace_once(
    t,
    "问题一与问题二由同一组补丁解决，已合入 `tizen_base`：\n\n"
    "| 提交 | 内容 |\n|---|---|\n"
    "| `6771dbc4` | libcxx: backport EH ABI forced-unwind handling |\n"
    "| `8dfebafe` | libcxxabi: declare `__gnu_unwind_frame` in cxa_personality (#189787) |\n\n"
    "该提交由四部分组成：",
    "问题一与问题二由 `6771dbc4` 中的同一组四层改动解决；`8dfebafe` 是该提交之前\n"
    "已在基线中的独立 ARM EHABI 编译修正，不属于这四层改动。当前\n"
    "`tizen_base` HEAD `2d23367d` 位于二者之上。\n\n"
    "| 提交 | 内容 |\n|---|---|\n"
    "| `6771dbc4` | libcxx: backport EH ABI forced-unwind handling（四层改动） |\n"
    "| `8dfebafe` | libcxxabi: declare `__gnu_unwind_frame` in cxa_personality (#189787)（较早的独立修正） |\n\n"
    "`6771dbc4` 由四部分组成：",
    "S1 commit relationship",
)
t = replace_once(
    t,
    "**补丁引入的失败数为 0。** 共同失败 213 项，经两组合对照判定均为上游既有。",
    "**补丁引入的失败数为 0。** 共同失败 213 项，按基线/补丁对照口径归为基线既有；\n"
    "该分类不表示 213 项均已逐项完成根因分析。",
    "S3 common failures",
)
t = replace_once(
    t,
    "**状态：complete，等待稳定性验证。**",
    "**状态：代码修复及两架构官方测试对照已完成；产品级稳定性验证仍待进行。**",
    "S6 status text",
)
t = replace_once(
    t,
    "**当前倾向**：保留现状，即维持现有的异常规格与错误处理行为。",
    "**待裁决**：是否保留现有异常规格与错误处理行为；作出选择前需先补齐正文所列影响面。",
    "M8 decision 3-1",
)
t = replace_once(
    t,
    "**该形态可直接借鉴**，此前评估中「libc++ 无现成 wrapper、实现写法未定」\n"
    "这一项已不再是障碍。\n\n"
    "**当前倾向**：与 libstdc++ 保持一致的处理方式。",
    "该形态可作为 libc++ 设计时的参考。libc++ 当前没有现成的 condition-variable\n"
    "兼容 wrapper 框架，移植可行性、旧调用点展开兼容性和具体实现仍需验证，不能\n"
    "据此认定实现障碍已经消失。\n\n"
    "**可行选项，尚未裁决**：参考 libstdc++ 的双版本符号和 wrapper 形态；尚未在\n"
    "libc++ 中实施验证，也尚未由人工选定。",
    "M8 decision 3-2",
)
t = replace_once(
    t,
    "**当前倾向**：保留现有语义，不改动。",
    "**待裁决**：保留现有同步语义，或改为不同步并接受尾部数据/编码收尾语义变化。",
    "M8 decision 3-3",
)
t = replace_once(t, "| # | 待决事项 | libstdc++ 的做法 | 当前倾向 |", "| # | 待决事项 | libstdc++ 的做法 | 待裁决选项 |", "M8 table header")
t = replace_once(t, "| 1 | 错误路径从终止变为抛异常，是否接受 | **不适用**（本身不检查、不抛） | 保留现状，待全面验证 |", "| 1 | 错误路径从终止变为抛异常，是否接受 | **不适用**（本身不检查、不抛） | 是否保留现有异常规格与错误处理行为；需先补齐影响面 |", "M8 table row 1")
t = replace_once(t, "| 2 | 是否为未重编的旧对象保留原有契约 | 双版本符号 + `.symver` | 与其保持一致 |", "| 2 | 是否为未重编的旧对象保留原有契约 | 双版本符号 + `.symver` | 可参考该形态；libc++ 实施与兼容性仍需验证 |", "M8 table row 2")
t = replace_once(t, "| 3 | 析构是否继续执行同步 | 不同步（`= default`） | 保留现有语义 |", "| 3 | 析构是否继续执行同步 | 不同步（`= default`） | 保留同步，或不同步并接受尾部语义变化 |", "M8 table row 3")
t = replace_once(
    t,
    "**一项需要指出的关系**：问题三的五处站点与异步取消模式**正交**。异步取消可在\n"
    "任意指令处送达，落在任何不抛异常的函数上都会终止，与使用哪套标准库无关。\n"
    "**因此问题三的处置方案对这两个组件不产生帮助**，需要单独处理。",
    "**一项需要指出的关系**：异步取消可以落在五处已知边界之外的其他不抛异常帧中，\n"
    "因此问题三的五处定点修正不能作为这两个组件的完整处置。当前只完成源码形态\n"
    "确认，尚未观察到两个组件运行时的实际取消落点；不能据此断言五处修正对其\n"
    "“完全不产生帮助”，该风险需要作为独立条目验证和处理。",
    "M7 async relation",
)
t = replace_once(
    t,
    "**平台上的规模**：仅 Boost 一处边界即涉及 13 个消费方源码包、75 个二进制、\n"
    "90 条实际传递 C++ 语义的符号边。",
    "**平台上的规模**：仅 Boost 一处边界即涉及 13 个 consumer 源码包、75 个去重\n"
    "`(架构, consumer ELF 路径)` 记录和 90 条真实 C++ 符号边。",
    "S2 boost units",
)
p.write_text(t, encoding="utf-8")

# Method document: S5.
p = DST / "base_method_explained.md"
t = p.read_text(encoding="utf-8")
t = replace_once(
    t,
    "六份镜像清单和关键派生表的哈希列在随附证据索引中。",
    "六份镜像清单和关键派生表的 SHA256 可按需一并提供。",
    "S5 first evidence index",
)
t = replace_once(
    t,
    "随附证据索引给出这些材料的\n仓内路径和 SHA256，便于取得后先验证文件身份。",
    "上述原始材料、生成脚本及其 SHA256 可按需一并提供；取得后应先校验文件身份。",
    "S5 second evidence index",
)
p.write_text(t, encoding="utf-8")

# Attachments: M6.
p = DST / "attach_50_row_audit.tsv"
with p.open(encoding="utf-8", newline="") as stream:
    reader = csv.DictReader(stream, delimiter="\t")
    rows = list(reader)
    fields = list(reader.fieldnames or [])
assert fields[-1] == "r32_status"
fields[-1] = "containing_handler_propagation"
for row in rows:
    row["containing_handler_propagation"] = row.pop("r32_status").replace("CONFIRMED_RETHROW", "RETHROW_CONFIRMED")
with p.open("w", encoding="utf-8", newline="") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)

p = DST / "attach_172_row_sites.tsv"
with p.open(encoding="utf-8", newline="") as stream:
    reader = csv.DictReader(stream, delimiter="\t")
    rows = list(reader)
    fields = list(reader.fieldnames or [])
assert fields[-1] == "source_kind"
assert {row["source_kind"] for row in rows} == {"R32_FROZEN_SOURCE_RPM_COMPREHENSIVE_TEXT_RESCAN"}
fields.remove("source_kind")
with p.open("w", encoding="utf-8", newline="") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows({field: row[field] for field in fields} for row in rows)

print(f"prepared={DST}")
for path in sorted(DST.iterdir()):
    print(path.name)
