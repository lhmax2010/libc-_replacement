#!/usr/bin/env python3
import csv
import re
import subprocess
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "progress/R16/tables"
OUT.mkdir(parents=True, exist_ok=True)

INPUTS = {
    "armv7l": (
        ROOT / "tmp/R16/extracted/armv7l/libgcc/usr/lib/libgcc_s.so.1",
        ROOT / "tmp/R16/extracted/armv7l/compiler-rt/usr/lib/clang/22/lib/linux/libclang_rt.builtins-arm.a",
    ),
    "aarch64": (
        ROOT / "tmp/R16/extracted/aarch64/libgcc/usr/lib64/libgcc_s.so.1",
        ROOT / "tmp/R16/extracted/aarch64/compiler-rt/usr/lib64/clang/22/lib/linux/libclang_rt.builtins-aarch64.a",
    ),
    "x86_64": (
        ROOT / "tmp/R16/extracted/x86_64/libgcc/usr/lib64/libgcc_s.so.1",
        ROOT / "tmp/R16/extracted/x86_64/compiler-rt/usr/lib64/clang/22/lib/linux/libclang_rt.builtins-x86_64.a",
    ),
}

HELPER_RE = re.compile(
    r"^(?:__aeabi_|__arm_|__gnu_(?![Uu]nwind)|__(?:absv|add|ash|atomic|bswap|clear_cache|clrsb|clz|cmp|ctz|div|enable_execute_stack|eq|extend|ffs|fix|float|gcc_bcmp|ge|get|gt|le|lshr|lt|mod|mul|ne|neg|parity|popcount|powi|sub|sync|trunc|ucmp|udiv|umod|unord))"
)


def classify(name):
    if (name.startswith("_Unwind_VRS") or name.startswith("__gnu_Unwind")
            or name.startswith("__aeabi_unwind_cpp_") or name == "__gnu_unwind_frame"):
        return "ARM_EHABI"
    if name.startswith("_Unwind") or name == "__gcc_personality_v0":
        return "UNWIND"
    if HELPER_RE.match(name):
        return "COMPILER_HELPER"
    return "OTHER"


def read_exports(path):
    text = subprocess.check_output(["readelf", "-W", "--dyn-syms", str(path)], text=True)
    rows = []
    for line in text.splitlines():
        m = re.match(r"\s*\d+:\s+\S+\s+\d+\s+(\S+)\s+(GLOBAL|WEAK)\s+(\S+)\s+(?:\[VARIANT_PCS\]\s+)?(\S+)\s+(\S+)", line)
        if not m:
            continue
        typ, bind, vis, ndx, decorated = m.groups()
        if ndx in {"UND", "ABS"}:
            continue
        decorated = decorated.split(" (")[0]
        vm = re.match(r"^(.*?)@{1,2}(.+)$", decorated)
        if vm:
            name, version = vm.groups()
        else:
            name, version = decorated, "UNVERSIONED"
        rows.append((name, version, typ, bind, vis))
    return sorted(set(rows))


def read_builtins(path):
    text = subprocess.check_output(["nm", "-g", "--defined-only", str(path)], text=True, stderr=subprocess.STDOUT)
    names = set()
    for line in text.splitlines():
        m = re.match(r"^[0-9a-fA-F]+\s+[A-Za-z]\s+(\S+)$", line.strip())
        if m:
            names.add(m.group(1))
    return names


def write_tsv(name, header, rows):
    with (OUT / name).open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n")
        w.writerow(header)
        w.writerows(rows)


summary = []
exports_all = []
versions_all = []
aeabi_all = []
other_all = []
missing_all = []
reverse_all = []

for arch, (libgcc, builtins) in INPUTS.items():
    export_rows = read_exports(libgcc)
    base_names = sorted({row[0] for row in export_rows})
    classes = {name: classify(name) for name in base_names}
    builtins_names = read_builtins(builtins)
    helper_names = {name for name in base_names if classes[name] == "COMPILER_HELPER"}
    counts = Counter(classes.values())
    denom = len(base_names)
    for category in ["UNWIND", "ARM_EHABI", "COMPILER_HELPER", "OTHER"]:
        count = counts[category]
        summary.append((arch, category, count, denom, f"{count / denom:.6%}" if denom else "NOT_AVAILABLE"))
    covered = helper_names & builtins_names
    missing = helper_names - builtins_names
    reverse = builtins_names - set(base_names)
    summary.extend([
        (arch, "HELPER_COVERED_BY_COMPILER_RT", len(covered), len(helper_names), f"{len(covered) / len(helper_names):.6%}" if helper_names else "NOT_AVAILABLE"),
        (arch, "HELPER_NOT_COVERED_BY_COMPILER_RT", len(missing), len(helper_names), f"{len(missing) / len(helper_names):.6%}" if helper_names else "NOT_AVAILABLE"),
        (arch, "COMPILER_RT_ONLY", len(reverse), len(builtins_names), f"{len(reverse) / len(builtins_names):.6%}" if builtins_names else "NOT_AVAILABLE"),
    ])
    for name in base_names:
        exports_all.append((arch, name, classes[name]))
        if name.startswith("__aeabi_"):
            aeabi_all.append((arch, name, "YES" if name in builtins_names else "NO"))
        if classes[name] == "OTHER":
            other_all.append((arch, name))
    for name, version, typ, bind, vis in export_rows:
        versions_all.append((arch, name, version, classes[name], typ, bind, vis))
    for name in sorted(missing):
        missing_all.append((arch, name))
    for name in sorted(reverse):
        reverse_all.append((arch, name))

write_tsv("libgcc_export_classification.tsv", ["arch", "symbol", "category"], exports_all)
write_tsv("libgcc_export_summary.tsv", ["arch", "category", "numerator", "denominator", "ratio"], summary)
write_tsv("libgcc_symbol_versions.tsv", ["arch", "symbol", "version_node", "category", "type", "bind", "visibility"], versions_all)
write_tsv("armv7l_aeabi_symbols.tsv", ["arch", "symbol", "compiler_rt_exact_name_match"], aeabi_all)
write_tsv("libgcc_other_symbols.tsv", ["arch", "symbol"], other_all)
write_tsv("compiler_rt_missing_helpers.tsv", ["arch", "libgcc_helper_not_in_compiler_rt"], missing_all)
write_tsv("compiler_rt_only_symbols.tsv", ["arch", "compiler_rt_symbol_not_exported_by_libgcc_s"], reverse_all)

version_counts = Counter((arch, version, category) for arch, _name, version, category, *_ in versions_all)
write_tsv(
    "libgcc_version_node_distribution.tsv",
    ["arch", "version_node", "category", "symbol_version_pairs"],
    sorted((arch, version, category, count) for (arch, version, category), count in version_counts.items()),
)
