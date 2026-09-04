#!/usr/bin/env python3
import csv
import re
import sys
from collections import defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path("/home/toolchain/development/libc++_replacement")
SOURCE = Path("/tmp/r114_input_20260904/hq_20260904/base_packages_final.tsv")
OUT_DIR = Path("/tmp/r114_output_20260904/hq_20260904")
OUT_TSV = OUT_DIR / "base_packages_final.tsv"
OUT_MD = OUT_DIR / "base_packages_final.md"
R100 = ROOT / "docs/progress/R100/tables/base_cpp_package_decisions.tsv"
EDGES = ROOT / "docs/progress/R100/tables/armv7l_cross_edges/actual_cross_source_edge_classification.tsv"
BASE_SOURCES = ROOT / "progress/R98/tables/base_source_records.tsv"
UNIFIED_SOURCES = ROOT / "progress/R98/tables/unified_source_records.tsv"

HEADERS = [
    "Source package",
    "C++ presence and basis",
    "Relationship to libstdc++",
    "Decision",
    "Rationale",
    "Evidence",
    "Included in device image",
    "Test/diagnostic/benchmark tool",
    "Current implementation status",
]

PURPOSE_OVERRIDES = {
    "abseil-cpp": "a set of foundational libraries reused by other C++ projects",
    "bcc-tools": "BPF-based performance analysis, monitoring, and tracing tools together with the libbcc runtime",
    "boost": "a general-purpose collection of C++ libraries reused by many projects",
    "bpftrace": "a high-level tracing tool based on eBPF",
    "icu": "a foundational library providing Unicode and internationalization support",
    "jsoncpp": "a C++ library for reading and writing JSON",
    "libsigc++": "a C++ library providing type-safe signals and callbacks",
    "llvm": "shared compiler infrastructure for compilers, linkers, debuggers, and related tools",
    "pcre": "a foundational Perl-compatible regular-expression library that also contains a C++ interface carrier",
    "taglib": "a library for reading and writing audio metadata",
    "bison": "a build-time tool that generates parser source code from grammar rules",
    "cereal": "a header-only C++ serialization library included and instantiated directly by consumers",
    "gmp": "a foundational library for multiple-precision integer, floating-point, and rational arithmetic",
    "tensorflow2": "a package providing the TensorFlow machine-learning runtime, static libraries, and development interfaces",
    "libcxx-runtimes": "the package that provides the libc++ and libc++abi runtime and development files",
}

SIGNALS = {
    "ACTUAL_CPP_BUILD_LOG": "a successful build log contains C++ compilation",
    "CPP_RUNTIME_DEPENDENCY": "a published RPM has a direct C++ runtime dependency",
    "GLOBAL_DEMANGLED_CPP_IN_STATIC_ARCHIVE": "a static archive exports globally visible demangleable C++ symbols",
    "CPP_HEADER_PAYLOAD": "a published package contains a C++ header carrier",
}

RELATION = {
    "无关": "unrelated",
    "直接链接 libstdc++": "directly links libstdc++",
    "仅构建期使用，产物不含 C++ 运行时": "used only at build time; delivered artifacts contain no C++ runtime",
    "含 C++ 但不与外部交换 C++ 语义": "contains C++ but does not exchange C++ semantics externally",
    "自身提供 libc++/libc++abi，与 libstdc++ 无直接链接": "provides libc++/libc++abi itself and does not directly link libstdc++",
}

DECISION = {
    "需要改": "requires adaptation",
    "不需要改": "does not require adaptation",
    "当前不能改": "currently cannot be adapted",
}

