#!/usr/bin/env python3
import csv
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path(__file__).resolve().parents[3]
R101_TABLE = ROOT / "docs/progress/R101/tables/base_all_source_packages.tsv"
R100_DECISIONS = ROOT / "docs/progress/R100/tables/base_cpp_package_decisions.tsv"
CROSS_EDGES = ROOT / "docs/progress/R100/tables/armv7l_cross_edges/actual_cross_source_edge_classification.tsv"
BASE_SOURCES = ROOT / "progress/R98/tables/base_source_records.tsv"
UNIFIED_SOURCES = ROOT / "progress/R98/tables/unified_source_records.tsv"

OUT_DIR = ROOT / "progress/R102/tables"
OUT_TSV = OUT_DIR / "base_all_source_packages_rewritten.tsv"
OUT_MD = OUT_DIR / "base_all_source_packages_rewritten.md"
COMPARE_TSV = OUT_DIR / "reason_before_after.tsv"
COMPARE_MD = OUT_DIR / "reason_before_after.md"
GROUPS_MD = ROOT / "progress/R102/NO_CHANGE_GROUPS.md"
INSUFFICIENT_MD = ROOT / "progress/R102/INSUFFICIENT_REASONS.md"


def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path, rows, fields):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(
            stream, delimiter="\t", fieldnames=fields, lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(rows)


def esc(value):
    return value.replace("|", "\\|").replace("\n", " ")


def write_md(path, title, intro, rows, fields):
    lines = [f"# {title}", "", intro, ""]
    lines.append("| " + " | ".join(fields) + " |")
    lines.append("|" + "|".join("---" for _ in fields) + "|")
    for row in rows:
        lines.append("| " + " | ".join(esc(str(row[field])) for field in fields) + " |")
    lines.append("")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines), encoding="utf-8")


def record_map(path):
    return {
        row["location"]: row["name"]
        for row in read_tsv(path)
        if row["kind"] == "source"
    }


inventory = read_tsv(R101_TABLE)
r100_rows = read_tsv(R100_DECISIONS)
r100 = {row["source_package"]: row for row in r100_rows}
base_sources = record_map(BASE_SOURCES)
unified_sources = record_map(UNIFIED_SOURCES)

runtime_edges = defaultdict(list)
for edge in read_tsv(CROSS_EDGES):
    provider = base_sources[edge["provider_sourcerpm"]]
    consumer = unified_sources[edge["consumer_sourcerpm"]]
    runtime_edges[provider].append(
        {
            "consumer": consumer,
            "classification": edge["classification"],
            "detail": edge["detail_classes"],
        }
    )


purpose_overrides = {
    "abseil-cpp": "一组供其他 C++ 项目复用的基础库",
    "bcc-tools": "基于 BPF 的性能分析、监控与跟踪工具及其 libbcc 运行库",
    "boost": "被大量项目复用的通用 C++ 库集合",
    "bpftrace": "基于 eBPF 的高级跟踪工具",
    "icu": "提供 Unicode 与国际化能力的基础库",
    "jsoncpp": "提供 JSON 读写能力的 C++ 库",
    "libsigc++": "提供类型安全信号与回调机制的 C++ 库",
    "llvm": "编译器、链接器、调试器等工具共用的编译基础设施",
    "pcre": "提供 Perl 兼容正则表达式能力的基础库，并包含 C++ 接口载体",
    "taglib": "读取和写入音频元数据的库",
    "bison": "把语法规则生成解析器源码的构建工具",
    "cereal": "由使用方直接包含和实例化的 C++ 序列化头文件库",
    "gmp": "提供多精度整数、浮点和有理数运算的基础库",
    "tensorflow2": "提供 TensorFlow 机器学习运行时、静态库和开发接口的软件包",
    "libcxx-runtimes": "提供 libc++ 与 libc++abi 本身的运行时和开发文件",
}


def summary(row):
    match = re.search(r"(?:^|；)Summary=(.*)$", row["证据"])
    if not match:
        raise ValueError(f"missing Summary for {row['源码包名']}")
    return match.group(1).strip()


