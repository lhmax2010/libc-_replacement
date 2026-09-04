#!/usr/bin/env python3
"""Compare fixed Tizen-Base and Tizen-Base-Toolchain RPM metadata."""

import argparse
import csv
import gzip
import re
import xml.etree.ElementTree as ET
from collections import defaultdict
from pathlib import Path


COMMON = "http://linux.duke.edu/metadata/common"
RPM = "http://linux.duke.edu/metadata/rpm"
NS = {"c": COMMON, "r": RPM}


def text(node, path, default=""):
    found = node.find(path, NS)
    return found.text if found is not None and found.text is not None else default


def parse_primary(path, repo):
    opener = gzip.open if str(path).endswith(".gz") else open
    records = []
    with opener(path, "rb") as stream:
        root = ET.parse(stream).getroot()
    for pkg in root.findall(f"{{{COMMON}}}package"):
        fmt = pkg.find(f"{{{COMMON}}}format")
        ver = pkg.find(f"{{{COMMON}}}version")
        loc = pkg.find(f"{{{COMMON}}}location")
        requires = []
        provides = []
        if fmt is not None:
            for entry in fmt.findall(f"{{{RPM}}}requires/{{{RPM}}}entry"):
                requires.append(entry.attrib.get("name", ""))
            for entry in fmt.findall(f"{{{RPM}}}provides/{{{RPM}}}entry"):
                provides.append(entry.attrib.get("name", ""))
        records.append({
            "repo": repo,
            "name": text(pkg, "c:name"),
            "arch": text(pkg, "c:arch"),
            "epoch": ver.attrib.get("epoch", "") if ver is not None else "",
            "version": ver.attrib.get("ver", "") if ver is not None else "",
            "release": ver.attrib.get("rel", "") if ver is not None else "",
            "sourcerpm": text(fmt, "r:sourcerpm") if fmt is not None else "",
            "requires": ";".join(sorted(set(requires))),
            "provides": ";".join(sorted(set(provides))),
            "location": loc.attrib.get("href", "") if loc is not None else "",
            "summary": text(pkg, "c:summary"),
        })
    return records


def read_tsv(path):
    with open(path, encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path, rows, fields):
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        writer.writeheader()
        writer.writerows(rows)


def source_name_from_sourcerpm(sourcerpm, source_names):
    value = sourcerpm.removesuffix(".src.rpm")
    matches = [name for name in source_names if value == name or value.startswith(name + "-")]
    return max(matches, key=len) if matches else "NOT_AVAILABLE"


def versions_by_name(records):
    result = defaultdict(set)
    for row in records:
        result[row["name"]].add(f'{row["epoch"]}:{row["version"]}-{row["release"]}')
    return result


