#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path

root = Path(__file__).resolve().parents[3]
task = root / "progress/R32"
tables = task / "tables"

def read(name):
    with (tables / name).open(newline="") as f:
        return list(csv.DictReader(f, delimiter="\t"))

summary = read("impact_summary_detailed.tsv")
by_stage = read("impact_by_stage.tsv")
guards = read("existing_guard_packages.tsv")
locations = read("impact_locations.tsv")
macros = read("catchall_macro_definitions.tsv")
macro_uses = read("catchall_macro_uses.tsv")
scan = read("scan_status.tsv")
wave = read("wave1_cancel_path_findings.tsv")
candidate = read("candidate_behavior_matrix.tsv")
direction = read("candidate_direction_summary.tsv")
red = read("r30_red_item_classification.tsv")

smap = {r["category"]: r for r in summary}
scan_counts = Counter(r["status"] for r in scan)
feature_counts = Counter(r["feature"] for r in locations)
scope_counts = Counter((r["feature"], r["scope"]) for r in locations)

def md_table(headers, rows):
    lines = ["| " + " | ".join(headers) + " |",
             "| " + " | ".join(["---"] * len(headers)) + " |"]
    for row in rows:
        lines.append("| " + " | ".join(str(x).replace("|", "\\|") for x in row) + " |")
    return "\n".join(lines)

cat_cn = {
    "EXISTING_GUARD": "已有 `__forced_unwind` 捕获并重抛防护",
    "UNGUARDED_RISK_PATTERN": "无防护但包级同时有 catch-all 与线程/取消迹象",
    "CATCH_ONLY_NO_THREAD_EVIDENCE": "仅有 catch-all、未见包内线程迹象",
    "NO_CATCH_ALL": "未命中 catch-all",
    "UNDETERMINED_SCAN_FAILED": "扫描失败、不可判定",
}

lines = []
lines += [
    "# R32 forced unwind 影响面、候选方向与 R30 红项定性报告",
    "",
    "日期：2026-08-13  ",
    "执行边界：第一、三部分只读取证；第二部分仅构建并运行 `progress/R32/` 中的独立实验件。平台源码、R30 候选补丁、既有制品和既有文档均未改动。",
    "",
    "## 结论摘要",
    "",
    f"- 影响面补扫以 T1 已成功解包的 371/372 个源码包为分母；最终状态为 SCAN_OK {scan_counts['SCAN_OK']}/371、SCAN_FAILED {scan_counts['SCAN_FAILED']}/371。Chromium 仍是 T1 的既有未扫 1 包，不在本次补扫分母之外另作补齐。",
]
for key in ("EXISTING_GUARD", "UNGUARDED_RISK_PATTERN", "CATCH_ONLY_NO_THREAD_EVIDENCE", "NO_CATCH_ALL", "UNDETERMINED_SCAN_FAILED"):
    if key in smap:
        r = smap[key]
        lines.append(f"- {cat_cn[key]}：{r['source_package_count']}/371 个源码包；相关位置 {r['relevant_location_count']} 处。")