def purpose(row):
    name = row["源码包名"]
    if name in purpose_overrides:
        return f"`{name}` 是{purpose_overrides[name]}（仓库摘要：“{summary(row)}”）。"
    return f"`{name}` 的仓库摘要将其用途定义为“{summary(row)}”。"


def image_sentence(row):
    if row["是否进入设备镜像"].startswith("是："):
        return "它的源码版本精确匹配至少一份已核对的标准设备镜像清单。"
    return "它的源码版本不在已核对的六份标准设备镜像清单中。"


def joined(names):
    names = sorted(set(names))
    return "、".join(f"`{name}`" for name in names)


def group_for(row):
    if row["是否含C++及依据"].startswith("否："):
        return "不含 C++ 载体"
    rule = r100[row["源码包名"]]["matched_rules"]
    return {
        "N1_EXACT_CPP_RUNTIME_EDGE": "需要迁移：存在实测 C++ 运行时边",
        "NO_ALREADY_EXPLICIT_LIBCXX_RUNTIME_BUILD": "已是 libc++ 运行时",
        "NO_BASE_ONLY_OR_INDEPENDENT_PRODUCT_NO_UNIFIED_RUNTIME_EDGE": "独立工具或仅在 Base 拓扑中运行",
        "NO_BUILD_EXPOSURE_WITHOUT_SHIPPED_CPP_BOUNDARY": "C++ 只出现在构建或非运行时载体",
        "NO_CPP_STATIC_ARCHIVE_WITHOUT_UNIFIED_CONSUMER": "含 C++ 静态归档但无消费边",
        "NO_EXACT_EDGES_ONLY_C_OR_NO_INTERSECTION": "跨包边界只有 C 接口",
        "NO_HEADER_PAYLOAD_COMPILED_BY_CONSUMER": "头文件由使用方编译",
        "C1_CURRENT_GCC_OVERRIDE_OR_GCC_ROLE": "当前 GCC 角色约束",
        "U_STATIC_CPP_ARCHIVE_WITH_BUILDREQUIRE_CONSUMERS_LINK_NOT_PROVEN": "静态链接关系尚未闭合",
    }[rule]


def need_reason(row, evidence):
    name = row["源码包名"]
    edges = runtime_edges.get(name, [])
    cpp_edges = [
        edge for edge in edges
        if edge["classification"] in {"LAYOUT_SENSITIVE_STD_TYPE", "CPP_ABI_NO_STD_LAYOUT_PROVEN"}
    ]
    layout = [edge["consumer"] for edge in cpp_edges if edge["classification"] == "LAYOUT_SENSITIVE_STD_TYPE"]
    cpp_only = [edge["consumer"] for edge in cpp_edges if edge["classification"] == "CPP_ABI_NO_STD_LAYOUT_PROVEN"]
    parts = [purpose(row)]
    if cpp_edges:
        parts.append(
            "armv7l 的实际 ELF 依赖和符号交集确认，计划切换到 libc++ 的 Unified 侧软件包"
            f"：{joined(edge['consumer'] for edge in cpp_edges)}，在运行时通过 C++ ABI 调用它。"
        )
        if layout:
            parts.append(f"其中，{joined(layout)} 的边已直接证明会传递布局敏感的标准库类型。")
        if cpp_only:
            parts.append(f"{joined(cpp_only)} 还存在已确认的 C++ ABI 耦合，但现有符号不能进一步证明具体布局传递。")
    if name in {"bcc-tools", "bpftrace"}:
        parts.append(
            f"armv7l 的 Base 内部实测显示，它以布局敏感的 C++ 接口调用 `llvm`；`llvm` 同属本次必须切换的集合。"
        )
    elif name == "llvm":
        parts.append(
            "此外，Base 内部的 `bcc-tools`、`bpftrace` 和 LLVM 自身工具会以布局敏感的 C++ 接口调用 LLVM/Clang 库。"
        )
    if layout or name in {"bcc-tools", "bpftrace", "llvm"}:
        parts.append(
            "若该包继续链接 libstdc++，这些调用会跨越两套标准库；链接器不会阻止这种组合，而对象布局、所有权或析构可能被按错误实现解释，因此必须一并切换。"
        )
    else:
        parts.append(
            "若该包继续链接 libstdc++，上述实测 C++ ABI 调用会保留跨标准库边界；该 ABI 组合不能保证类型、异常和所有权语义一致，因此必须一并切换。"
        )
    return "".join(parts)


