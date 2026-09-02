#!/usr/bin/env python3
"""Produce concise package, edge, image, and pending-evidence decision tables."""

import csv
import sys
from collections import Counter, defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path("progress/R98")
TABLES = ROOT / "tables"
GCC_KEYS = [
    ("base", "gcc"), ("base", "gcc-aarch64"), ("base", "gcc-armv7hl"),
    ("base", "gcc-armv7l"), ("base", "gcc-riscv64"), ("base", "glibc"),
    ("unified", "lapack"), ("unified", "nntrainer"),
    ("unified", "onnxruntime"), ("unified", "openblas"), ("unified", "yaca"),
]
PURPOSE = {
    "gcc": "BUILD_TOOL_AND_RUNTIME_SUBPACKAGES",
    "gcc-aarch64": "CROSS_BUILD_TOOL", "gcc-armv7hl": "CROSS_BUILD_TOOL",
    "gcc-armv7l": "CROSS_BUILD_TOOL", "gcc-riscv64": "CROSS_BUILD_TOOL",
    "glibc": "DEVICE_RUNTIME",
    "lapack": "NUMERICAL_LIBRARY", "openblas": "NUMERICAL_LIBRARY",
    "nntrainer": "DEVICE_RUNTIME_WITH_APPLICATION_TEST_SUBPACKAGES",
    "onnxruntime": "INFERENCE_RUNTIME_LIBRARY",
    "yaca": "DEVICE_LIBRARY_WITH_EXAMPLE_TEST_SUBPACKAGES",
}


