#!/usr/bin/env python3
import csv
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
source = ROOT / "docs/progress/R109/tables/w3_edges_90_results.tsv"
out_dir = ROOT / "docs/progress/R110/tables"
out_dir.mkdir(parents=True, exist_ok=True)

observations = {
    "ut_detail::normalize_test_case_name": (
        "BOOST_TEST_NORMALIZE",
        "SILENT_WRONG_VALUE_SIZE_56;DESTRUCTION_COMPLETED_5_OF_5",
    ),
    "enter/rollback/commit_once_region": (
        "BOOST_THREAD_ONCE_STATE",
        "EXACT_STATES_1_1_0;TRIVIAL_DESTRUCTION_COMPLETED_5_OF_5",
    ),
    "status/read_symlink": (
        "BOOST_FILESYSTEM_STATUS_READ_SYMLINK",
        "WRONG_STATUS_AND_VALUE;DESTRUCTION_COMPLETED_5_OF_5",
    ),
    "mapped_file_source::open_impl": (
        "BOOST_IOSTREAMS_MAPPED_FILE",
        "TERMINATED_BY_LENGTH_ERROR_BEFORE_VALUE;DESTRUCTION_NOT_OBSERVED_5_OF_5",
    ),
    "variables_map constructor": (
        "BOOST_PROGRAM_OPTIONS_VARIABLES_MAP",
        "EXACT_VALUE_THEN_SIGSEGV_DURING_DESTRUCTION_5_OF_5",
    ),
}

with source.open(newline="") as handle:
    reader = csv.DictReader(handle, delimiter="\t")
    fields = reader.fieldnames
    assert fields is not None
    rows = list(reader)

selected = []
for row in rows:
    group = row["tested_symbol_group"]
    eligible_status = row["runtime_status"] in {
        "W3_SYMBOL_GROUP_ARCH_NOT_OBSERVED",
        "PRIOR_SYMBOL_GROUP_ARCH_NOT_OBSERVED",
    }
    if row["arch"] == "aarch64" and eligible_status and group in observations:
        family, observed = observations[group]
        row["runtime_status"] = "MEASURED_R110_AARCH64"
        row["selection_reason"] = (
            "R110 used the real R11 aarch64 provider under QEMU user mode; "
            "five mixed and five same-runtime control repetitions"
        )
        row["representative_symbol_measured_on_arch"] = "YES"
        row["observed_result"] = observed
        row["unmeasured_risk"] = (
            "MEASURED_FOR_THIS_SYMBOL_GROUP_AND_ARCHITECTURE_ONLY;"
            "OTHER_SYMBOLS_AND_ARCHITECTURES_NOT_INFERRED"
        )
        row["measurement_extent"] = (
            "ONE_REPRESENTATIVE_SYMBOL_GROUP_ON_EDGE_NOT_ALL_CPP_SYMBOLS"
        )
        selected.append((row["edge_id"], family, row["requested_class"], observed))

def write_rows(path: Path, data):
    with path.open("w", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(data)

write_rows(out_dir / "w3_edges_90_results.tsv", rows)
remaining = [row for row in rows if row["representative_symbol_measured_on_arch"] != "YES"]
write_rows(out_dir / "w3_remaining_edges.tsv", remaining)

with (out_dir / "w3_selected_edges.tsv").open("w", newline="") as handle:
    writer = csv.writer(handle, delimiter="\t", lineterminator="\n")
    writer.writerow(["edge_id", "family", "requested_class", "observed_result"])
    writer.writerows(selected)

status_counts = Counter((row["requested_class"], row["runtime_status"]) for row in rows)
with (out_dir / "w3_edge_status_summary.tsv").open("w", newline="") as handle:
    writer = csv.writer(handle, delimiter="\t", lineterminator="\n")
    writer.writerow(["requested_class", "runtime_status", "edges"])
    for (classification, status), count in sorted(status_counts.items()):
        writer.writerow([classification, status, count])

family_counts = defaultdict(lambda: Counter())
for _, family, classification, _ in selected:
    family_counts[family][classification] += 1
with (out_dir / "w3_selection_summary.tsv").open("w", newline="") as handle:
    writer = csv.writer(handle, delimiter="\t", lineterminator="\n")
    writer.writerow(["family", "layout_sensitive_edges", "layout_unproven_edges", "total_edges"])
    for family in sorted(family_counts):
        counts = family_counts[family]
        layout = counts["LAYOUT_SENSITIVE"]
        unproven = counts["CPP_ABI_COUPLED_LAYOUT_UNPROVEN"]
        writer.writerow([family, layout, unproven, layout + unproven])

print(f"TOTAL_EDGES={len(rows)}")
print(f"PRIOR_MEASURED={sum(1 for row in rows if row['representative_symbol_measured_on_arch'] == 'YES') - len(selected)}")
print(f"NEWLY_MEASURED={len(selected)}")
print(f"MEASURED_TOTAL={sum(1 for row in rows if row['representative_symbol_measured_on_arch'] == 'YES')}")
print(f"REMAINING={len(remaining)}")
