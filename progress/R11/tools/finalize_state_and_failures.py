#!/usr/bin/env python3
import argparse
import collections
import csv


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
    parser.add_argument("--table-root", required=True)
    args = parser.parse_args()
    root = args.table_root.rstrip("/")
    binary_download = read_tsv(f"{root}/binary_download_status.tsv")
    rpm_extract = read_tsv(f"{root}/rpm_extraction_status.tsv")
    elf_fail = read_tsv(f"{root}/elf_analysis_failures.tsv")
    source_download = read_tsv(f"{root}/source_download_status.tsv")
    spec = read_tsv(f"{root}/source_spec_extraction_status.tsv")
    dlopen_status = read_tsv(f"{root}/dlopen_source_analysis_status.tsv")
    archives = read_tsv(f"{root}/dlopen_source_archive_status.tsv")

    def failed(rows, field, good):
        return [row for row in rows if row[field] not in good]

    failures = []
    stages = [
        ("binary_RPM_download_and_checksum", binary_download, "result", {"PASS"}, "binary_package_records"),
        ("binary_RPM_extraction", rpm_extract, "result", {"PASS"}, "binary_package_records"),
        ("ELF_readelf_analysis", elf_fail, None, None, "ELF_files"),
        ("source_RPM_download_and_checksum", source_download, "result", {"PASS"}, "source_package_records"),
        ("source_spec_extraction", spec, "result", {"PASS"}, "source_package_records"),
        ("dlopen_target_source_RPM_extraction", dlopen_status, "srpm_extract_result", {"PASS"}, "dlopen_target_source_package_records"),
    ]
    for stage, rows, field, good, unit in stages:
        bad = rows if field is None else failed(rows, field, good)
        affected = []
        for row in bad:
            name = row.get("name") or row.get("sourcerpm") or row.get("path") or "UNKNOWN"
            reason = row.get("stderr") or row.get("srpm_extract_result") or row.get("result") or "SEE_DETAIL_TABLE"
            affected.append(f"{name}:{reason}")
        denominator = 39166 if stage == "ELF_readelf_analysis" else len(rows)
        failures.append({
            "stage": stage, "failure_numerator": len(bad), "denominator": denominator, "unit": unit,
            "status": "PASS_ZERO_FAILURE" if not bad else "PARTIAL_WITH_REGISTERED_FAILURE",
            "affected_and_reason": ";".join(affected) if affected else "NONE",
        })
    archive_bad = [row for row in archives if not row["result"].startswith("PASS")]
    failures.append({
        "stage": "dlopen_source_archive_expansion", "failure_numerator": len(archive_bad),
        "denominator": len(archives), "unit": "recognized_source_archive_files",
        "status": "PASS_ZERO_FAILURE" if not archive_bad else "PARTIAL_WITH_REGISTERED_FAILURE",
        "affected_and_reason": ";".join(f"{row['sourcerpm']}:{row['archive']}:{row['result']}" for row in archive_bad) if archive_bad else "NONE",
    })
    write_tsv(f"{root}/failure_registry.tsv", ["stage", "failure_numerator", "denominator", "unit", "status", "affected_and_reason"], failures)

    state = [
        {"item": "task", "value": "R11"},
        {"item": "status", "value": "DONE_WITH_NOT_AVAILABLE"},
        {"item": "scope", "value": "READ_ONLY_FULL_FROZEN_BASE_UNIFIED_BINARY_AND_SOURCE_REPODATA"},
        {"item": "binary_sample", "value": "NO_SAMPLING_11332_OF_11332"},
        {"item": "runtime_elf_denominator", "value": "36829"},
        {"item": "source_package_denominator", "value": "1450"},
        {"item": "actual_platform_link_driver_distribution", "value": "NOT_AVAILABLE"},
        {"item": "same_ELF_C_CPP_distribution", "value": "UNDETERMINED_36786_OF_36829"},
        {"item": "runtime_RTLD_scope_distribution", "value": "NOT_AVAILABLE_FROM_ELF_SOURCE_APPROXIMATION_RECORDED"},
        {"item": "recommendation", "value": "NONE_BY_TASK_CONTRACT"},
        {"item": "external_platform_push", "value": "NOT_PERFORMED"},
        {"item": "build_or_install", "value": "NOT_PERFORMED"},
    ]
    write_tsv(f"{root}/state.tsv", ["item", "value"], state)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