TOOL = {
    "否：仓库元数据的主要用途不是测试/诊断/基准": "No: repository metadata does not identify testing, diagnostics, or benchmarking as the primary purpose",
    "是：交叉架构二进制检查与诊断工具": "Yes: cross-architecture binary inspection and diagnostic tools",
    "是：测试框架": "Yes: test framework",
    "是：BPF 性能分析、监控与跟踪工具": "Yes: BPF performance analysis, monitoring, and tracing tools",
    "是：ELF 检查与诊断工具集": "Yes: ELF inspection and diagnostic tool suite",
    "是：HAL rootstrap 完整性检查器": "Yes: HAL rootstrap integrity checker",
    "是：eBPF 跟踪工具": "Yes: eBPF tracing tool",
    "是：rootstrap 完整性检查器": "Yes: rootstrap integrity checker",
    "是：交互程序自动化/测试工具": "Yes: interactive-program automation and testing tool",
    "是：交叉构建加速/模拟工具": "Yes: cross-build acceleration and emulation tool",
    "是：产出 boost-test 测试框架子包": "Yes: produces the boost-test framework subpackage",
    "是：产出 golang-tests 测试子包": "Yes: produces the golang-tests test subpackage",
    "是：产出 libunwind-tests 测试子包": "Yes: produces the libunwind-tests test subpackage",
    "是：产出 lldb 调试器子包": "Yes: produces the lldb debugger subpackage",
    "是：产出 python3-testsuite 子包": "Yes: produces the python3-testsuite subpackage",
    "是：含 readelf/objdump 等二进制检查与诊断工具": "Yes: contains binary inspection and diagnostic tools such as readelf and objdump",
    "是：构建结果比较工具": "Yes: build-result comparison tool",
    "是：测试 mock 库": "Yes: testing mock library",
    "是：测试/覆盖率模块": "Yes: testing and coverage module",
    "是：测试模块": "Yes: testing module",
    "是：用户态模拟器": "Yes: user-mode emulator",
    "是：用户态跟踪设施": "Yes: user-space tracing facility",
    "是：调试器": "Yes: debugger",
    "是：进程监控与诊断工具": "Yes: process monitoring and diagnostic tool",
}

RULE_PUBLIC = {
    "N1_EXACT_CPP_RUNTIME_EDGE": "a verified cross-package C++ runtime edge exists",
    "C1_CURRENT_GCC_OVERRIDE_OR_GCC_ROLE": "currently explicitly uses GCC or performs a GNU toolchain role",
    "NO_ALREADY_EXPLICIT_LIBCXX_RUNTIME_BUILD": "the package already explicitly builds the libc++ runtime and needs no duplicate migration option",
    "NO_BASE_ONLY_OR_INDEPENDENT_PRODUCT_NO_UNIFIED_RUNTIME_EDGE": "the measured topology is Base-only or the product runs independently, with no C++ runtime edge to the migrating set",
    "NO_BUILD_EXPOSURE_WITHOUT_SHIPPED_CPP_BOUNDARY": "C++ occurs only during the build or in a carrier compiled by the consumer; the package delivers no C++ boundary fixed to its own standard library",
    "NO_CPP_STATIC_ARCHIVE_WITHOUT_UNIFIED_CONSUMER": "a static archive contains C++, but no verified consumer in the migrating set absorbs its C++ members",
    "NO_EXACT_EDGES_ONLY_C_OR_NO_INTERSECTION": "the measured runtime edges expose only C interfaces or have no visible symbol intersection",
    "NO_HEADER_PAYLOAD_COMPILED_BY_CONSUMER": "the package provides only a C++ header carrier compiled by the consumer, so the consumer selects the standard library",
}