def read(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(name, fields, rows):
    with (TABLES / name).open("w", encoding="utf-8", newline="") as stream:
        out = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        out.writeheader()
        out.writerows(rows)


summary = {(r["project"], r["source_package"]): r for r in read(TABLES / "gcc_source_package_summary.tsv")}
build = read(TABLES / "gcc_inbound_build_dependencies.tsv")
selected_edges = read(TABLES / "gcc_non_system_symbol_edges.tsv")
pair_classes = read(ROOT / "symbol_edges/actual_cross_source_edge_classification.tsv")
pair_class = {(r["consumer_sourcerpm"], r["provider_sourcerpm"]): r for r in pair_classes}

# Recover direction for each source pair and emit representative symbol evidence.
pair_direction = {}
for row in selected_edges:
    pair_direction[(row["consumer_sourcerpm"], row["provider_sourcerpm"])] = row["direction"]
category = {
    "LAYOUT_SENSITIVE_STD_TYPE": "PROVEN_LAYOUT_SENSITIVE_STD_TYPE",
    "CPP_ABI_NO_STD_LAYOUT_PROVEN": "CPP_ABI_COUPLING_LAYOUT_NOT_PROVEN",
    "PURE_C_INTERFACE": "C_ONLY_SYMBOL_INTERSECTION",
    "OTHER_NO_SYMBOL_INTERSECTION": "NO_SYMBOL_INTERSECTION",
}
pair_rows = []
for key in sorted(pair_class):
    row = pair_class[key]
    symbols = [item for item in row["demangled_cpp_symbols"].split(";") if item]
    direction = pair_direction[key]
    gcc_source = key[1] if direction == "GCC_INBOUND" else key[0]
    other_source = key[0] if direction == "GCC_INBOUND" else key[1]
    pair_rows.append({
        "direction": direction, "gcc_source": gcc_source, "other_source": other_source,
        "consumer_source": key[0], "provider_source": key[1],
        "classification": category[row["classification"]],
        "elf_edge_count": row["elf_edge_count"], "cpp_symbol_count": row["cpp_symbol_count"],
        "representative_cpp_symbols": ";".join(symbols[:5]) or "NONE",
        "complete_symbol_evidence": "symbol_edges/actual_cross_elf_edge_evidence.tsv",
    })
write("gcc_runtime_source_pair_classification.tsv", list(pair_rows[0]), pair_rows)

# Exact source-to-image and binary-to-image association.
source_by_srpm = {}
binary_rows = []
for project in ("base", "unified"):
    sources = read(TABLES / f"{project}_source_records.tsv")
    source_by_srpm[project] = {r["location"]: r["name"] for r in sources}
    for row in read(TABLES / f"{project}_binary_records.tsv"):
        row["project"] = project
        row["source_name"] = source_by_srpm[project].get(row["sourcerpm"], "NOT_AVAILABLE")
        binary_rows.append(row)
image_rows = []
for manifest in sorted((ROOT / "inputs/image_packages").glob("*.packages")):
    image = manifest.stem
    with manifest.open(encoding="utf-8") as stream:
        for line in stream:
            fields = line.rstrip("\n").split(" ", 2)
            if len(fields) < 2 or "." not in fields[0]:
                continue
            name, arch = fields[0].rsplit(".", 1)
            vr = fields[1]
            matches = [r for r in binary_rows if r["name"] == name and r["arch"] == arch
                       and f"{r['version']}-{r['release']}" == vr]
            for row in matches:
                if (row["project"], row["source_name"]) in GCC_KEYS:
                    image_rows.append({
                        "image": image, "project": row["project"], "source_package": row["source_name"],
                        "binary_package": row["name"], "arch": arch, "version_release": vr,
                        "rpm_sha256": row["checksum"],
                    })
write("gcc_exact_image_membership.tsv", list(image_rows[0]), image_rows)

# Build and runtime aggregates per GCC source.
build_by = defaultdict(list)
for row in build:
    build_by[(row["provider_project"], row["gcc_provider_source"])].append(row)
pair_by = defaultdict(list)
for row in pair_rows:
    pair_by[row["gcc_source"]].append(row)
image_by = defaultdict(list)
for row in image_rows:
    image_by[(row["project"], row["source_package"])].append(row)

decision = []
for key in GCC_KEYS:
    project, source = key
    item = summary[key]
    build_rows = build_by[key]
    pairs = pair_by[source]
    classes = Counter(r["classification"] for r in pairs)
    inbound = sorted({r["other_source"] for r in pairs if r["direction"] == "GCC_INBOUND"})
    outbound = sorted({r["other_source"] for r in pairs if r["direction"] == "GCC_OUTBOUND"})
    if classes["PROVEN_LAYOUT_SENSITIVE_STD_TYPE"] or classes["CPP_ABI_COUPLING_LAYOUT_NOT_PROVEN"]:
        disposition = "UNABLE_TO_DETERMINE_UNTIL_POST_MIGRATION_RUNTIME_IDENTITY"
    elif pairs:
        disposition = "NO_FOR_OBSERVED_SYMBOL_EDGES"
    else:
        disposition = "NO_NON_SYSTEM_SHARED_LIBRARY_EDGE_OBSERVED"
    exact_images = image_by[key]
    decision.append({
        "project": project, "source_package": source, "gcc_built": "YES",
        "gcc_arches": item["compiler_arches"],
        "produces_cpp_runtime_elf": "YES" if item["cpp_runtime_binary_packages"] else "NO",
        "cpp_runtime_binary_packages": item["cpp_runtime_binary_packages"] or "NONE",
        "directly_links_libstdcxx": "YES" if "libstdc++.so" in item["direct_runtime_signals"] else "NO",
        "device_images": ";".join(sorted({r["image"] for r in exact_images})) or "NONE_IN_SIX_IMAGES",
        "image_binary_packages": ";".join(sorted({r["binary_package"] for r in exact_images})) or "NONE",
        "package_role": PURPOSE[source],
        "build_time_dependent_source_count": str(len({(r["consumer_project"], r["consumer_source"]) for r in build_rows})),
        "build_time_dependent_sources": ";".join(sorted({r["consumer_source"] for r in build_rows})) or "NONE",
        "runtime_inbound_other_sources": ";".join(inbound) or "NONE",
        "runtime_outbound_other_sources": ";".join(outbound) or "NONE",
        "layout_sensitive_pair_count": str(classes["PROVEN_LAYOUT_SENSITIVE_STD_TYPE"]),
        "cpp_coupled_unproven_pair_count": str(classes["CPP_ABI_COUPLING_LAYOUT_NOT_PROVEN"]),
        "c_only_pair_count": str(classes["C_ONLY_SYMBOL_INTERSECTION"]),
        "no_intersection_pair_count": str(classes["NO_SYMBOL_INTERSECTION"]),
        "must_handle_mixed_runtime_edge": disposition,
    })
write("gcc_package_decision_table.tsv", list(decision[0]), decision)

# Explicitly requested tool checks; all are non-GCC controls in this snapshot.
control_names = ["bcc-tools", "bpftrace", "gdb", "llvm", "elfutils", "binutils", "openfst"]
compiler = read(TABLES / "buildlog_compiler_evidence.tsv")
control_rows = []
for name in control_names:
    matches = [r for r in compiler if r["source_package"] == name]
    control_rows.append({
        "source_package": name,
        "projects": ";".join(sorted({r["project"] for r in matches})) or "NOT_AVAILABLE",
        "compiler_statuses": ";".join(sorted({r["compiler_selection"] for r in matches})) or "NOT_AVAILABLE",
        "arches": ";".join(sorted({r["arch"] for r in matches})) or "NOT_AVAILABLE",
        "cc_values": ";".join(sorted({r["cc"] for r in matches})) or "NOT_AVAILABLE",
        "conclusion": "NOT_A_GCC_EXCEPTION_IN_THIS_SNAPSHOT" if matches and
                      {r["compiler_selection"] for r in matches} == {"CLANG"} else "REVIEW_REQUIRED",
    })
write("requested_tool_compiler_checks.tsv", list(control_rows[0]), control_rows)

print(f"decision_packages={len(decision)}")
print("pair_classes=" + ";".join(f"{k}={v}" for k, v in sorted(Counter(r["classification"] for r in pair_rows).items())))
print(f"exact_image_membership_records={len(image_rows)}")
print(f"requested_tool_checks={len(control_rows)}")
