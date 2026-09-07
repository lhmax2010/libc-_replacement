#!/usr/bin/env python3
from __future__ import annotations

import csv
import os
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
BUILD = ROOT / "docs/progress/R113/build/item3"
DUAL = BUILD / "dual"
RUNTIME = Path("/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib")
RUNS = BUILD / "runs"
TABLE = ROOT / "docs/progress/R113/tables/item3_dual_version.tsv"
CASES = {
    "old_prebound_old": ("LLVM_22", "old", 86, "terminate"),
    "old_relinked_dual": ("LLVM_23", "new", 0, "retained_lock"),
    "new_linked_dual": ("LLVM_23", "new", 0, "clean"),
    "new_rebuilt_before_dual": ("LLVM_22", "old", 86, "terminate"),
}


def version(binary: Path) -> str:
    output = subprocess.check_output(["readelf", "-V", str(binary)], text=True)
    wait_lines = subprocess.check_output(["readelf", "-Ws", str(binary)], text=True)
    relevant_indices = []
    for line in wait_lines.splitlines():
        if "_ZNSt3__118condition_variable4wait" in line and "UND" in line:
            relevant_indices.append(line)
    if not relevant_indices:
        raise AssertionError(f"no wait reference in {binary}")
    for candidate in ("LLVM_22", "LLVM_23"):
        if candidate in output and any(candidate in line for line in relevant_indices):
            return candidate
    raise AssertionError(f"wait version not found for {binary}: {relevant_indices}")


def main() -> None:
    RUNS.mkdir(parents=True, exist_ok=True)
    TABLE.parent.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["LD_LIBRARY_PATH"] = f"{DUAL}:{RUNTIME}"
    rows: list[dict[str, object]] = []
    for case, (expected_version, expected_wrapper, expected_rc, expected_behavior) in CASES.items():
        binary = BUILD / case
        actual_version = version(binary)
        assert actual_version == expected_version
        for repeat in range(1, 6):
            stem = RUNS / f"{case}_{repeat}"
            completed = subprocess.run(
                [str(binary)], cwd=ROOT, env=env, text=True,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=5,
            )
            stem.with_suffix(".stdout").write_text(completed.stdout)
            stem.with_suffix(".stderr").write_text(completed.stderr)
            stem.with_suffix(".exitcode").write_text(f"{completed.returncode}\n")
            stem.with_suffix(".command.txt").write_text(
                f"cwd: {ROOT}\ncommand: LD_LIBRARY_PATH={DUAL}:{RUNTIME} {binary}\n"
            )
            wrapper = "old" if "wrapper=old" in completed.stdout else "new" if "wrapper=new" in completed.stdout else "none"
            terminate = "event=terminate" in completed.stdout
            clean = "frame_cleanup=1 target_unlock=1" in completed.stdout and "event=trylock rc=0" in completed.stdout
            retained_lock = "frame_cleanup=0 target_unlock=0" in completed.stdout and "event=trylock rc=16" in completed.stdout and "contender_rc=110" in completed.stdout
            behavior = "terminate" if terminate else "clean" if clean else "retained_lock" if retained_lock else "other"
            row = {
                "case": case,
                "repeat": repeat,
                "wait_version": actual_version,
                "wrapper": wrapper,
                "returncode": completed.returncode,
                "behavior": behavior,
            }
            rows.append(row)
            assert wrapper == expected_wrapper and completed.returncode == expected_rc and behavior == expected_behavior
    with TABLE.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    print(f"ASSERTIONS=PASS rows={len(rows)} cases={len(CASES)} repeats=5")
    for case, expected in CASES.items():
        selected = [row for row in rows if row["case"] == case]
        print(
            f"case={case} version={sorted({row['wait_version'] for row in selected})} "
            f"wrapper={sorted({row['wrapper'] for row in selected})} "
            f"rc={sorted({row['returncode'] for row in selected})} "
            f"behavior={sorted({row['behavior'] for row in selected})}"
        )


if __name__ == "__main__":
    main()