def read_tsv(path: Path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def record_map(path: Path):
    return {row["location"]: row["name"] for row in read_tsv(path) if row["kind"] == "source"}


source_rows = read_tsv(SOURCE)
r100_rows = read_tsv(R100)
r100 = {row["source_package"]: row for row in r100_rows}
base_sources = record_map(BASE_SOURCES)
unified_sources = record_map(UNIFIED_SOURCES)
runtime_edges = defaultdict(list)
for edge in read_tsv(EDGES):
    provider = base_sources[edge["provider_sourcerpm"]]
    runtime_edges[provider].append({
        "consumer": unified_sources[edge["consumer_sourcerpm"]],
        "classification": edge["classification"],
    })


def summary(row):
    match = re.search(r"(?:^|；)Summary=(.*)$", row["证据"])
    if not match:
        raise ValueError(f"missing Summary for {row['源码包名']}")
    return match.group(1).strip()


def purpose(row):
    name = row["源码包名"]
    if name in PURPOSE_OVERRIDES:
        return f"`{name}` is {PURPOSE_OVERRIDES[name]} (repository summary: \"{summary(row)}\"). "
    return f"The repository summary defines `{name}` as \"{summary(row)}\". "


def image_sentence(row):
    if row["是否进入设备镜像"].startswith("是："):
        return "Its exact source revision matches at least one of the verified standard device-image manifests. "
    return "Its source revision is absent from all six verified standard device-image manifests. "


def joined(names):
    return ", ".join(f"`{name}`" for name in sorted(set(names)))


def need_reason(row):
    name = row["源码包名"]
    edges = [edge for edge in runtime_edges.get(name, []) if edge["classification"] in {"LAYOUT_SENSITIVE_STD_TYPE", "CPP_ABI_NO_STD_LAYOUT_PROVEN"}]
    layout = [edge["consumer"] for edge in edges if edge["classification"] == "LAYOUT_SENSITIVE_STD_TYPE"]
    cpp_only = [edge["consumer"] for edge in edges if edge["classification"] == "CPP_ABI_NO_STD_LAYOUT_PROVEN"]
    parts = [purpose(row)]
    if edges:
        parts.append("Actual armv7l ELF dependencies and symbol intersections verify that Unified-side packages in the migrating set call it through the C++ ABI at runtime: " + joined(edge["consumer"] for edge in edges) + ". ")
        if layout:
            parts.append("The edges from " + joined(layout) + " have been directly verified to transfer layout-sensitive standard-library types. ")
        if cpp_only:
            verb = "has" if len(set(cpp_only)) == 1 else "have"
            parts.append(joined(cpp_only) + f" also {verb} verified C++ ABI coupling, although the available symbols do not prove a concrete layout transfer. ")
    if name in {"bcc-tools", "bpftrace"}:
        parts.append("The measured armv7l Base-internal topology also shows it calling `llvm` through a layout-sensitive C++ interface; `llvm` belongs to the same required migration set. ")
    elif name == "llvm":
        parts.append("In addition, Base-internal `bcc-tools`, `bpftrace`, and LLVM's own tools call LLVM/Clang libraries through layout-sensitive C++ interfaces. ")
    if layout or name in {"bcc-tools", "bpftrace", "llvm"}:
        parts.append("If this package continues to link libstdc++, these calls cross two standard-library implementations. The linker does not prevent that combination, and object layout, ownership, or destruction may be interpreted according to the wrong implementation; therefore the package must be switched together with its consumers. ")
    else:
        parts.append("If this package continues to link libstdc++, the measured C++ ABI calls retain a cross-standard-library boundary. That ABI combination does not guarantee consistent type, exception, or ownership semantics; therefore the package must be switched together with its consumers. ")
    return "".join(parts).strip()


def non_cpp_reason(row):
    return (purpose(row)
        + "Across the available three-architecture evidence, none of the four carrier checks matched: the build logs contain no C++ compilation, RPM metadata has no C++ runtime dependency, static archives expose no globally visible demangleable C++ symbols, and file lists contain no C++ header carrier. "
        + image_sentence(row)
        + "Within those stated detection boundaries, the package has no C++ build or delivery boundary on which libc++ versus libstdc++ must be selected, so no standard-library switch option is required.").strip()


def build_only_reason(row, evidence):
    signals = evidence["denominator_evidence"].split(";")
    parts = [purpose(row)]
    if "ACTUAL_CPP_BUILD_LOG" in signals:
        parts.append("Actual build logs contain C++ compilation. ")
    if "CPP_HEADER_PAYLOAD" in signals:
        parts.append(f"Published packages also contain {evidence['cpp_header_rpm_count']} architecture-specific instance(s) of C++ header carriers; those headers are compiled by each consumer's own compiler and standard library. ")
    else:
        parts.append("Published packages contain no C++ header carrier. ")
    parts.append(f"Package-level checks also report a direct C++ runtime dependency of \"{evidence['dynamic_cpp_runtime_signal']}\" and {evidence['cpp_static_archive_count']} static archive(s) with globally visible C++ symbols; the exact armv7l runtime graph has 0 C++ edge(s) to Unified. ")
    if "PURE_C_INTERFACE" in evidence["base_internal_edge_classes_as_consumer"]:
        parts.append("Measured cross-package calls within Base also use only C interfaces. ")
    parts.append(image_sentence(row))
    parts.append("Thus C++ occurs only during the build or in a non-runtime carrier compiled by the consumer; it does not form a cross-package C++ ABI boundary whose implementation is fixed by this package, so this package does not need to be switched.")
    return "".join(parts)


def independent_reason(row, evidence):
    parts = [purpose(row), "Its ELF artifacts directly depend on libstdc++, but the complete exact armv7l runtime graph measured 0 runtime edge(s) to the Unified migrating set. "]
    internal = evidence["base_internal_edge_classes_as_consumer"]
    if internal == "NONE":
        parts.append("It also has 0 Base-internal cross-package C++ semantic edge(s). ")
    elif "PURE_C_INTERFACE" in internal:
        parts.append("The only relevant Base-internal calls share C-interface symbols and do not transfer C++ objects. ")
    else:
        raise ValueError(f"unexpected Base-internal classification for {row['源码包名']}: {internal}")
    parts.append(image_sentence(row))
    if row["是否为测试诊断基准类工具"].startswith("是："):
        parts.append("Repository metadata also identifies it as a test, diagnostic, or build tool that runs independently in the measured topology. ")
    parts.append("Although the package itself uses libstdc++, it therefore forms no identified mixed-standard-library C++ object boundary. Switching it would close no additional measured risk edge, so it does not require adaptation.")
    return "".join(parts)


def c_interface_reason(row, evidence):
    consumers = [edge["consumer"] for edge in runtime_edges[row["源码包名"]]]
    internal = " Measured Base-internal calls likewise share only C interfaces." if "PURE_C_INTERFACE" in evidence["base_internal_edge_classes_as_consumer"] else ""
    return (purpose(row)
        + "Some of its ELF artifacts directly depend on libstdc++, but actual armv7l dependencies and symbol intersections show the Unified-side callers to be " + joined(consumers) + "; all cross-package shared symbols are C interfaces, while the other analyzed edges have no C++ symbol intersection."
        + internal + " " + image_sentence(row)
        + "A C interface transfers only C types and does not depend on either C++ standard library's object layout, exceptions, or destruction rules. Different standard libraries on the two sides therefore do not change this measured boundary, so the package does not require adaptation.").strip()


def static_archive_reason(row, evidence):
    return (purpose(row)
        + f"The package does contain {evidence['cpp_static_archive_count']} architecture-specific instance(s) of static archives with globally visible demangleable C++ symbols, but the exact armv7l dependency graph has 0 Unified or Base-internal runtime consumer edge(s) to those artifacts. "
        + image_sentence(row)
        + "Within the measured topology, this C++ code is not absorbed by a consumer in the migrating set into the same process and does not transfer C++ semantics across packages. Switching the package would close no additional boundary, so it does not require adaptation.").strip()


def header_reason(row, evidence):
    extra = "As a build tool, it generates source code from input grammars and does not provide a C++ runtime library. " if row["源码包名"] == "bison" else ""
    return (purpose(row) + extra
        + f"The C++ match comes from {evidence['cpp_header_rpm_count']} architecture-specific instance(s) of header carriers. Published packages have a direct C++ runtime dependency of \"NO\", contain 0 static archive(s) with globally visible C++ symbols, and have 0 Unified or Base C++ edge(s) in the armv7l runtime graph. "
        + image_sentence(row)
        + "These headers are ultimately instantiated by consumers using their own toolchains, so they naturally inherit the consumer's libc++ selection; no separate standard-library switch is required for this package.").strip()


def already_libcxx_reason(row):
    return (purpose(row)
        + "It builds and installs the target libc++/libc++abi implementation itself and does not directly link libstdc++. This per-package migration applies to other C++ packages that still consume libstdc++; adding a consumer-side `-stdlib=libc++` option again to the runtime provider would not constitute a migration, so this package does not require adaptation.").strip()


def cannot_reason(row, evidence):
    name = row["源码包名"]
    role = "the GCC compiler and its target-architecture toolchain" if name.startswith("gcc") else "the construction of the GNU C runtime"
    if evidence["dynamic_cpp_runtime_signal"] == "YES":
        runtime = "Published C++ ELF artifacts from it directly depend on libstdc++. "
        island = "It therefore remains a libstdc++ toolchain island; any future edge that exchanges C++ objects with libc++ code must be reassessed."
    else:
        runtime = "Its published artifacts have no direct C++ runtime dependency; C++ appears only in build actions. "
        island = "It remains in a GCC toolchain build role, but its current artifacts do not form a libstdc++ C++ ABI island. If future artifacts acquire a C++ runtime and exchange objects with libc++ code, they must be reassessed."
    return (purpose(row)
        + f"The current spec and product role require {role}, so it cannot be switched to libc++ in this round. This is a constraint of the current configuration and product responsibility, not a permanent technical conclusion. "
        + runtime
        + "The exact armv7l graph reports both the C++ runtime edges to Unified and the Base-internal C++ semantic edges as 0; within the covered scope, no mixed-standard-library object boundary has been established. "
        + image_sentence(row) + island).strip()


def gmp_reason():
    return ("`gmp` is a foundational library for multiple-precision integer, floating-point, and rational arithmetic, with a separate C++ wrapper library, `libgmpxx`. Actual consumer link steps and final artifacts were traced: `eigen` installs only headers and CMake configuration and has no final ELF; the actual link commands for `python-pycrypto` on all three architectures use `-lgmp`, not `-lgmpxx`; the generated `_fastmath.so` depends only on `libgmp.so.3`, not on `libgmpxx` or `libstdc++.so.6`; and the symbol intersection between its 18 extensions and `libgmpxx.a` is 0. Therefore, within the verified consumer scope, no C++ static code is incorporated into artifacts that will migrate to libc++, and this package does not require adaptation.")


def tensorflow_reason():
    return ("`tensorflow2` provides TensorFlow Lite/XLA AOT static archives and C++ development interfaces. Tracing three real consumer classes verified that `inference-engine-tflite`, `nnstreamer`, and `nntrainer` all use `libtensorflow2-lite.a` in their actual link commands on all three architectures. Across 20 consumer ELF/architecture combinations, 5,312 strong archive symbols matched, involving at least 546 archive members. The interfaces also hold and transfer layout-sensitive C++ types such as `std::unique_ptr<tflite::Interpreter>` and `std::unique_ptr<tflite::FlatBufferModel>`. Static-archive code is copied directly into consumers that will migrate to libc++; continuing to build it against libstdc++ would create an actual mixed-runtime C++ boundary, so this package requires adaptation.")


def make_reason(row):
    name = row["源码包名"]
    if name == "gmp": return gmp_reason()
    if name == "tensorflow2": return tensorflow_reason()
    if row["是否含C++及依据"].startswith("否："): return non_cpp_reason(row)
    evidence = r100[name]
    rule = evidence["matched_rules"]
    return {
        "N1_EXACT_CPP_RUNTIME_EDGE": lambda: need_reason(row),
        "NO_BUILD_EXPOSURE_WITHOUT_SHIPPED_CPP_BOUNDARY": lambda: build_only_reason(row, evidence),
        "NO_BASE_ONLY_OR_INDEPENDENT_PRODUCT_NO_UNIFIED_RUNTIME_EDGE": lambda: independent_reason(row, evidence),
        "NO_EXACT_EDGES_ONLY_C_OR_NO_INTERSECTION": lambda: c_interface_reason(row, evidence),
        "NO_CPP_STATIC_ARCHIVE_WITHOUT_UNIFIED_CONSUMER": lambda: static_archive_reason(row, evidence),
        "NO_HEADER_PAYLOAD_COMPILED_BY_CONSUMER": lambda: header_reason(row, evidence),
        "NO_ALREADY_EXPLICIT_LIBCXX_RUNTIME_BUILD": lambda: already_libcxx_reason(row),
        "C1_CURRENT_GCC_OVERRIDE_OR_GCC_ROLE": lambda: cannot_reason(row, evidence),
    }[rule]()


def cpp_presence(value):
    if value.startswith("否："):
        return "No: none of the four C++ candidate evidence classes matched (subject to the detection boundaries stated in the methodology)"
    tokens = value.removeprefix("是：").split(";")
    return "Yes: " + "; ".join(SIGNALS[token] for token in tokens)


def image_value(value):
    if value.startswith("否："):
        return "No: the VCS revision is absent from all six exact image manifests"
    return "Yes: " + value.removeprefix("是：")


def status_value(value):
    if value == "不适用": return "not applicable"
    if value == "暂停（等待 MLGO/XLA AOT 资产重建）": return "paused (pending rebuild of MLGO/XLA AOT assets)"
    match = re.fullmatch(r"已完成并推送（([0-9a-f]{40})）", value)
    if match: return f"completed and pushed ({match.group(1)})"
    raise ValueError(f"unknown status: {value}")


def translate_classifications(value):
    if value == "NONE": return "none"
    mapped = []
    for item in value.split(";"):
        if item.startswith("CPP_ABI_NO_STD_LAYOUT_PROVEN="):
            mapped.append("verified C++ ABI coupling without proven concrete standard-library layout transfer=" + item.split("=", 1)[1])
        elif item.startswith("LAYOUT_SENSITIVE_STD_TYPE="):
            mapped.append("proven layout-sensitive standard-library type transfer=" + item.split("=", 1)[1])
        elif item == "PURE_C_INTERFACE": mapped.append("C interfaces only")
        elif item.startswith("PURE_C_INTERFACE="):
            mapped.append("C interfaces only=" + item.split("=", 1)[1])
        elif item == "OTHER_NO_SYMBOL_INTERSECTION": mapped.append("no visible symbol intersection")
        elif item.startswith("OTHER_NO_SYMBOL_INTERSECTION="):
            mapped.append("no visible symbol intersection=" + item.split("=", 1)[1])
        else: raise ValueError(f"unknown edge classification: {item}")
    return ";".join(mapped)


def translate_evidence(row):
    name = row["源码包名"]
    if name == "gmp":
        return ("Evidence from `tables/consumer_linkage_summary.tsv` and `tables/provider_observation_summary.tsv`: eigen has no final ELF; python-pycrypto links `-lgmp` on all three architectures; `_fastmath.so` has libgmp.so.3 in DT_NEEDED; the strong-symbol intersection between 18 extensions and libgmpxx.a is 0; Base VCS=product/upstream/gmp#490fe651469987ccb00b5cba908d69609345e226")
    if name == "tensorflow2":
        return ("Evidence from `tables/consumer_linkage_summary.tsv`, `tables/archive_member_attribution.tsv`, and `tables/provider_observation_summary.tsv`: 3 consumers × 3 architectures actually link libtensorflow2-lite.a; 20 ELF/architecture combinations contain 5,312 distinct strong symbols from at least 546 archive members; the Base and Unified archives have identical match sets for 20/20 of these consumer ELFs; Base VCS=platform/upstream/tensorflow2#1cdba73549f741720f11dd53da1bb516b03763a2")
    original = row["证据"]
    if original.startswith("R100 分母扫描未进入 73 个 C++ 候选；"):
        return original.replace("R100 分母扫描未进入 73 个 C++ 候选；", "Not included in the 73-package candidate union because none of the four C++ candidate evidence classes matched (subject to the detection boundaries stated in the methodology); ").replace("；", "; ")
    parts = original.split("；")
    out = []
    for part in parts:
        if part.startswith("R100="):
            rule = part.split("/", 1)[1]
            out.append(RULE_PUBLIC[rule])
        elif part.startswith("直接libstdc++二进制="):
            value = part.split("=", 1)[1]
            out.append("direct libstdc++ binaries=" + ("none" if value == "NONE" else value))
        elif part.startswith("Unified边="):
            out.append("Unified edges=" + translate_classifications(part.split("=", 1)[1]))
        elif part.startswith("Base内部消费边="):
            out.append("Base-internal consumer edges=" + translate_classifications(part.split("=", 1)[1]))
        elif part.startswith("VCS=") or part.startswith("Summary="):
            out.append(part)
        else:
            raise ValueError(f"unknown evidence component for {name}: {part}")
    return "; ".join(out)


translated = []
for row in source_rows:
    translated.append({
        HEADERS[0]: row["源码包名"],
        HEADERS[1]: cpp_presence(row["是否含C++及依据"]),
        HEADERS[2]: RELATION[row["与libstdc++的关系"]],
        HEADERS[3]: DECISION[row["判定结果"]],
        HEADERS[4]: make_reason(row),
        HEADERS[5]: translate_evidence(row),
        HEADERS[6]: image_value(row["是否进入设备镜像"]),
        HEADERS[7]: TOOL[row["是否为测试诊断基准类工具"]],
        HEADERS[8]: status_value(row["当前实施状态"]),
    })

with OUT_TSV.open("w", encoding="utf-8", newline="") as stream:
    writer = csv.DictWriter(stream, fieldnames=HEADERS, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(translated)

def esc(value):
    return value.replace("|", "\\|").replace("\n", "<br>")

with OUT_MD.open("w", encoding="utf-8") as stream:
    stream.write("# libc++ adaptation decisions for all Tizen-Base-Toolchain source packages\n\n")
    stream.write("This table covers 254 deduplicated source packages in the fixed snapshot and records the final decision, evidence, and current implementation status. The accompanying methodology explains the evidence scope and detection boundaries. A value of **No** in the C++ column means that none of the four candidate evidence classes matched; it does not prove that the source contains no C++ under every possible detection method.\n\n")
    stream.write("| " + " | ".join(HEADERS) + " |\n")
    stream.write("|" + "|".join("---" for _ in HEADERS) + "|\n")
    for row in translated:
        stream.write("| " + " | ".join(esc(row[h]) for h in HEADERS) + " |\n")

print(f"source_rows={len(source_rows)}")
print(f"translated_rows={len(translated)}")
print(OUT_TSV)
print(OUT_MD)
