#!/usr/bin/env python3
import csv
import os
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
BUILD = ROOT / "docs/progress/R112/build/item2"
RUNS = BUILD / "runs"
TABLES = ROOT / "docs/progress/R112/tables"
RUNTIME = Path("/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib")


def main() -> int:
    RUNS.mkdir(parents=True, exist_ok=True)
    TABLES.mkdir(parents=True, exist_ok=True)
    rows = []
    for binary in ("old_first", "new_first", "archive_old_first"):
        for entry in ("old", "new"):
            for repeat in range(1, 6):
                env = os.environ.copy()
                env["LD_LIBRARY_PATH"] = str(RUNTIME)
                completed = subprocess.run(
                    [str(BUILD / binary), entry], cwd=ROOT, env=env, text=True,
                    stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=5,
                )
                stem = RUNS / f"{binary}_{entry}_{repeat}"
                stem.with_suffix(".stdout").write_text(completed.stdout)
                stem.with_suffix(".stderr").write_text(completed.stderr)
                stem.with_suffix(".exitcode").write_text(f"{completed.returncode}\n")
                stem.with_suffix(".command.txt").write_text(
                    f"cwd: {ROOT}\ncommand: LD_LIBRARY_PATH={RUNTIME} {BUILD / binary} {entry}\n"
                )
                rows.append({
                    "binary": binary,
                    "entry": entry,
                    "repeat": repeat,
                    "returncode": completed.returncode,
                    "terminate": int("event=terminate" in completed.stdout),
                    "caught_system_error": int("event=caught_system_error" in completed.stdout),
                })
    with (TABLES / "item2_comdat_matrix.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    for binary in ("old_first", "new_first", "archive_old_first"):
        selected = [row for row in rows if row["binary"] == binary]
        rcs = sorted({int(row["returncode"]) for row in selected})
        print(binary, "runs", len(selected), "returncodes", rcs,
              "terminate", sum(int(row["terminate"]) for row in selected),
              "caught", sum(int(row["caught_system_error"]) for row in selected))
        assert len(rcs) == 1
        assert (rcs == [86] and sum(int(row["terminate"]) for row in selected) == 10) or (
            rcs == [42] and sum(int(row["caught_system_error"]) for row in selected) == 10
        )
    print("ASSERTIONS=PASS repeats=5 entries=2")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
