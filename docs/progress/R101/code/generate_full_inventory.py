#!/usr/bin/env python3
import csv
import sys
from collections import defaultdict
from pathlib import Path


def read_tsv(path):
    with open(path, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))


if len(sys.argv) != 7:
    raise SystemExit(
        "usage: generate_full_inventory.py SOURCE_RECORDS BINARY_RECORDS "
        "R100_DECISIONS IMAGE_DIR OUTPUT_TSV OUTPUT_MD"
    )

source_path, binary_path, decision_path, image_dir, output_tsv, output_md = map(
    Path, sys.argv[1:]
)
source_rows = read_tsv(source_path)
binary_rows = read_tsv(binary_path)
decision_rows = read_tsv(decision_path)
decisions = {row["source_package"]: row for row in decision_rows}

sources = defaultdict(list)
for row in source_rows:
    sources[row["name"]].append(row)

sourcerpm_to_source = {row["location"]: row["name"] for row in source_rows}
direct_libstdcpp_binaries = defaultdict(set)
for row in binary_rows:
    if "libstdc++.so" in row["requires"]:
        source = sourcerpm_to_source.get(row["sourcerpm"])
        if source:
            direct_libstdcpp_binaries[source].add(row["name"])

image_by_vcs = defaultdict(set)
for image_file in sorted(image_dir.glob("*.packages")):
    image = image_file.name.removesuffix(".packages")
    with open(image_file, encoding="utf-8") as f:
        for line in f:
            fields = line.rstrip("\n").split()
            if len(fields) >= 3 and "#" in fields[-1]:
                image_by_vcs[fields[-1]].add(image)

tool_roles = {
    "app-rootstrap-checker": "rootstrap 完整性检查器",
    "bcc-tools": "BPF 性能分析、监控与跟踪工具",
    "binutils": "含 readelf/objdump 等二进制检查与诊断工具",
    "binutils-aarch64": "交叉架构二进制检查与诊断工具",
    "binutils-armv7hl": "交叉架构二进制检查与诊断工具",
    "binutils-armv7l": "交叉架构二进制检查与诊断工具",
    "binutils-riscv64": "交叉架构二进制检查与诊断工具",
    "boost": "产出 boost-test 测试框架子包",
    "bpftrace": "eBPF 跟踪工具",
    "build-compare": "构建结果比较工具",
    "dejagnu": "测试框架",
    "elfutils": "ELF 检查与诊断工具集",
    "expect": "交互程序自动化/测试工具",
    "gdb": "调试器",
    "golang": "产出 golang-tests 测试子包",
    "hal-rootstrap-checker": "HAL rootstrap 完整性检查器",
    "lttng-ust": "用户态跟踪设施",
    "libunwind": "产出 libunwind-tests 测试子包",
    "llvm": "产出 lldb 调试器子包",
    "perl-Test-Pod": "测试模块",
    "perl-Test-Pod-Coverage": "测试/覆盖率模块",
    "procps-ng": "进程监控与诊断工具",
    "python-mock": "测试 mock 库",
    "python-nose": "测试框架",
    "python-unittest2": "测试框架",
    "python3-nose": "测试框架",
    "python3-base": "产出 python3-testsuite 子包",
    "qemu-accel": "交叉构建加速/模拟工具",
    "qemu-linux-user": "用户态模拟器",
}

decision_zh = {
    "NEED_LIBCXX": "需要改",
    "NO_LIBCXX_NEEDED": "不需要改",
    "CANNOT_SWITCH_CURRENTLY": "当前不能改",
    "UNDETERMINED": "未定",
}


def reason_for(row):
    decision = row["decision"]
    cross = row["unified_runtime_edge_classes"]
    internal = row["base_internal_edge_classes_as_consumer"]
    rule = row["matched_rules"]
    if decision == "NEED_LIBCXX":
        if "LAYOUT_SENSITIVE" in internal and cross == "NONE":
            return "以布局敏感的 C++ ABI 消费 Base 内已需迁移的 LLVM，必须留在同一 libc++ 闭包。"
        if "LAYOUT_SENSITIVE" in cross:
            return "与 Unified 消费方存在已证明传递布局敏感 C++ 类型的运行期边。"
        return "与 Unified 消费方存在已证明的 C++ ABI 边，现有证据不能排除对象布局耦合。"
    if decision == "CANNOT_SWITCH_CURRENTLY":
        return "当前 spec/产品角色明确使用 GCC；这是当前配置约束，不是永久技术限制。"
    if decision == "UNDETERMINED":
        return "含真实 C++ 静态归档且有 Unified 构建依赖方，但尚无实际链接证据。"
    if rule == "NO_EXACT_EDGES_ONLY_C_OR_NO_INTERSECTION":
        return "精确符号交集证明跨项目运行期边仅为 C 接口或无符号交集，不传递 C++ 对象。"
    if rule == "NO_HEADER_PAYLOAD_COMPILED_BY_CONSUMER":
        return "只提供由消费方自行实例化的 C++ 头文件，本包不形成独立运行时边界。"
    if rule == "NO_CPP_STATIC_ARCHIVE_WITHOUT_UNIFIED_CONSUMER":
        return "虽有 C++ 静态归档，但精确依赖拓扑中没有迁移侧消费方。"
    if rule == "NO_ALREADY_EXPLICIT_LIBCXX_RUNTIME_BUILD":
        return "本包就是显式构建 libc++/libc++abi 的运行时包，不靠 Base 全局选项切换自身。"
    if rule == "NO_BASE_ONLY_OR_INDEPENDENT_PRODUCT_NO_UNIFIED_RUNTIME_EDGE":
        return "产出 C++，但精确拓扑未形成通向 Unified 的运行期 C++ 语义边，属于 Base 闭包或独立工具。"
    if rule == "NO_BUILD_EXPOSURE_WITHOUT_SHIPPED_CPP_BOUNDARY":
        if row["six_image_membership"] != "NONE":
            return "构建中虽出现 C++ 动作，但已发布产物不含 C++ 运行时边界；入镜像不等于交换 C++ 语义。"
        return "C++ 仅见于构建过程，已发布产物不含可跨项目使用的 C++ 运行时边界。"
    raise RuntimeError(f"unhandled rule {rule}")


