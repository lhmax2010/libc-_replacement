#!/usr/bin/env python3
"""Build the mechanical R100 per-source-package decision table."""

import csv
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read(path):
    with Path(path).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, rows):
    fields = list(rows[0])
    with Path(path).open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def normalized(name):
    name = name.replace("%2B%2B", "++")
    if name in {"qemu-accel-aarch64", "qemu-accel-armv7l", "qemu-accel-riscv64"}:
        return "qemu-accel"
    return name


base_sources = read("progress/R98/tables/base_source_records.tsv")
base_binaries = read("progress/R98/tables/base_binary_records.tsv")
unified_sources = read("progress/R98/tables/unified_source_records.tsv")
source_by_srpm = {row["location"]: row["name"] for row in base_sources}
source_info = defaultdict(list)
for row in base_sources:
    source_info[row["name"]].append(row)
binary_by_source = defaultdict(list)
for row in base_binaries:
    binary_by_source[source_by_srpm.get(row["sourcerpm"], "NOT_AVAILABLE")].append(row)

# Exact positive evidence sets.
build_rows = read("progress/R100/tables/base_cpp_buildlog_scan.tsv")
build = defaultdict(list)
for row in build_rows:
    if row["cpp_compile_detected"] == "YES":
        build[normalized(row["source_package"])].append(row)
dynamic = {
    row["source_name"] for row in read("progress/R98/tables/base_cpp_source_records.tsv")
}
static_table = read("progress/R100/tables/static_archive_symbols_refined.tsv")
static_cpp = defaultdict(list)
for row in static_table:
    if row["cpp_mangled_symbol"] == "YES":
        static_cpp[source_by_srpm.get(row["source_name"], row["source_name"])].append(row)
carrier_rows = read("progress/R100/tables/base_filelist_cpp_carriers.tsv")
headers = defaultdict(list)
for row in carrier_rows:
    if int(row["cpp_header_count"]):
        headers[row["source_name"]].append(row)

candidates = set(build) | dynamic | set(static_cpp) | set(headers)

# Runtime cross-boundary evidence, exact armv7l snapshot contents.
cross_rows = read("progress/R100/armv7l_cross_edges/actual_cross_source_edge_classification.tsv")
cross_classes = defaultdict(Counter)
cross_consumers = defaultdict(set)
for row in cross_rows:
    provider = source_by_srpm.get(row["provider_sourcerpm"], row["provider_sourcerpm"])
    cross_classes[provider][row["classification"]] += 1
    cross_consumers[provider].add(row["consumer_sourcerpm"])

cpp_classes = {"LAYOUT_SENSITIVE_STD_TYPE", "CPP_ABI_NO_STD_LAYOUT_PROVEN"}
need = {name for name, classes in cross_classes.items() if any(classes[key] for key in cpp_classes)}

# Base-internal C++ edges propagate the same-runtime requirement.
internal_rows = read("progress/R100/armv7l_internal_edges/actual_cross_source_edge_classification.tsv")
internal_cpp = []
internal_classes = defaultdict(Counter)
for row in internal_rows:
    consumer = source_by_srpm.get(row["consumer_sourcerpm"], row["consumer_sourcerpm"])
    provider = source_by_srpm.get(row["provider_sourcerpm"], row["provider_sourcerpm"])
    internal_classes[consumer][row["classification"]] += 1
    if row["classification"] in cpp_classes:
        internal_cpp.append((consumer, provider, row["classification"]))
changed = True
while changed:
    changed = False
    for consumer, provider, _ in internal_cpp:
        if provider in need and consumer not in need:
            need.add(consumer)
            changed = True

# Current compiler constraints are exact R98 spec/build-log findings.
constraints = {
    row["source_package"]
    for row in read("progress/R98/tables/gcc_source_package_summary.tsv")
    if row["project"] == "base" and "GCC" in row["compiler_statuses"].split(";")
}

# Exact device-image membership, mapped by VCS identity.
names_by_vcs = defaultdict(set)
for row in base_sources:
    names_by_vcs[row["vcs"]].add(row["name"])
images = defaultdict(set)
for manifest in sorted(Path("progress/R98/inputs/image_packages").glob("*.packages")):
    with manifest.open(encoding="utf-8") as stream:
        for line in stream:
            fields = line.rstrip("\n").split(" ", 2)
            if len(fields) != 3:
                continue
            for name in names_by_vcs.get(fields[2], ()):
                images[name].add(manifest.stem)

# Exact Unified source BuildRequires -> Base provider source topology.  It is
# evidence of availability to the build, not proof that a static archive is linked.
provider_by_capability = defaultdict(set)
for row in base_binaries:
    name = source_by_srpm.get(row["sourcerpm"], "NOT_AVAILABLE")
    for capability in filter(None, row["provides"].split(";")):
        provider_by_capability[capability].add(name)
