#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path

root = Path(__file__).resolve().parents[4]
base = root / "docs/progress/R110"

def read_tsv(path):
    with path.open(newline="") as handle:
        return list(csv.DictReader(handle, delimiter="\t"))

edges = read_tsv(base / "tables/w3_edges_90_results.tsv")
selected = read_tsv(base / "tables/w3_selected_edges.tsv")
remaining = read_tsv(base / "tables/w3_remaining_edges.tsv")
results = read_tsv(base / "build/w3/runtime_results.tsv")
commands = read_tsv(base / "build/w3/runtime_commands.tsv")

assert len(edges) == 90
assert len(selected) == 22
assert Counter(row["requested_class"] for row in selected) == {
    "LAYOUT_SENSITIVE": 14,
    "CPP_ABI_COUPLED_LAYOUT_UNPROVEN": 8,
}
assert sum(row["representative_symbol_measured_on_arch"] == "YES" for row in edges) == 49
assert len(remaining) == 41
assert len(results) == 50
assert len(commands) == 50
assert Counter(row["case"] for row in results) == {
    "boost_test_mixed": 5,
    "boost_test_control": 5,
    "boost_thread_mixed": 5,
    "boost_thread_control": 5,
    "boost_filesystem_mixed": 5,
    "boost_filesystem_control": 5,
    "boost_iostreams_mixed": 5,
    "boost_iostreams_control": 5,
    "boost_program_options_mixed": 5,
    "boost_program_options_control": 5,
}

for row in results:
    if row["case"].endswith("_mixed"):
        assert (row["maps_libcxx"], row["maps_libstdcxx"], row["maps_provider"]) == ("1", "1", "1")
    else:
        assert (row["exit_code"], row["maps_libcxx"], row["maps_libstdcxx"], row["maps_provider"]) == ("0", "0", "1", "1")
        assert row["value_state"] == "EXACT_PASS"
        assert row["destruction_state"] in {"COMPLETED", "COMPLETED_TRIVIAL"}

expected_mixed = {
    "boost_test_mixed": ("41", "OBSERVED_MISMATCH", "COMPLETED"),
    "boost_thread_mixed": ("0", "EXACT_PASS", "COMPLETED_TRIVIAL"),
    "boost_filesystem_mixed": ("41", "OBSERVED_MISMATCH", "COMPLETED"),
    "boost_iostreams_mixed": ("134", "NOT_OBSERVED", "NOT_OBSERVED"),
    "boost_program_options_mixed": ("139", "EXACT_PASS", "NOT_OBSERVED"),
}
for case_name, expected in expected_mixed.items():
    rows = [row for row in results if row["case"] == case_name]
    assert len(rows) == 5
    assert all((row["exit_code"], row["value_state"], row["destruction_state"]) == expected for row in rows)

missing_exit = []
for command_file in sorted((base / "raw").glob("*.command.txt")):
    if "validate_w3_evidence" in command_file.name:
        continue
    exit_file = command_file.with_name(command_file.name.removesuffix(".command.txt") + ".exitcode")
    if not exit_file.exists():
        missing_exit.append(str(exit_file.relative_to(root)))
assert not missing_exit, missing_exit

report = (base / "W3_REPORT.md").read_text()
for token in ["PARTIAL", "49/90", "41/90", "NOT_OBSERVED", "QEMU 用户态"]:
    assert token in report

print("W3_VALIDATION=PASS")
print("edges=90 selected_new=22 measured_total=49 remaining=41 runtime_rows=50 command_rows=50")