def non_cpp_reason(row):
    return (
        purpose(row)
        + "现有三架构材料对四类载体的检查结果均为否：构建日志没有 C++ 编译动作，"
        "RPM 没有 C++ 运行时依赖，静态归档没有可解码的全局 C++ 符号，文件清单也没有 C++ 头文件载体。"
        + image_sentence(row)
        + "因此该包没有需要选择 libc++ 或 libstdc++ 的 C++ 构建/交付边界，无需加入标准库切换选项。"
    )


def build_only_reason(row, evidence):
    signals = evidence["denominator_evidence"].split(";")
    parts = [purpose(row)]
    if "ACTUAL_CPP_BUILD_LOG" in signals:
        parts.append("实际构建日志中有 C++ 编译动作，")
    if "CPP_HEADER_PAYLOAD" in signals:
        parts.append(
            f"已发布包还包含 {evidence['cpp_header_rpm_count']} 个架构份次的 C++ 头文件载体；这些头文件由使用方自己的编译器和标准库编译。"
        )
    else:
        parts.append("但已发布包没有 C++ 头文件载体。")
    parts.append(
        f"包级检查同时给出：直接 C++ 运行时依赖为“{evidence['dynamic_cpp_runtime_signal']}”、"
        f"含全局 C++ 符号的静态归档数量为 {evidence['cpp_static_archive_count']}；"
        "armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。"
    )
    if "PURE_C_INTERFACE" in evidence["base_internal_edge_classes_as_consumer"]:
        parts.append("它在 Base 内部测得的跨包调用也只经过 C 接口。")
    parts.append(image_sentence(row))
    parts.append(
        "所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。"
    )
    return "".join(parts)


def independent_reason(row, evidence):
    parts = [purpose(row)]
    parts.append(
        "它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得："
        "通向 Unified 迁移集合的运行时边为 0；"
    )
    internal = evidence["base_internal_edge_classes_as_consumer"]
    if internal == "NONE":
        parts.append("Base 内部跨包 C++ 语义边也为 0。")
    elif "PURE_C_INTERFACE" in internal:
        parts.append("Base 内部唯一相关调用只共享 C 接口符号，不传递 C++ 对象。")
    else:
        raise ValueError(f"unexpected internal class for {row['源码包名']}: {internal}")
    parts.append(image_sentence(row))
    if row["是否为测试诊断基准类工具"].startswith("是："):
        parts.append("仓元数据还将它标识为测试、诊断或构建工具，按已测拓扑独立运行。")
    parts.append(
        "因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。"
    )
    return "".join(parts)


def c_interface_reason(row, evidence):
    consumers = [edge["consumer"] for edge in runtime_edges[row["源码包名"]]]
    return (
        purpose(row)
        + "它的部分 ELF 自身直接依赖 libstdc++，但 armv7l 的实际依赖和符号交集显示，"
        f"Unified 侧调用方：{joined(consumers)}；跨包共享的都是 C 接口符号，其余被分析边没有 C++ 符号交集。"
        + ("Base 内部相关调用同样只共享 C 接口。" if "PURE_C_INTERFACE" in evidence["base_internal_edge_classes_as_consumer"] else "")
        + image_sentence(row)
        + "C 接口只传递 C 类型，不依赖任一 C++ 标准库的对象布局、异常或析构规则，因此两侧标准库不同不会改变这条已测边界，判定无需改。"
    )


