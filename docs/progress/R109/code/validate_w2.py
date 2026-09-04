#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path


ROOT = Path("docs/progress/R109")


def rows(path):
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def check(value, message):
    if not value:
        raise RuntimeError(message)
    print(f"PASS\t{message}")


def main():
    check((ROOT / "raw/100_resource_gate_w2_resource_gate_w2.exitcode").read_text().strip() == "0", "W2 resource gate passed")
    manifest = rows(ROOT / "tables/w2_no_cpp_gap_manifest.tsv")
    check(len(manifest) == 965 and all(r["local_file_present"] == "YES" and r["download_result"] == "PASS" for r in manifest), "965 manifest inputs are present and downloaded successfully")
    check(len(rows(ROOT / "tables/w2_positive_control_hits.tsv")) == 1, "same-method positive control has one hit")
    check((ROOT / "raw/105_independent_w2_control_independent_w2_control.exitcode").read_text().strip() == "0", "independent positive control passed")
    status = rows(ROOT / "tables/w2_source_scan_status.tsv")
    counts = Counter(r["status"] for r in status)
    check(len(status) == 965 and counts == Counter({"SCAN_OK": 925, "PARTIAL": 40}), "all 965 inputs scanned with expected raw completeness split")
    check(sum(int(r["hits"]) for r in status) == 0, "new source scan has zero visible hits")
    check(sum(int(r["archives_seen"]) for r in status) == 987 and sum(int(r["archives_expanded"]) for r in status) == 987, "all 987 embedded archives expanded")
    links = rows(ROOT / "tables/w2_missing_link_summary.tsv")
    check(len(links) == 40 and sum(r["wbuffer_coverage"] == "CLOSED_FOR_WBUFFER_SCAN" for r in links) == 18, "link audit closed 18 of 40 target-specific partial packages")
    rollup = rows(ROOT / "tables/w2_source_coverage_rollup.tsv")[-1]
    check((rollup["records"], rollup["full"], rollup["partial"], rollup["not_available"], rollup["visible_hits"]) == ("1450", "1424", "24", "2", "0"), "frozen source rollup is 1424 full, 24 partial, 2 unavailable, zero visible hits")
    elf = {r["metric"]: r["value"] for r in rows(Path("docs/progress/R108/tables/w1_published_elf_summary.tsv"))}
    check(elf == {"package_records": "11332", "elf_records": "39166", "matching_symbol_rows": "0", "parse_failures": "0"}, "prior published ELF evidence has 39166 successful zero-hit records")
    check((ROOT / "W2_REPORT.md").is_file(), "W2 report exists")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