rows = []
for name in sorted(sources):
    records = sources[name]
    vcs_values = sorted({r["vcs"] for r in records if r["vcs"]})
    sourcerpms = sorted({r["location"] for r in records})
    summaries = sorted({r["summary"] for r in records if r["summary"]})
    images = sorted({img for vcs in vcs_values for img in image_by_vcs.get(vcs, set())})
    image_value = "是：" + ";".join(images) if images else "否：六个精确镜像清单无该 VCS"
    tool_value = (
        "是：" + tool_roles[name]
        if name in tool_roles
        else "否：仓库元数据的主要用途不是测试/诊断/基准"
    )
    if name not in decisions:
        cpp = "否：R100 四路完整候选扫描未命中"
        relationship = "无关"
        decision = "不需要改"
        reason = "该快照的构建日志与产物扫描均未发现 C++ 编译、ABI、运行时、静态归档或头文件载体。"
        evidence = (
            "R100 分母扫描未进入 73 个 C++ 候选；"
            f"source={';'.join(sourcerpms)}；VCS={';'.join(vcs_values) or 'NOT_AVAILABLE'}"
        )
    else:
        r = decisions[name]
        cpp = "是：" + r["denominator_evidence"]
        if direct_libstdcpp_binaries.get(name):
            relationship = "直接链接 libstdc++"
        elif r["matched_rules"] == "NO_ALREADY_EXPLICIT_LIBCXX_RUNTIME_BUILD":
            relationship = "自身提供 libc++/libc++abi，与 libstdc++ 无直接链接"
        elif (
            r["matched_rules"] == "NO_EXACT_EDGES_ONLY_C_OR_NO_INTERSECTION"
            and "PURE_C_INTERFACE" in (
                r["unified_runtime_edge_classes"]
                + r["base_internal_edge_classes_as_consumer"]
            )
        ):
            relationship = "仅通过 C 接口间接相关"
        elif r["matched_rules"] == "NO_HEADER_PAYLOAD_COMPILED_BY_CONSUMER":
            relationship = "仅构建期使用，产物不含 C++ 运行时"
        elif r["matched_rules"] == "NO_BUILD_EXPOSURE_WITHOUT_SHIPPED_CPP_BOUNDARY":
            relationship = "仅构建期使用，产物不含 C++ 运行时"
        else:
            relationship = "含 C++ 但不与外部交换 C++ 语义"
        decision = decision_zh[r["decision"]]
        reason = reason_for(r)
        evidence = (
            f"R100={r['decision']}/{r['matched_rules']}；"
            f"直接libstdc++二进制={';'.join(sorted(direct_libstdcpp_binaries.get(name, set()))) or 'NONE'}；"
            f"Unified边={r['unified_runtime_edge_classes']}；"
            f"Base内部消费边={r['base_internal_edge_classes_as_consumer']}；"
            f"VCS={r['source_vcs']}"
        )
    if summaries:
        evidence += f"；Summary={summaries[0]}"
    rows.append(
        {
            "源码包名": name,
            "是否含C++及依据": cpp,
            "与libstdc++的关系": relationship,
            "判定结果": decision,
            "理由": reason,
            "证据": evidence,
            "是否进入设备镜像": image_value,
            "是否为测试诊断基准类工具": tool_value,
        }
    )

fields = list(rows[0])
output_tsv.parent.mkdir(parents=True, exist_ok=True)
with open(output_tsv, "w", newline="", encoding="utf-8") as f:
    writer = csv.DictWriter(
        f, delimiter="\t", fieldnames=fields, lineterminator="\n"
    )
    writer.writeheader()
    writer.writerows(rows)

with open(output_md, "w", encoding="utf-8") as f:
    f.write("# Tizen-Base-Toolchain 全部源码包 libc++ 判定清单\n\n")
    f.write(
        "数据锚点：`tizen-base-toolchain_20260828.101647`；按源码包名去重。"
        "表中的“不需要改”沿用 R100 的 55 项结论；其余 181 项是 R100 四路完整候选扫描均未命中的非 C++ 包。\n\n"
    )
    f.write("| " + " | ".join(fields) + " |\n")
    f.write("|" + "---|" * len(fields) + "\n")
    for row in rows:
        values = [row[field].replace("|", "\\|").replace("\n", " ") for field in fields]
        f.write("| " + " | ".join(values) + " |\n")

print(f"source_packages={len(rows)}")
print(f"cpp_packages={sum(name in decisions for name in sources)}")
for label in ("需要改", "不需要改", "当前不能改", "未定"):
    print(f"decision_{label}={sum(row['判定结果'] == label for row in rows)}")
print(f"image_yes={sum(row['是否进入设备镜像'].startswith('是：') for row in rows)}")
print(f"tool_yes={sum(row['是否为测试诊断基准类工具'].startswith('是：') for row in rows)}")