def static_archive_reason(row, evidence):
    return (
        purpose(row)
        + f"包中确有 {evidence['cpp_static_archive_count']} 个架构份次的静态归档含可解码全局 C++ 符号，"
        "但 armv7l 精确依赖图中，Unified 与 Base 内部对这些产物的运行时消费边均为 0。"
        + image_sentence(row)
        + "在当前已测拓扑里，这些 C++ 代码没有被迁移侧吸收到同一进程，也没有跨包传递 C++ 语义；切换该包不能关闭额外边界，判定无需改。"
    )


def header_reason(row, evidence):
    extra = ""
    if row["源码包名"] == "bison":
        extra = "它作为构建工具把输入语法生成源码，本身不提供 C++ 运行库。"
    return (
        purpose(row)
        + extra
        + f"C++ 命中来自 {evidence['cpp_header_rpm_count']} 个架构份次的头文件载体；"
        "已发布包的直接 C++ 运行时依赖为“NO”，含全局 C++ 符号的静态归档数量为 0，"
        "armv7l 运行时图中的 Unified/Base C++ 边也都是 0。"
        + image_sentence(row)
        + "这些头文件最终由使用方按其自身工具链实例化，会自然继承使用方选择的 libc++，无需给本包单独增加标准库切换。"
    )


def already_libcxx_reason(row):
    return (
        purpose(row)
        + "它构建和安装的正是目标 libc++/libc++abi，实现本身不直接链接 libstdc++。"
        "本轮逐包切换针对的是仍消费 libstdc++ 的其他 C++ 包；对运行时提供方再添加消费者侧的 `-stdlib=libc++` 不构成迁移，因此判定无需改。"
    )


def cannot_reason(row, evidence):
    name = row["源码包名"]
    if name.startswith("gcc"):
        role = "GCC 编译器及其目标架构工具链"
    else:
        role = "GNU C 运行库的构建"
    if evidence["dynamic_cpp_runtime_signal"] == "YES":
        runtime = "其中已发布 C++ ELF 直接依赖 libstdc++。"
        island = (
            "它会继续作为 libstdc++ 工具链岛存在；今后若新增与 libc++ 代码同进程交换 C++ 对象的边，必须重新评估。"
        )
    else:
        runtime = "其已发布产物没有直接 C++ 运行时依赖，C++ 只出现在构建动作中。"
        island = (
            "它会继续处于 GCC 工具链构建角色，但当前产物并未形成 libstdc++ C++ ABI 岛；今后若产物开始携带 C++ 运行时并与 libc++ 代码交换对象，必须重新评估。"
        )
    return (
        purpose(row)
        + f"当前 spec/产品角色要求使用 {role}，因此不能在本轮直接改成 libc++；这是当前配置和产品职责的限制，不是永久技术结论。"
        + runtime
        + "armv7l 精确图中，通向 Unified 的 C++ 运行时边和 Base 内部 C++ 语义边均为 0，故覆盖范围内没有已建立的混合标准库对象边界。"
        + image_sentence(row)
        + island
    )


def undetermined_reason(row, evidence):
    name = row["源码包名"]
    consumers = evidence["unified_buildrequire_consumers"].split(";")
    if name == "gmp":
        fact = (
            "三架构 `libgmpxx.a` 各含 11 个可解码全局 C++ 符号，`libgmpxx` 运行库也直接依赖 libstdc++。"
        )
        artifacts = "`libgmpxx.a`"
    else:
        fact = (
            "三架构的 `libtf_xla_runtime.a` 每架构约有 4.67 万个全局 C++ 符号，"
            "`libtensorflow2-lite.a` 每架构约有 0.99–1.00 万个，并安装大规模 C++ 头文件。"
        )
        artifacts = "`libtf_xla_runtime.a` 或 `libtensorflow2-lite.a`"
    return (
        purpose(row)
        + fact
        + f"Unified 包：{joined(consumers)}，把它列为构建依赖，但构建依赖不能证明最终 ELF 实际吸收了 {artifacts} 的成员。"
        "缺少同一构建轮次的详细链接命令、link map 和最终 ELF 对象来源，因而不能把它判成安全或必须迁移。"
        "定下来前不直接修改 spec，但按“可能需要改”预留构建与验证资源；若实际静态链接成立而仍保留 libstdc++，风险是把其对象布局或异常语义复制进 libc++ 消费方。"
    )