unified_build_consumers = defaultdict(set)
for consumer in unified_sources:
    for requirement in filter(None, consumer["requires"].split(";")):
        for provider in provider_by_capability.get(requirement, ()):
            unified_build_consumers[provider].add(consumer["name"])

# Binary role facts.
shared_provider = {}
for name in candidates:
    shared_provider[name] = any(
        any(re.match(r"^lib.*\.so(?:\.|$)", cap) for cap in row["provides"].split(";"))
        for row in binary_by_source.get(name, [])
    )

rows = []
for name in sorted(candidates):
    evidence = []
    if build[name]: evidence.append("ACTUAL_CPP_BUILD_LOG")
    if name in dynamic: evidence.append("CPP_RUNTIME_DEPENDENCY")
    if static_cpp[name]: evidence.append("GLOBAL_DEMANGLED_CPP_IN_STATIC_ARCHIVE")
    if headers[name]: evidence.append("CPP_HEADER_PAYLOAD")

    reasons = []
    if name in need:
        reasons.append("N1_EXACT_CPP_RUNTIME_EDGE")
    if name in constraints:
        reasons.append("C1_CURRENT_GCC_OVERRIDE_OR_GCC_ROLE")

    if name in need and name in constraints:
        decision = "CONFLICT_NEED_BUT_CANNOT"
    elif name in need:
        decision = "NEED_LIBCXX"
    elif name in constraints:
        decision = "CANNOT_SWITCH_CURRENTLY"
    elif name == "libcxx-runtimes":
        decision = "NO_LIBCXX_NEEDED"
        reasons.append("NO_ALREADY_EXPLICIT_LIBCXX_RUNTIME_BUILD")
    elif cross_classes[name] and not any(cross_classes[name][key] for key in cpp_classes):
        decision = "NO_LIBCXX_NEEDED"
        reasons.append("NO_EXACT_EDGES_ONLY_C_OR_NO_INTERSECTION")
    elif headers[name] and name not in dynamic and not static_cpp[name] and not shared_provider[name]:
        decision = "NO_LIBCXX_NEEDED"
        reasons.append("NO_HEADER_PAYLOAD_COMPILED_BY_CONSUMER")
    elif static_cpp[name] and unified_build_consumers[name]:
        decision = "UNDETERMINED"
        reasons.append("U_STATIC_CPP_ARCHIVE_WITH_BUILDREQUIRE_CONSUMERS_LINK_NOT_PROVEN")
    elif not cross_consumers[name]:
        decision = "NO_LIBCXX_NEEDED"
        if static_cpp[name]:
            reasons.append("NO_CPP_STATIC_ARCHIVE_WITHOUT_UNIFIED_CONSUMER")
        elif name in dynamic:
            reasons.append("NO_BASE_ONLY_OR_INDEPENDENT_PRODUCT_NO_UNIFIED_RUNTIME_EDGE")
        else:
            reasons.append("NO_BUILD_EXPOSURE_WITHOUT_SHIPPED_CPP_BOUNDARY")
    else:
        decision = "UNDETERMINED"
        reasons.append("U_NO_POSITIVE_ISOLATION_OR_CPP_EDGE_PROOF")

    rows.append({
        "source_package": name,
        "decision": decision,
        "matched_rules": ";".join(reasons),
        "denominator_evidence": ";".join(evidence),
        "buildlog_arches": ";".join(sorted({row["arch"] for row in build[name]})) or "NONE",
        "dynamic_cpp_runtime_signal": "YES" if name in dynamic else "NO",
        "cpp_static_archive_count": str(len(static_cpp[name])),
        "cpp_header_rpm_count": str(len(headers[name])),
        "provides_shared_library": "YES" if shared_provider[name] else "NO",
        "unified_runtime_consumer_count": str(len(cross_consumers[name])),
        "unified_runtime_edge_classes": ";".join(f"{k}={v}" for k, v in sorted(cross_classes[name].items())) or "NONE",
        "base_internal_edge_classes_as_consumer": ";".join(f"{k}={v}" for k, v in sorted(internal_classes[name].items())) or "NONE",
        "unified_buildrequire_consumer_count": str(len(unified_build_consumers[name])),
        "unified_buildrequire_consumers": ";".join(sorted(unified_build_consumers[name])) or "NONE",
        "six_image_membership": ";".join(sorted(images[name])) or "NONE",
        "current_gcc_constraint": "YES" if name in constraints else "NO",
        "source_vcs": ";".join(sorted({row["vcs"] for row in source_info[name]})) or "NOT_AVAILABLE",
        "evidence_scope_note": "ARMV7L_SYMBOL_EDGES;ALL_ARCH_BUILDS_AND_ARTIFACT_METADATA",
    })

write("progress/R100/tables/base_cpp_package_decisions.tsv", rows)
counts = Counter(row["decision"] for row in rows)
print(f"denominator={len(rows)}")
for key in sorted(counts):
    names = [row["source_package"] for row in rows if row["decision"] == key]
    print(f"{key}={counts[key]}")
    print(f"{key}_NAMES=" + ";".join(names))
