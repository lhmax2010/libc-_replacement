#!/usr/bin/env python3
import csv
import os
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
BUILD = ROOT / "docs/progress/R112/build/item1"
RUNS = BUILD / "runs"
TABLES = ROOT / "docs/progress/R112/tables"
RUNTIME = Path("/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib")


def main() -> int:
    RUNS.mkdir(parents=True, exist_ok=True)
    TABLES.mkdir(parents=True, exist_ok=True)
    rows = []
    for cell in (
        "oldcaller_retained", "oldcaller_removed", "oldcaller_quiet_removed",
        "fno_oldcaller_removed", "fno_oldcaller_quiet_removed",
        "newcaller_removed", "newcaller_quiet_removed",
    ):
        for repeat in range(1, 6):
            env = os.environ.copy()
            env["LD_LIBRARY_PATH"] = f"{BUILD}:{RUNTIME}"
            completed = subprocess.run(
                [str(BUILD / cell)], cwd=ROOT, env=env, text=True,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30,
            )
            stem = RUNS / f"{cell}_{repeat}"
            stem.with_suffix(".stdout").write_text(completed.stdout)
            stem.with_suffix(".stderr").write_text(completed.stderr)
            stem.with_suffix(".exitcode").write_text(f"{completed.returncode}\n")
            stem.with_suffix(".command.txt").write_text(
                f"cwd: {ROOT}\ncommand: LD_LIBRARY_PATH={BUILD}:{RUNTIME} {BUILD / cell}\n"
            )
            row = {
                "cell": cell,
                "repeat": repeat,
                "returncode": completed.returncode,
                "terminate": int("event=terminate" in completed.stdout),
                "joined_canceled": int("canceled=1" in completed.stdout),
                "frame_cleanup": int("frame_cleanup=1" in completed.stdout),
                "target_unlock": int("target_unlock=1" in completed.stdout),
                "trylock_busy": int("event=trylock rc=16" in completed.stdout),
                "contender_timeout": int("contender_rc=110" in completed.stdout),
                "assert_pass": int("event=assert expected=1" in completed.stdout),
            }
            rows.append(row)
            if cell == "oldcaller_retained":
                assert completed.returncode == 86 and row["terminate"] and not row["frame_cleanup"]
            elif cell.startswith("newcaller_"):
                assert completed.returncode == 0 and row["assert_pass"] and row["frame_cleanup"] and row["target_unlock"]
            else:
                assert completed.returncode == 0 and row["assert_pass"]
    with (TABLES / "item1_cancel_matrix.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    cells = sorted({str(row["cell"]) for row in rows})
    print(f"ASSERTIONS=PASS cells={len(cells)} repeats=5")
    for cell in cells:
        selected = [row for row in rows if row["cell"] == cell]
        print(cell, {key: sum(int(row[key]) for row in selected) for key in list(selected[0])[3:]},
              "returncodes", sorted({int(row["returncode"]) for row in selected}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