def make_reason(row):
    name = row["源码包名"]
    if row["是否含C++及依据"].startswith("否："):
        return non_cpp_reason(row)
    evidence = r100[name]
    rule = evidence["matched_rules"]
    if rule == "N1_EXACT_CPP_RUNTIME_EDGE":
        return need_reason(row, evidence)
    if rule == "NO_BUILD_EXPOSURE_WITHOUT_SHIPPED_CPP_BOUNDARY":
        return build_only_reason(row, evidence)
    if rule == "NO_BASE_ONLY_OR_INDEPENDENT_PRODUCT_NO_UNIFIED_RUNTIME_EDGE":
        return independent_reason(row, evidence)
    if rule == "NO_EXACT_EDGES_ONLY_C_OR_NO_INTERSECTION":
        return c_interface_reason(row, evidence)
    if rule == "NO_CPP_STATIC_ARCHIVE_WITHOUT_UNIFIED_CONSUMER":
        return static_archive_reason(row, evidence)
    if rule == "NO_HEADER_PAYLOAD_COMPILED_BY_CONSUMER":
        return header_reason(row, evidence)
    if rule == "NO_ALREADY_EXPLICIT_LIBCXX_RUNTIME_BUILD":
        return already_libcxx_reason(row)
    if rule == "C1_CURRENT_GCC_OVERRIDE_OR_GCC_ROLE":
        return cannot_reason(row, evidence)
    if rule == "U_STATIC_CPP_ARCHIVE_WITH_BUILDREQUIRE_CONSUMERS_LINK_NOT_PROVEN":
        return undetermined_reason(row, evidence)
    raise ValueError(f"unhandled rule {rule} for {name}")


rewritten = []
comparison = []
for original in inventory:
    row = dict(original)
    old = row["理由"]
    row["理由"] = make_reason(row)
    rewritten.append(row)
    comparison.append(
        {
            "源码包名": row["源码包名"],
            "判定结果": row["判定结果"],
            "理由分组": group_for(row),
            "改写前理由": old,
            "改写后理由": row["理由"],
        }
    )

fields = list(inventory[0])
write_tsv(OUT_TSV, rewritten, fields)
write_md(
    OUT_MD,
    "Tizen-Base-Toolchain 全部源码包 libc++ 判定清单（对外说明版）",
    "数据与判定逐项沿用 R101/R100；仅将理由改写为可独立理解的对外说明。精确运行时符号边的架构范围在理由中明确写为 armv7l。",
    rewritten,
    fields,
)

compare_fields = ["源码包名", "判定结果", "理由分组", "改写前理由", "改写后理由"]
write_tsv(COMPARE_TSV, comparison, compare_fields)
write_md(
    COMPARE_MD,
    "R101 与 R102 理由逐包对照",
    "判定列原样保留；本表用于抽查说明文字是否超出已有证据。",
    comparison,
    compare_fields,
)

no_groups = defaultdict(list)
for row, comp in zip(rewritten, comparison):
    if row["判定结果"] == "不需要改":
        no_groups[comp["理由分组"]].append(row)

