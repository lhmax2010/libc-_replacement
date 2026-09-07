#!/usr/bin/env python3
from __future__ import annotations

import csv
import os
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
BUILD = ROOT / "docs/progress/R113/build/item1"
RUNS = BUILD / "runs"
TABLE = ROOT / "docs/progress/R113/tables/item1_facility_cancel.tsv"
RUNTIME = Path("/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib")
MODES = ("timed_mutex", "shared_mutex", "shared_timed_mutex", "future_wait", "cv_any_wait", "cv_any_throw")


def main() -> None:
    RUNS.mkdir(parents=True, exist_ok=True)
    TABLE.parent.mkdir(parents=True, exist_ok=True)
    rows: list[dict[str, object]] = []
    env = os.environ.copy()
    env["LD_LIBRARY_PATH"] = f"{BUILD}:{RUNTIME}"
    binary = BUILD / "item1_facility_cancel"
    for mode in MODES:
        for repeat in range(1, 6):
            stem = RUNS / f"{mode}_{repeat}"
            completed = subprocess.run(
                [str(binary), mode], cwd=ROOT, env=env, text=True,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=10,
            )
            stem.with_suffix(".stdout").write_text(completed.stdout)
            stem.with_suffix(".stderr").write_text(completed.stderr)
            stem.with_suffix(".exitcode").write_text(f"{completed.returncode}\n")
            stem.with_suffix(".command.txt").write_text(
                f"cwd: {ROOT}\ncommand: LD_LIBRARY_PATH={BUILD}:{RUNTIME} {binary} {mode}\n"
            )
            row = {
                "mode": mode,
                "repeat": repeat,
                "returncode": completed.returncode,
                "blocked": int("second_gate=1" in completed.stdout),
                "canceled": int("canceled=1" in completed.stdout),
                "cleanup": int("event=cleanup\n" in completed.stdout),
                "terminate": int("event=terminate\n" in completed.stdout),
                "reusable": int("reusable=1" in completed.stdout),
                "internal_mutex_unlocked": int("internal_mutex_unlocked=1" in completed.stdout),
                "write_entered": int("write_entered=1" in completed.stdout or "write_entered_observed=1" in completed.stdout),
                "reader_blocked": int("reader_blocked=1" in completed.stdout),
                "writer_blocked": int("writer_blocked=1" in completed.stdout),
            }
            rows.append(row)
            if mode == "cv_any_throw":
                assert completed.returncode == 86 and row["blocked"] and row["terminate"]
            elif mode in ("shared_mutex", "shared_timed_mutex"):
                assert completed.returncode == 0 and row["blocked"] and row["canceled"] and row["cleanup"]
                assert row["write_entered"] and not row["reusable"]
                if mode == "shared_mutex":
                    assert row["internal_mutex_unlocked"] and row["reader_blocked"] and row["writer_blocked"]
            else:
                assert completed.returncode == 0 and row["blocked"] and row["canceled"] and row["cleanup"] and row["reusable"]
    with TABLE.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    print(f"ASSERTIONS=PASS rows={len(rows)} repeats=5 modes={len(MODES)}")
    for mode in MODES:
        selected = [row for row in rows if row["mode"] == mode]
        print(
            f"mode={mode} rc={sorted({row['returncode'] for row in selected})} "
            f"canceled={sum(int(row['canceled']) for row in selected)}/5 "
            f"cleanup={sum(int(row['cleanup']) for row in selected)}/5 "
            f"terminate={sum(int(row['terminate']) for row in selected)}/5 "
            f"reusable={sum(int(row['reusable']) for row in selected)}/5 "
            f"write_entered={sum(int(row['write_entered']) for row in selected)}/5"
        )


if __name__ == "__main__":
    main()
