#!/usr/bin/env python3
import argparse
import collections
import csv


def read(path):
    with open(path, encoding="utf-8") as stream: return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader(); writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--status", required=True)
    ap.add_argument("--classifications", required=True)
    ap.add_argument("--hits", required=True)
    ap.add_argument("--output-summary", required=True)
    ap.add_argument("--output-availability", required=True)
    args = ap.parse_args()
    status, classifications, hits = map(read, [args.status, args.classifications, args.hits])
    denominator = len(status)
    categories = collections.Counter(row["category"] for row in classifications)
    rows = []
    for category, count in sorted(categories.items()):
        rows.append({
            "evidence_type": "SPEC_LITERAL_TEXT_CLASSIFICATION",
            "category": category, "source_package_numerator": count,
            "source_package_denominator": denominator,
            "ratio": f"{count / denominator:.9f}" if denominator else "NOT_AVAILABLE",
            "can_establish_actual_link_driver": "NO",
        })
    token_sources = collections.defaultdict(set)
    token_lines = collections.Counter()
    for row in hits:
        token_sources[row["token"]].add(row["checksum"]); token_lines[row["token"]] += 1
    for token in sorted(token_sources):
        rows.append({
            "evidence_type": "SPEC_LITERAL_TOKEN_HIT",
            "category": token, "source_package_numerator": len(token_sources[token]),
            "source_package_denominator": denominator,
            "ratio": f"{len(token_sources[token]) / denominator:.9f}" if denominator else "NOT_AVAILABLE",
            "can_establish_actual_link_driver": "NO",
        })
    write(args.output_summary, [
        "evidence_type", "category", "source_package_numerator", "source_package_denominator",
        "ratio", "can_establish_actual_link_driver",
    ], rows)
    availability = [
        {
            "requested_measurement": "ACTUAL_CLANG_LINK_DRIVER_DISTRIBUTION",
            "status": "NOT_AVAILABLE",
            "denominator": f"{denominator}_source_package_records",
            "reason": "frozen repodata and SRPM specs do not contain complete expanded link command logs; literal compiler tokens cannot distinguish compile from link invocations",
            "available_approximation": "source_spec_link_driver_text_classification.tsv and source_spec_text_hits.tsv",
        },
        {
            "requested_measurement": "ACTUAL_CLANGXX_LINK_DRIVER_DISTRIBUTION",
            "status": "NOT_AVAILABLE",
            "denominator": f"{denominator}_source_package_records",
            "reason": "frozen repodata and SRPM specs do not contain complete expanded link command logs; literal compiler tokens cannot distinguish compile from link invocations",
            "available_approximation": "source_spec_link_driver_text_classification.tsv and source_spec_text_hits.tsv",
        },
    ]
    write(args.output_availability, [
        "requested_measurement", "status", "denominator", "reason", "available_approximation",
    ], availability)
    return 0


if __name__ == "__main__": raise SystemExit(main())
