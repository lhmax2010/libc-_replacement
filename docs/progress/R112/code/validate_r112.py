#!/usr/bin/env python3
import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
BASE = ROOT / "docs/progress/R112"

def rows(name):
    with (BASE / "tables" / name).open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))

def main() -> int:
    item1 = rows("item1_cancel_matrix.tsv")
    assert len(item1) == 35
    for row in item1:
        cell = row["cell"]
        if cell == "oldcaller_retained":
            assert row["returncode"] == "86" and row["terminate"] == "1"
        elif cell in {"oldcaller_quiet_removed", "fno_oldcaller_removed",
                      "fno_oldcaller_quiet_removed"}:
            assert row["joined_canceled"] == "1" and row["frame_cleanup"] == "0"
            assert row["trylock_busy"] == "1" and row["contender_timeout"] == "1"
        else:
            assert row["joined_canceled"] == "1" and row["frame_cleanup"] == "1"
            assert row["target_unlock"] == "1"
    item2 = rows("item2_comdat_matrix.tsv")
    assert len(item2) == 30
    assert all((r["binary"] == "new_first") == (r["caught_system_error"] == "1") for r in item2)
    assert all((r["binary"] != "new_first") == (r["terminate"] == "1") for r in item2)
    item3 = rows("item3_wbuffer_matrix.tsv")
    assert len(item3) == 300
    assert all(r["returncode"] == "0" and r["all_x"] == "1" for r in item3)
    filesystem = rows("item4_actual_direction_matrix.tsv")
    boost_test = rows("item4_boost_test_actual_direction_matrix.tsv")
    assert len(filesystem) == 10 and len(boost_test) == 10
    assert sum(r["exact_value"] == "1" for r in filesystem) == 5
    assert sum(r["destruction_completed"] == "1" for r in filesystem) == 10
    assert sum(r["exact_value"] == "1" for r in boost_test) == 5
    assert sum(r["returncode"] == "-11" for r in boost_test) == 5
    binding = rows("item4_partial_binding_matrix.tsv")
    assert len(binding) == 15
    tags = rows("item4_consumer_binding_tags.tsv")
    assert len(tags) == 84 and all(r["bind_now"] == "0" for r in tags)
    assert sum(r["gnu_relro"] == "1" for r in tags) == 75
    retention = rows("item4_e029_symbol_retention.tsv")
    assert len(retention) == 33
    assert sum(r["old_provider_defined"] == "1" for r in retention) == 33
    assert sum(r["adapted_provider_same_mangled_defined"] == "1" for r in retention) == 21
    for path in sorted((BASE / "raw").glob("*.command.txt")):
        stem = path.name.removesuffix(".command.txt")
        for suffix in (".stdout", ".stderr"):
            assert (BASE / "raw" / f"{stem}{suffix}").exists(), (stem, suffix)
    for gate in ("002", "016", "022", "029", "052"):
        matches = list((BASE / "raw").glob(f"{gate}_resource_gate_*.stdout"))
        assert len(matches) == 1 and "gate_result=PASS" in matches[0].read_text()
    print("VALIDATION=PASS item1=35 item2=30 item3=300 filesystem=10 boost_test=10 binding=15 tags=84 retention=33")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