common = {
    "不含 C++ 载体": "三架构四项检查（实际构建日志、C++ 运行时依赖、静态归档全局 C++ 符号、C++ 头文件载体）结果均为否；没有可施加标准库选择的 C++ 构建或交付边界。",
    "C++ 只出现在构建或非运行时载体": "有构建期 C++ 动作或头文件载体，但包级运行时信号和精确运行时图不构成由本包固定实现的跨包 C++ ABI 边界。进入镜像与否逐包注明。",
    "独立工具或仅在 Base 拓扑中运行": "产物直接链接 libstdc++，但 armv7l 完整精确拓扑测得通向 Unified 的运行时边为 0；Base 内部也没有 C++ 语义边，或仅有纯 C 接口。",
    "跨包边界只有 C 接口": "产物自身可以用 C++ 实现并链接 libstdc++，但实际 consumer/provider 符号交集只有 C 接口；C++ 对象布局、异常和析构规则不跨边界。",
    "含 C++ 静态归档但无消费边": "静态归档确有 C++ 符号，但精确拓扑中的 Unified 与 Base 内部运行时消费边均为 0，当前没有被迁移侧吸收到同一进程。",
    "头文件由使用方编译": "C++ 载体是头文件而非本包固定实现的运行时；使用方会用自己的编译器和标准库实例化。",
    "已是 libc++ 运行时": "该包就是 libc++/libc++abi 提供方，不是仍消费 libstdc++、需要切换的对象。",
}
group_order = [
    "不含 C++ 载体",
    "C++ 只出现在构建或非运行时载体",
    "独立工具或仅在 Base 拓扑中运行",
    "跨包边界只有 C 接口",
    "含 C++ 静态归档但无消费边",
    "头文件由使用方编译",
    "已是 libc++ 运行时",
]
group_lines = [
    "# 236 个“不需要改”包的分组说明",
    "",
    "本文件把共同判据与逐包差异分开。所有判定均沿用 R100；运行时精确符号边覆盖 armv7l，镜像归属覆盖已保存的六份标准清单。",
    "",
    "| 分组 | 数量 | 共同依据 |",
    "|---|---:|---|",
]
for group in group_order:
    group_lines.append(f"| {group} | {len(no_groups[group])} | {common[group]} |")
group_lines.append("")
for group in group_order:
    group_lines.extend(
        [
            f"## {group}（{len(no_groups[group])} 包）",
            "",
            common[group],
            "",
            "| 包 | 仓库用途摘要 | 镜像与工具差异 |",
            "|---|---|---|",
        ]
    )
    for row in no_groups[group]:
        differences = [row["是否进入设备镜像"], row["是否为测试诊断基准类工具"]]
        if group != "不含 C++ 载体":
            differences.append(row["理由"])
        group_lines.append(
            f"| `{esc(row['源码包名'])}` | {esc(summary(row))} | {esc('；'.join(differences))} |"
        )
    group_lines.append("")
GROUPS_MD.parent.mkdir(parents=True, exist_ok=True)
GROUPS_MD.write_text("\n".join(group_lines), encoding="utf-8")

insufficient = """# 现有证据尚不足以完成最终判定的包

## gmp

`libgmpxx.a` 与 `libgmpxx` 已证明包含/依赖 C++，`eigen`、`python-pycrypto`
仅被证明把它列为构建依赖。缺少实际链接命令、link map 和最终 ELF 的归档成员
来源，不能确认是否把 C++ 静态代码带入 libc++ 消费方。补齐这些材料后，应再按
C++ 类型、异常和所有权边分类。定案前按“可能需要改”预留资源，但不直接修改
spec。

## tensorflow2

两组大型静态归档和头文件已证明含大量 C++ 载体，`inference-engine-tflite`、
`nnstreamer`、`nntrainer` 仅被证明把它列为构建依赖。缺少详细链接命令、link map
及最终 ELF 对象来源，不能确认哪些归档成员实际进入消费方。定案前按“可能需要
改”预留资源，但不直接修改 spec。

## 其余包

没有发现 R100 已定的其余 252 个判定缺少其原判定所要求的证据。本轮没有改变
任何判定。共同覆盖限制仍然存在：精确运行时符号边只测了 armv7l；镜像归属只
覆盖六份已保存的标准清单；R100 快照之后的新 revision 需在实施时重新核对。
"""
INSUFFICIENT_MD.write_text(insufficient, encoding="utf-8")

print(f"rows={len(rewritten)}")
print("decision_counts=" + str(dict(Counter(row["判定结果"] for row in rewritten))))
print("no_change_group_counts=" + str({group: len(no_groups[group]) for group in group_order}))
print(f"comparison_rows={len(comparison)}")
print("insufficient_final_decision_packages=gmp;tensorflow2")