def upstream_versions_by_name(records):
    result = defaultdict(set)
    for row in records:
        result[row["name"]].add(f'{row["epoch"]}:{row["version"]}')
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-binary", required=True)
    parser.add_argument("--base-source", required=True)
    parser.add_argument("--toolchain-binary", required=True)
    parser.add_argument("--toolchain-source", required=True)
    parser.add_argument("--r95-cpp-sources", required=True)
    parser.add_argument("--r101-all", required=True)
    parser.add_argument("--paired-unified-source", required=True)
    parser.add_argument("--output", required=True)
    args = parser.parse_args()
    out = Path(args.output)

    # R95 parsed rows are reused verbatim; fixed snapshot identity is recorded by caller/report.
    base_bin = read_tsv(args.base_binary)
    base_src = read_tsv(args.base_source)
    for row in base_bin + base_src:
        row["repo"] = "Tizen-Base"
    tc_bin = parse_primary(args.toolchain_binary, "Tizen-Base-Toolchain")
    tc_src = parse_primary(args.toolchain_source, "Tizen-Base-Toolchain")

    base_source_names = {r["name"] for r in base_src}
    tc_source_names = {r["name"] for r in tc_src}
    all_source_names = base_source_names | tc_source_names
    for row in base_bin:
        row["source_name"] = source_name_from_sourcerpm(row.get("sourcerpm", ""), all_source_names)
    for row in tc_bin:
        row["source_name"] = source_name_from_sourcerpm(row.get("sourcerpm", ""), all_source_names)

    def libstdcpp(row):
        return "libstdc++.so.6" in row.get("requires", "")

    def unique_names(records):
        return {r["name"] for r in records}

    r95_cpp_rows = read_tsv(args.r95_cpp_sources)
    r95_cpp = {r.get("source_name", r.get("name", "")) for r in r95_cpp_rows}
    r101 = {r["源码包名"]: r for r in read_tsv(args.r101_all)}
    paired_unified = read_tsv(args.paired_unified_source)
    paired_unified_names = {r["name"] for r in paired_unified}
    paired_unified_versions = versions_by_name(paired_unified)

    summary = []
    for label, binary, source, cpp_set, definition in [
        ("Tizen-Base", base_bin, base_src, r95_cpp,
         "R95_RUNTIME_CPP_ELF: source produces at least one runtime ELF with C++ evidence"),
        ("Tizen-Base-Toolchain", tc_bin, tc_src,
         {name for name, row in r101.items() if row["是否含C++及依据"].startswith("是：")},
         "R100/R101 four-way union: build log/runtime dep/static archive/C++ headers"),
    ]:
        summary.append({
            "repository": label,
            "binary_records": len(binary),
            "unique_binary_names": len(unique_names(binary)),
            "source_records": len(source),
            "unique_source_names": len(unique_names(source)),
            "libstdcpp_binary_records": sum(libstdcpp(r) for r in binary),
            "unique_libstdcpp_binary_names": len({r["name"] for r in binary if libstdcpp(r)}),
            "unique_sources_behind_libstdcpp_binaries": len({r.get("source_name", "NOT_AVAILABLE") for r in binary if libstdcpp(r) and r.get("source_name", "NOT_AVAILABLE") != "NOT_AVAILABLE"}),
            "cpp_source_names": len(cpp_set),
            "cpp_source_definition": definition,
        })
    write_tsv(out / "repository_summary.tsv", summary, list(summary[0]))

    per_arch = []
    for label, binary in [("Tizen-Base", base_bin), ("Tizen-Base-Toolchain", tc_bin)]:
        for arch in sorted({row["arch"] for row in binary}):
            rows = [row for row in binary if row["arch"] == arch]
            per_arch.append({
                "repository": label,
                "arch": arch,
                "binary_records": len(rows),
                "unique_binary_names": len({row["name"] for row in rows}),
                "libstdcpp_binary_records": sum(libstdcpp(row) for row in rows),
                "unique_libstdcpp_binary_names": len({row["name"] for row in rows if libstdcpp(row)}),
                "unique_sources_behind_libstdcpp_binaries": len({row.get("source_name", "NOT_AVAILABLE") for row in rows if libstdcpp(row) and row.get("source_name", "NOT_AVAILABLE") != "NOT_AVAILABLE"}),
            })
    write_tsv(out / "repository_per_arch_summary.tsv", per_arch, list(per_arch[0]))

    base_bin_names = unique_names(base_bin)
    tc_bin_names = unique_names(tc_bin)
    base_src_versions = versions_by_name(base_src)
    tc_src_versions = versions_by_name(tc_src)
    base_bin_versions = versions_by_name(base_bin)
    tc_bin_versions = versions_by_name(tc_bin)
    base_src_upstream = upstream_versions_by_name(base_src)
    tc_src_upstream = upstream_versions_by_name(tc_src)
    base_bin_upstream = upstream_versions_by_name(base_bin)
    tc_bin_upstream = upstream_versions_by_name(tc_bin)

    def set_rows(values, label):
        return [{label: value} for value in sorted(values)]

    write_tsv(out / "source_common.tsv", set_rows(base_source_names & tc_source_names, "source_package"), ["source_package"])
    write_tsv(out / "source_only_tizen_base.tsv", set_rows(base_source_names - tc_source_names, "source_package"), ["source_package"])
    write_tsv(out / "source_only_base_toolchain.tsv", set_rows(tc_source_names - base_source_names, "source_package"), ["source_package"])
    write_tsv(out / "binary_common.tsv", set_rows(base_bin_names & tc_bin_names, "binary_package"), ["binary_package"])
    write_tsv(out / "binary_only_tizen_base.tsv", set_rows(base_bin_names - tc_bin_names, "binary_package"), ["binary_package"])
    write_tsv(out / "binary_only_base_toolchain.tsv", set_rows(tc_bin_names - base_bin_names, "binary_package"), ["binary_package"])

    source_version_diff = []
    for name in sorted(base_source_names & tc_source_names):
        if base_src_versions[name] != tc_src_versions[name]:
            source_version_diff.append({"source_package": name,
                                        "tizen_base_evrs": ";".join(sorted(base_src_versions[name])),
                                        "base_toolchain_evrs": ";".join(sorted(tc_src_versions[name])),
                                        "difference_kind": "UPSTREAM_VERSION_DIFFERS" if base_src_upstream[name] != tc_src_upstream[name] else "RELEASE_ONLY_DIFFERS"})
    write_tsv(out / "source_version_differences.tsv", source_version_diff,
              ["source_package", "tizen_base_evrs", "base_toolchain_evrs", "difference_kind"])

    binary_version_diff = []
    for name in sorted(base_bin_names & tc_bin_names):
        if base_bin_versions[name] != tc_bin_versions[name]:
            binary_version_diff.append({"binary_package": name,
                                        "tizen_base_evrs": ";".join(sorted(base_bin_versions[name])),
                                        "base_toolchain_evrs": ";".join(sorted(tc_bin_versions[name])),
                                        "difference_kind": "UPSTREAM_VERSION_DIFFERS" if base_bin_upstream[name] != tc_bin_upstream[name] else "RELEASE_ONLY_DIFFERS"})
    write_tsv(out / "binary_version_differences.tsv", binary_version_diff,
              ["binary_package", "tizen_base_evrs", "base_toolchain_evrs", "difference_kind"])

    def children(records):
        result = defaultdict(set)
        for row in records:
            if row.get("source_name") != "NOT_AVAILABLE":
                result[row["source_name"]].add(row["name"])
        return result

    base_children = children(base_bin)
    tc_children = children(tc_bin)
    child_diff = []
    for source in sorted(base_source_names & tc_source_names):
        only_b = base_children[source] - tc_children[source]
        only_t = tc_children[source] - base_children[source]
        if only_b or only_t:
            child_diff.append({"source_package": source,
                               "only_tizen_base_subpackages": ";".join(sorted(only_b)) or "NONE",
                               "only_base_toolchain_subpackages": ";".join(sorted(only_t)) or "NONE",
                               "common_subpackage_count": len(base_children[source] & tc_children[source])})
    write_tsv(out / "shared_source_subpackage_differences.tsv", child_diff,
              ["source_package", "only_tizen_base_subpackages", "only_base_toolchain_subpackages", "common_subpackage_count"])

    base_only_analysis = []
    for source in sorted(base_source_names - tc_source_names):
        binaries = sorted(base_children.get(source, set()))
        libstd = sorted({r["name"] for r in base_bin if r.get("source_name") == source and libstdcpp(r)})
        cpp = source in r95_cpp or bool(libstd)
        # The package is outside R101's Toolchain denominator. Without its Toolchain build/exposure
        # evidence, R101's need/no-need decision tree cannot be completed.
        decision = "UNDETERMINED" if cpp else "NO_LIBCXX_NEEDED"
        reason = ("Tizen-Base runtime artifacts show C++/libstdc++ evidence, but the source is absent from "
                  "the Toolchain snapshot and has no R101 cross-boundary/image evidence; the R101 decision tree cannot be completed."
                  if cpp else
                  "R95 runtime-ELF scan and repository requirements show no C++/libstdc++ runtime carrier in Tizen-Base; no migration carrier is observed in this repository snapshot.")
        base_only_analysis.append({
            "source_package": source,
            "tizen_base_binary_packages": ";".join(binaries) or "NONE",
            "r95_cpp_runtime_source": "YES" if source in r95_cpp else "NO",
            "libstdcpp_binary_packages": ";".join(libstd) or "NONE",
            "contains_cpp_fact": "YES" if cpp else "NO_RUNTIME_CPP_EVIDENCE",
            "toolchain_partition_observation": ("PRESENT_IN_PAIRED_UNIFIED_TOOLCHAIN" if source in paired_unified_names else "NOT_OBSERVED_IN_PAIRED_UNIFIED_TOOLCHAIN"),
            "paired_unified_toolchain_evrs": (";".join(sorted(paired_unified_versions[source])) if source in paired_unified_names else "NONE"),
            "absence_reason_from_base_toolchain": ("PARTITIONED_TO_PAIRED_UNIFIED_TOOLCHAIN; policy rationale NOT_AVAILABLE" if source in paired_unified_names else "NOT_AVAILABLE"),
            "r101_rule_result": decision,
            "rule_evidence_and_limit": reason,
        })
    write_tsv(out / "tizen_base_only_source_analysis.tsv", base_only_analysis, list(base_only_analysis[0]) if base_only_analysis else ["source_package"])

    metrics = {
        "source_common": len(base_source_names & tc_source_names),
        "source_only_tizen_base": len(base_source_names - tc_source_names),
        "source_only_base_toolchain": len(tc_source_names - base_source_names),
        "source_version_differences": len(source_version_diff),
        "source_upstream_version_differences": sum(r["difference_kind"] == "UPSTREAM_VERSION_DIFFERS" for r in source_version_diff),
        "source_release_only_differences": sum(r["difference_kind"] == "RELEASE_ONLY_DIFFERS" for r in source_version_diff),
        "binary_common": len(base_bin_names & tc_bin_names),
        "binary_only_tizen_base": len(base_bin_names - tc_bin_names),
        "binary_only_base_toolchain": len(tc_bin_names - base_bin_names),
        "binary_version_differences": len(binary_version_diff),
        "binary_upstream_version_differences": sum(r["difference_kind"] == "UPSTREAM_VERSION_DIFFERS" for r in binary_version_diff),
        "binary_release_only_differences": sum(r["difference_kind"] == "RELEASE_ONLY_DIFFERS" for r in binary_version_diff),
        "shared_source_subpackage_differences": len(child_diff),
        "base_only_cpp_or_libstdcpp": sum(r["contains_cpp_fact"] == "YES" for r in base_only_analysis),
        "base_only_r101_undetermined": sum(r["r101_rule_result"] == "UNDETERMINED" for r in base_only_analysis),
    }
    write_tsv(out / "comparison_metrics.tsv", [{"metric": k, "value": v} for k, v in metrics.items()], ["metric", "value"])

    normalized_fields = ["repo", "name", "arch", "epoch", "version", "release", "source_name", "sourcerpm", "requires", "provides", "location", "summary"]
    write_tsv(out / "normalized_binary_records.tsv", [dict(row, source_name=row.get("source_name", "")) for row in base_bin + tc_bin], normalized_fields)
    print("comparison_complete")
    for key, value in metrics.items():
        print(f"{key}={value}")


if __name__ == "__main__":
    main()
