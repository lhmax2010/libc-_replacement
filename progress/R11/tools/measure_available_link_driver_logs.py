#!/usr/bin/env python3
import argparse
import csv
import hashlib
import pathlib
import re


def write_tsv(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--link-log", required=True)
    parser.add_argument("--raw-candidate-log", required=True)
    parser.add_argument("--output-measurement", required=True)
    parser.add_argument("--output-scope", required=True)
    args = parser.parse_args()
    link_log = pathlib.Path(args.link_log)
    raw_log = pathlib.Path(args.raw_candidate_log)
    lines = link_log.read_text(encoding="utf-8", errors="replace").splitlines()
    clangxx = sum(bool(re.search(r"(?:^|/)\S*clang\+\+(?:\s|$)", line)) for line in lines)
    clang = sum(bool(re.search(r"(?:^|/)\S*clang(?:\s|$)", line)) for line in lines)
    total = len(lines)
    measurement = [
        {"evidence_scope": "R2_LIBCYNARA_COMMONS_ACTUAL_LINK_COMMANDS", "driver": "clang++", "link_invocation_numerator": clangxx, "link_invocation_denominator_available_log": total, "source_package_numerator": 1, "source_package_denominator_frozen_snapshot": 1450, "platform_wide_distribution_available": "NO"},
        {"evidence_scope": "R2_LIBCYNARA_COMMONS_ACTUAL_LINK_COMMANDS", "driver": "clang", "link_invocation_numerator": clang, "link_invocation_denominator_available_log": total, "source_package_numerator": 1, "source_package_denominator_frozen_snapshot": 1450, "platform_wide_distribution_available": "NO"},
    ]
    write_tsv(args.output_measurement, ["evidence_scope", "driver", "link_invocation_numerator", "link_invocation_denominator_available_log", "source_package_numerator", "source_package_denominator_frozen_snapshot", "platform_wide_distribution_available"], measurement)

    scope = []
    for kind, path, disposition in [
        ("RAW_CANDIDATE_BUILD_LOG", raw_log, "AVAILABLE_ONE_SOURCE_PACKAGE_ONLY"),
        ("CURATED_ACTUAL_LINK_COMMANDS", link_log, "AVAILABLE_FIVE_LINK_INVOCATIONS"),
    ]:
        data = path.read_bytes()
        scope.append({"scope": kind, "path": str(path.resolve()), "file_count": 1, "size_bytes": len(data), "sha256": hashlib.sha256(data).hexdigest(), "disposition": disposition})
    scope.extend([
        {"scope": "FROZEN_REPODATA", "path": "progress/R11/metadata", "file_count": 4, "size_bytes": "NOT_APPLICABLE", "sha256": "SEE_REPODATA_INVENTORY", "disposition": "NO_BUILD_LOG_FIELDS"},
        {"scope": "FROZEN_SOURCE_SPECS", "path": "progress/R11/tables/source_spec_inventory.tsv", "file_count": 1449, "size_bytes": "SEE_INVENTORY", "sha256": "SEE_SOURCE_SPEC_TABLE_HASH", "disposition": "LITERAL_TEXT_ONLY_CANNOT_CLASSIFY_ACTUAL_LINK_INVOCATIONS"},
        {"scope": "PROJECT_LIBCXX_BUILD_RECORDS", "path": "progress/T3R;progress/T3R2;progress/R6;progress/R9B;progress/R9C", "file_count": "NOT_USED_AS_PLATFORM_SAMPLE", "size_bytes": "NOT_APPLICABLE", "sha256": "NOT_APPLICABLE", "disposition": "SINGLE_PROJECT_PACKAGE_NOT_PLATFORM_WIDE_DISTRIBUTION"},
    ])
    write_tsv(args.output_scope, ["scope", "path", "file_count", "size_bytes", "sha256", "disposition"], scope)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
