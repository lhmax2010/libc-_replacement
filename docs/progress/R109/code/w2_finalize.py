#!/usr/bin/env python3
import argparse
import csv
from collections import Counter
from pathlib import Path


def read(path):
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--status", required=True, type=Path)
    parser.add_argument("--hits", required=True, type=Path)
    parser.add_argument("--r92-completeness", required=True, type=Path)
    parser.add_argument("--link-summary", required=True, type=Path)
    parser.add_argument("--partial-output", required=True, type=Path)
    parser.add_argument("--rollup-output", required=True, type=Path)
    parser.add_argument("--summary-output", required=True, type=Path)
    args = parser.parse_args()
    manifest = {r["source_rpm"]: r for r in read(args.manifest)}
    status = read(args.status)
    hits = read(args.hits)
    r92 = read(args.r92_completeness)
    link_summary = {r["source_rpm"]: r for r in read(args.link_summary)}
    if len(status) != 965 or len(r92) != 140:
        raise RuntimeError("coverage denominator mismatch")
    r92_counts = Counter(r["source_content_status"] for r in r92)
    full_r92 = r92_counts["AVAILABLE_AND_SCANNED"] + r92_counts["AVAILABLE_AND_SCANNED_R93_LINKS_CLOSED"]
    partial_r92 = r92_counts["PARTIAL_DANGLING_LINK_TARGETS_NOT_AVAILABLE"]
    unavailable_r92 = r92_counts["NOT_AVAILABLE"]
    w2_counts = Counter(r["status"] for r in status)
    partial = []
    link_closed = 0
    for row in status:
        if row["status"] != "PARTIAL":
            continue
        item = manifest[row["source_rpm"]]
        audit = link_summary[row["source_rpm"]]
        if audit["wbuffer_coverage"] == "CLOSED_FOR_WBUFFER_SCAN":
            link_closed += 1
        partial.append({
            "source_rpm": row["source_rpm"], "package_name": row["package_name"],
            "compressed_bytes": row["compressed_bytes"],
            "published_elf_rows": item["published_elf_rows"],
            "runtime_elf_rows": item["runtime_elf_rows"],
            "files_scanned": row["files_scanned"], "archives_seen": row["archives_seen"],
            "skipped_members": row["skipped_members"],
            "missing_link_targets": row["missing_link_targets"],
            "visible_hits": row["hits"],
            "closed_by_link_chain": audit["closed_by_link_chain"],
            "targets_not_in_archive": audit["targets_not_in_archive"],
            "cxx_source_paths": audit["cxx_source_paths"],
            "unknown_path_kind": audit["unknown_path_kind"],
            "non_cxx_paths": audit["non_cxx_paths"],
            "wbuffer_coverage": audit["wbuffer_coverage"],
            "risk_basis": "missing link targets carry no in-archive content; their external target content is unavailable",
        })
    write(args.partial_output, list(partial[0]), partial)
    rollup = [
        {"source_subset": "R87 exact frozen records", "records": 345, "full": 345, "partial": 0, "not_available": 0, "visible_hits": 0},
        {"source_subset": "R92 C++-indicator gap", "records": 140, "full": full_r92, "partial": partial_r92, "not_available": unavailable_r92, "visible_hits": 0},
        {"source_subset": "R109 no-C++-indicator gap", "records": 965, "full": w2_counts["SCAN_OK"] + link_closed, "partial": w2_counts["PARTIAL"] - link_closed, "not_available": w2_counts["NOT_AVAILABLE"], "visible_hits": len(hits)},
        {"source_subset": "R11 frozen total", "records": 1450, "full": 345 + full_r92 + w2_counts["SCAN_OK"] + link_closed, "partial": partial_r92 + w2_counts["PARTIAL"] - link_closed, "not_available": unavailable_r92 + w2_counts["NOT_AVAILABLE"], "visible_hits": len(hits)},
    ]
    write(args.rollup_output, tuple(rollup[0]), rollup)
    metrics = [
        ("w2_manifest_records", len(manifest)),
        ("w2_scan_status_records", len(status)),
        ("w2_scan_ok", w2_counts["SCAN_OK"]),
        ("w2_partial", w2_counts["PARTIAL"]),
        ("w2_partial_closed_for_wbuffer_by_link_audit", link_closed),
        ("w2_partial_remaining_for_wbuffer", w2_counts["PARTIAL"] - link_closed),
        ("w2_not_available", w2_counts["NOT_AVAILABLE"]),
        ("w2_visible_hits", len(hits)),
        ("w2_files_scanned", sum(int(r["files_scanned"]) for r in status)),
        ("w2_archives_seen", sum(int(r["archives_seen"]) for r in status)),
        ("w2_archives_expanded", sum(int(r["archives_expanded"]) for r in status)),
        ("w2_skipped_members", sum(int(r["skipped_members"]) for r in status)),
        ("w2_missing_link_targets", sum(int(r["missing_link_targets"]) for r in status)),
        ("w2_partial_compressed_bytes", sum(int(r["compressed_bytes"]) for r in partial)),
        ("w2_partial_published_elf_rows", sum(int(r["published_elf_rows"]) for r in partial)),
        ("frozen_full_records", rollup[-1]["full"]),
        ("frozen_partial_records", rollup[-1]["partial"]),
        ("frozen_not_available_records", rollup[-1]["not_available"]),
    ]
    write(args.summary_output, ("metric", "value"), [{"metric": k, "value": v} for k, v in metrics])
    for key, value in metrics:
        print(f"{key}={value}")
    return 0 if len(manifest) == len(status) and not hits and not w2_counts["NOT_AVAILABLE"] else 2


if __name__ == "__main__":
    raise SystemExit(main())
