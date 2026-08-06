#!/usr/bin/env python3
import argparse
import collections
import csv
from pathlib import Path


def read_tsv(path):
    with open(path, encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--status", required=True)
    parser.add_argument("--archives", required=True)
    parser.add_argument("--calls", required=True)
    parser.add_argument("--output-summary", required=True)
    parser.add_argument("--output-limitations", required=True)
    args = parser.parse_args()

    status = read_tsv(args.status)
    archives = read_tsv(args.archives)
    calls = read_tsv(args.calls)
    source_denominator = len(status)
    dlopen_elf_denominator = sum(int(row["dlopen_runtime_elf_count"]) for row in status)
    dlopen_binary_denominator = sum(int(row["dlopen_binary_package_count"]) for row in status)

    summary = []
    status_counts = collections.Counter((row["source_download_result"], row["srpm_extract_result"]) for row in status)
    for (download, extract), count in sorted(status_counts.items()):
        summary.append({
            "evidence_type": "SOURCE_ANALYSIS_STATUS", "category": f"{download}/{extract}",
            "numerator": count, "denominator": source_denominator, "unit": "source_package_records",
            "ratio": f"{count/source_denominator:.9f}" if source_denominator else "NOT_AVAILABLE",
        })

    call_counts = collections.Counter(row["scope_window_class"] for row in calls)
    call_sources = collections.defaultdict(set)
    for row in calls:
        call_sources[row["scope_window_class"]].add(row["sourcerpm"])
    status_by_source = {row["sourcerpm"]: row for row in status}
    for category, count in sorted(call_counts.items()):
        summary.append({
            "evidence_type": "SOURCE_LITERAL_PLUS_MINUS_10_LINE_WINDOW", "category": category,
            "numerator": count, "denominator": len(calls), "unit": "dlopen_source_literal_occurrences",
            "ratio": f"{count/len(calls):.9f}" if calls else "NOT_AVAILABLE",
        })
        summary.append({
            "evidence_type": "SOURCE_LITERAL_PLUS_MINUS_10_LINE_WINDOW", "category": category,
            "numerator": len(call_sources[category]), "denominator": source_denominator,
            "unit": "source_package_records", "ratio": f"{len(call_sources[category])/source_denominator:.9f}" if source_denominator else "NOT_AVAILABLE",
        })
        category_binary_packages = sum(int(status_by_source[source]["dlopen_binary_package_count"]) for source in call_sources[category])
        category_elfs = sum(int(status_by_source[source]["dlopen_runtime_elf_count"]) for source in call_sources[category])
        summary.append({
            "evidence_type": "SOURCE_LITERAL_PLUS_MINUS_10_LINE_WINDOW", "category": category,
            "numerator": category_binary_packages, "denominator": dlopen_binary_denominator,
            "unit": "binary_package_records_mapped_from_dlopen_target_sources", "ratio": f"{category_binary_packages/dlopen_binary_denominator:.9f}" if dlopen_binary_denominator else "NOT_AVAILABLE",
        })
        summary.append({
            "evidence_type": "SOURCE_LITERAL_PLUS_MINUS_10_LINE_WINDOW", "category": category,
            "numerator": category_elfs, "denominator": dlopen_elf_denominator,
            "unit": "runtime_ELF_with_dlopen_UND_mapped_from_target_sources", "ratio": f"{category_elfs/dlopen_elf_denominator:.9f}" if dlopen_elf_denominator else "NOT_AVAILABLE",
        })

    any_call_source = set().union(*call_sources.values()) if call_sources else set()
    no_call_sources = set(status_by_source) - any_call_source
    no_call_binary = sum(int(status_by_source[source]["dlopen_binary_package_count"]) for source in no_call_sources)
    no_call_elf = sum(int(status_by_source[source]["dlopen_runtime_elf_count"]) for source in no_call_sources)
    for numerator, denominator, unit in [
        (len(no_call_sources), source_denominator, "source_package_records"),
        (no_call_binary, dlopen_binary_denominator, "binary_package_records_mapped_from_dlopen_target_sources"),
        (no_call_elf, dlopen_elf_denominator, "runtime_ELF_with_dlopen_UND_mapped_from_target_sources"),
    ]:
        summary.append({
            "evidence_type": "SOURCE_LITERAL_PLUS_MINUS_10_LINE_WINDOW",
            "category": "NO_DLOPEN_LITERAL_IN_EXTRACTED_TEXT",
            "numerator": numerator, "denominator": denominator, "unit": unit,
            "ratio": f"{numerator/denominator:.9f}" if denominator else "NOT_AVAILABLE",
        })

    archive_counts = collections.Counter(row["result"] for row in archives)
    for category, count in sorted(archive_counts.items()):
        summary.append({
            "evidence_type": "SOURCE_ARCHIVE_EXPANSION_STATUS", "category": category,
            "numerator": count, "denominator": len(archives), "unit": "source_archive_files",
            "ratio": f"{count/len(archives):.9f}" if archives else "NOT_AVAILABLE",
        })
    summary.extend([
        {"evidence_type": "DL_OPEN_ELF_INPUT", "category": "RUNTIME_ELF_WITH_DLOPEN_UND", "numerator": dlopen_elf_denominator, "denominator": 36829, "unit": "runtime_ELF_files", "ratio": f"{dlopen_elf_denominator/36829:.9f}"},
        {"evidence_type": "DL_OPEN_BINARY_PACKAGE_INPUT", "category": "BINARY_PACKAGE_WITH_DLOPEN_UND", "numerator": dlopen_binary_denominator, "denominator": 11332, "unit": "binary_package_records", "ratio": f"{dlopen_binary_denominator/11332:.9f}"},
    ])
    write_tsv(args.output_summary, ["evidence_type", "category", "numerator", "denominator", "unit", "ratio"], summary)

    limitations = [
        {
            "requested_measurement": "RUNTIME_RTLD_LOCAL_VS_GLOBAL_DISTRIBUTION",
            "status": "NOT_AVAILABLE_FROM_ELF",
            "measured_input_denominator": f"{dlopen_elf_denominator}_runtime_ELF_files",
            "reason": "dlopen mode is a runtime value and is not encoded as a classifiable ELF property",
            "available_approximation": "source literal RTLD_LOCAL/RTLD_GLOBAL within plus/minus 10 source lines of a dlopen literal",
        },
        {
            "requested_measurement": "SOURCE_WINDOW_APPROXIMATION",
            "status": "APPROXIMATION_ONLY",
            "measured_input_denominator": f"{source_denominator}_source_package_records",
            "reason": "literal proximity does not establish the runtime expression, executed branch, macro expansion, or loaded-object scope",
            "available_approximation": "dlopen_source_call_scope_approximation.tsv",
        },
    ]
    write_tsv(args.output_limitations, ["requested_measurement", "status", "measured_input_denominator", "reason", "available_approximation"], limitations)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