lines += [
    "- 首批两包均有 in-tree 单元测试实际创建线程、调用 `pthread_cancel`，并让取消穿过目标包装器；搜索到的生产代码中未观察到 `pthread_cancel` 调用，因此定性为“测试已触发；所搜生产路径中为防御性代码”，不外推到仓外消费者。",
    "- 候选方向 A–D 中，没有一个在本次两套标准库实测里同时满足“强制展开正常传播、真实异常原处理语义不变、两套标准库行为一致”三个条件。该句是矩阵观测的机械合取，不是方案评价。",
    "- R30 三项遗留红项：`_S_badbit`、`__filebuf_type` 为源码级缺陷；Boost.Test 为第三方库 ABI 问题。它们分别揭示 T1 对任意实现私有成员和已构建 DSO ABI 的系统性不可见面；不重算 R1 的 45。",
    "",
    "## 一、语料、口径与方法",
    "",
    "T1 原扫描已覆盖 `pthread_cancel`、`pthread_setcancelstate`、`pthread_testcancel`，但未覆盖 catch-all、`pthread_create` 和 `std::thread`。本次仅在 `progress/T1/extraction_results.tsv` 标为 `EXTRACTED_OK` 的 371 个既有解包目录内补扫；未下载、未重新解包、未扫描仓快照。输入 SHA256 在 `tables/input_identity.tsv`。",
    "",
    "最终扫描命令由 `tools/scan_impact.py` 生成：4 个包级 worker，每个 `rg --threads 1`，限定常见源码/构建/文档文本后缀；命令、进度与退出码在 `commands/017_impact_scan_io_bounded_cached.log`。此前五次无效或受阻尝试均保留在 `tables/supplemental_scan_attempts.tsv`，没有将其结果并入结论。",
    "",
    "包级互斥分类规则：",
    "",
    "1. `EXISTING_GUARD`：存在 `abi::__forced_unwind`/`__cxxabiv1::__forced_unwind` 捕获，且在下一个 catch 前实见 `throw;`；",
    "2. 否则，catch-all 与线程/取消标记在同一源码包内共存为 `UNGUARDED_RISK_PATTERN`；",
    "3. 否则，有 catch-all 为 `CATCH_ONLY_NO_THREAD_EVIDENCE`；",
    "4. 否则为 `NO_CATCH_ALL`。",
    "",
    "`DOC` 路径不参与包分类；`TEST`、`EXAMPLE`、`BUILD` 与 `PRODUCTION_LIKE` 均保留并逐位置标注。catch-all 宏定义及其标识符使用另见 `catchall_macro_definitions.tsv` 与 `catchall_macro_uses.tsv`。",
    "",
    f"位置总表共 {len(locations)} 行：catch-all {feature_counts['catch_all']}、forced-unwind token {feature_counts['forced_unwind']}、pthread_cancel {feature_counts['pthread_cancel']}、pthread_create {feature_counts['pthread_create']}、pthread_setcancelstate {feature_counts['pthread_setcancelstate']}、pthread_testcancel {feature_counts['pthread_testcancel']}、std::thread {feature_counts['std::thread']}。识别到 catch-all 宏定义 {len(macros)} 处、宏定义/使用记录 {len(macro_uses)} 行。",
    "",
    "局限：包级共现不等于调用路径；外部包发起的取消无法由被取消包源码识别；C 宏的多层拼接、动态调用和运行时取消状态无法由该词法扫描完全还原。以上无法判定处没有推断补全。",
    "",
    "## 二、平台影响面",
    "",
    md_table(["类别", "源码包数", "分母", "相关位置数"],
             [(cat_cn.get(r['category'], r['category']), r['source_package_count'], "371 源码包", r['relevant_location_count']) for r in summary]),
    "",
    "逐包分类在 `tables/impact_package_classification.tsv`，逐位置 `file:line`、scope 与阶段在 `tables/impact_category_locations.tsv`；所有匹配位置在 `tables/impact_locations.tsv`。",
    "",
    "### 迁移阶段分布",
    "",
    md_table(["阶段", "类别", "源码包数"],
             [(r['stage'], cat_cn.get(r['category'], r['category']), r['source_package_count']) for r in by_stage]),
    "",
    "### 已有防护包",
    "",
    md_table(["源码 RPM", "包名", "阶段", "确认重抛位置", "生产形态位置"],
             [(r['source_rpm'], r['package_name'], r['stage'], r['confirmed_guard_locations_nondoc'], r['confirmed_guard_locations_production_like']) for r in guards]),
    "",
    "其中测试、文档或第三方内嵌源码中的命中仍按路径 scope 保留；“已有防护包”不等同于其所有命中均为安装后的生产路径。",
    "",
    "## 三、首批两包的取消路径",
    "",
    md_table(["包", "防护位置", "传播位置", "pthread_cancel 实测位置", "结论"],
             [(r['target_source_package'], r['guard_location'], r['propagation_locations'], r['pthread_cancel_calls'], r['production_runtime_conclusion']) for r in wave]),
    "",
    "完整上下文与 44 条传播链来自 `commands/009_collect_wave1_cancel_paths.log` 和 `tables/wave1_cancel_path_findings.tsv`。`libcynara-commons` 测试在 `test/common/exceptions/trycatch.cpp:118-136`；`security-manager` 测试在 `test/test_misc.cpp:79-100`。两个测试均断言 join 结果为 `PTHREAD_CANCELED`。所搜生产路径零调用只表示 NOT_OBSERVED，不表示仓外或未来调用不存在。",
    "",
    "## 四、x86_64 宿主原生候选实验",
    "",
    "执行方式统一为 `HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU`：宿主 x86_64 CPU 原生执行，显式调用各 buildroot 的 x86_64 loader 与 `--library-path`。源码为 `src/forced_unwind_candidates.cpp`；编译/链接原文在两份成功 GBS 日志中，逐格命令与退出码在 `results/native_matrix/`。",
    "",
    "构建留痕：libstdc++ 成功；libc++ 首次因未带 R31 的 extra-packs 导致 `<atomic>` 不可得，失败根未复用；全新 `-02` 根补齐 `libc++-devel,libc++abi-devel,libc++,libc++abi` 后成功。另一次仓根调用在解析 fixture 前失败，亦保留。详见 `tables/experiment_build_status.tsv`。",
    "",
    "### 基准与方向矩阵",
    "",
    "- 基准 catch-all 吞掉取消：libstdc++ 与 libc++ 均 SIGABRT/134，均出现 `FATAL: exception not rethrown`。",
    "- libstdc++ 专有类型防护对照：libstdc++ join 为 `PTHREAD_CANCELED`、退出 0；libc++ 因类型不存在记 77/NOT_AVAILABLE。",
    "- 无 catch 对照：两侧均 join 为 `PTHREAD_CANCELED`、退出 0。",
    "",
    md_table(["方向", "同时满足三个观测条件", "强制展开观测", "真实异常观测"],
             [(r['direction'], r['simultaneously_meets_all_three_observed_conditions'], r['forced_unwind_observation'], r['real_exception_semantics_observation']) for r in direction]),
    "",
    "方向 A 的直接观测在两侧一致：强制展开进入 catch 时 `__cxa_current_exception_type()==nullptr` 且 `std::current_exception()` 为空；真实 `std::runtime_error` 与 `int` 两侧均为 type 非空、exception_ptr 非空。该判据成功区分两类异常，但按判据 `throw;` 后 libstdc++ 正常取消，libc++ 调用本用例 terminate handler 并 SIGABRT。",
    "",
    "方向 B 两侧强制展开均自然穿过 `catch(const std::exception&)`；`std::runtime_error` 仍被该具体捕获，`int` 未被捕获并到达外层观察器。方向 C 两侧均在取消禁用期间完成工作：恢复取消时立即取消；始终不恢复时线程正常返回而非 `PTHREAD_CANCELED`。方向 D 的强制展开在 libstdc++ 正常，在 libc++ terminate；真实 `runtime_error` 与 `int` 两侧都由原 catch 重新抛至外层观察器。",
    "",
    "逐格机器表为 `tables/candidate_behavior_matrix.tsv`；上述结果仅覆盖 x86_64/Itanium、本次裸 pthread/延迟取消及用例中的两种真实异常，不覆盖 armv7l/EHABI、异步取消、任意第三方异常或实际业务调用图。",
    "",
    "## 五、R30 三个遗留红项",
    "",
    md_table(["红项", "包/位置", "归属依据", "分类", "T1 覆盖"],
             [(r['red_item'], f"{r['source_package']} {r['location']}", r['ownership_evidence'], r['classification'], r['t1_pattern_coverage']) for r in red]),
    "",
    "`_S_badbit`：R30 原文为 `std::ios_base::iostate` 在 libc++ 中是 `unsigned int`，不能解析 `_S_badbit`；libstdc++ 的 `bits/ios_base.h` 定义内部 `_Ios_Iostate::_S_badbit`，并用公开 `ios_base::badbit` 映射它。",
    "",
    "`__filebuf_type`：R30 原文为 `no type named '__filebuf_type' in 'std::ifstream/std::ofstream'`；libstdc++ `fstream` 的注释明确写 `Non-standard types:` 后定义该别名，libc++ 头零命中。",
    "",
    "Boost.Test：三架构 R30 buildroot 中 `libboost_unit_test_framework.so` 均 `DT_NEEDED libstdc++.so.6`，其 ostream 导出签名是 libstdc++ ABI；libc++ 消费对象请求 `std::__1::basic_ostream` 版本而链接失败。完整 readelf 与错误原文在 `commands/008_collect_r30_red_item_evidence.log`。",
    "",
    "### 对 T1 分类可靠性的已知含义",
    "",
    "三项揭示两类系统性遗漏：一是没有稳定统一前缀的 libstdc++ 实现私有枚举/嵌套别名；二是源码 token 扫描无法建立已构建第三方 DSO 的标准库 ABI。可以把经头文件归属确认的 `_S_badbit`、`__filebuf_type` 精确 token 加入后续模式集，但泛化 `_S_*` 或双下划线会混入项目私有/编译器保留标识符；Boost 类问题不能仅靠扩展源码 token 模式覆盖，需要链接或 ELF ABI 证据。此处不重扫、不重算 45，只申报：45 不是全部真实构建不兼容的穷举数。",
    "",
    "## 六、证据索引与边界",
    "",
    "- 影响面输入与 SHA：`tables/input_identity.tsv`；实验和红项输入：`tables/experiment_input_identity.tsv`、`tables/red_item_input_identity.tsv`。",
    "- 命令原文与退出码：`commands/`。失败/中止尝试未删除。",
    "- 原始逐格记录：`results/native_matrix/`。",
    "- 本任务没有给出补丁修法、方向推荐、优劣评价或新的 45 包计数。",
]

(task / "R32_report.md").write_text("\n".join(lines) + "\n")
