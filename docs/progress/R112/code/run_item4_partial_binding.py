#!/usr/bin/env python3
import csv
import os
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
OUT = ROOT / "docs/progress/R112/build/item4_partial"
RUNS = OUT / "runs"
TABLE = ROOT / "docs/progress/R112/tables/item4_partial_binding_matrix.tsv"

def main() -> int:
    RUNS.mkdir(parents=True, exist_ok=True)
    cells = (
        ("lazy_profile_load_without_missing_call", "consumer_lazy_profile", False),
        ("lazy_profile_first_missing_call", "consumer_lazy_profile", True),
        ("now_control_without_missing_call", "consumer_now_control", False),
    )
    rows = []
    env = dict(os.environ, LD_LIBRARY_PATH=str(OUT / "new"))
    for cell, binary, invoke_missing in cells:
        for repeat in range(1, 6):
            command = [str(OUT / binary)] + (["call-missing"] if invoke_missing else [])
            completed = subprocess.run(command, cwd=ROOT, env=env, text=True,
                                       stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       timeout=10)
            stem = RUNS / f"{cell}_{repeat}"
            stem.with_suffix(".stdout").write_text(completed.stdout)
            stem.with_suffix(".stderr").write_text(completed.stderr)
            stem.with_suffix(".exitcode").write_text(f"{completed.returncode}\n")
            stem.with_suffix(".command.txt").write_text(
                f"cwd: {ROOT}\ncommand: LD_LIBRARY_PATH={OUT / 'new'} {' '.join(command)}\n")
            rows.append({"cell": cell, "repeat": repeat,
                         "returncode": completed.returncode,
                         "main_entered": int("main_entered=YES" in completed.stdout),
                         "before_missing": int("before_missing=YES" in completed.stdout),
                         "symbol_lookup_error": int("symbol lookup error" in completed.stderr and
                                                    "missing" in completed.stderr)})
    with TABLE.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    for cell, _, _ in cells:
        selected = [row for row in rows if row["cell"] == cell]
        print(cell, "returncodes", sorted({r["returncode"] for r in selected}),
              "main", sum(r["main_entered"] for r in selected),
              "before_missing", sum(r["before_missing"] for r in selected),
              "lookup_error", sum(r["symbol_lookup_error"] for r in selected))
    assert all(r["returncode"] == 0 and r["main_entered"] for r in rows
               if r["cell"] == "lazy_profile_load_without_missing_call")
    assert all(r["returncode"] == 127 and r["main_entered"] and r["before_missing"] and
               r["symbol_lookup_error"] for r in rows
               if r["cell"] == "lazy_profile_first_missing_call")
    assert all(r["returncode"] == 127 and not r["main_entered"] and
               r["symbol_lookup_error"] for r in rows
               if r["cell"] == "now_control_without_missing_call")
    print("ASSERTIONS=PASS repeats=5 lazy_first_call_and_now_load_control=PASS")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
