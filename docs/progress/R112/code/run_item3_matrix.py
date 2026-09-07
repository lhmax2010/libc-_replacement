#!/usr/bin/env python3
import csv
import os
import re
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
BUILD = ROOT / "docs/progress/R112/build/item3"
RUNS = BUILD / "runs"
TABLES = ROOT / "docs/progress/R112/tables"
RUNTIME = Path("/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib")
LENGTHS = (1, 3, 100, 4094, 4095, 4096, 4097, 5000, 8191, 8192)


def main() -> int:
    RUNS.mkdir(parents=True, exist_ok=True)
    TABLES.mkdir(parents=True, exist_ok=True)
    rows = []
    for build_kind in ("retained", "removed"):
        for action in ("destructor", "pubsync", "seekoff"):
            for length in LENGTHS:
                for repeat in range(1, 6):
                    env = os.environ.copy()
                    env["LD_LIBRARY_PATH"] = str(RUNTIME)
                    completed = subprocess.run(
                        [str(BUILD / build_kind), str(length), action], cwd=ROOT, env=env,
                        text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=5,
                    )
                    match = re.search(r"output_bytes=(\d+).*all_x=(\d+).*action_rc=(-?\d+)", completed.stdout)
                    if match is None:
                        raise AssertionError((build_kind, action, length, repeat, completed.stdout, completed.stderr))
                    output_bytes, all_x, action_rc = map(int, match.groups())
                    stem = RUNS / f"{build_kind}_{action}_{length}_{repeat}"
                    stem.with_suffix(".stdout").write_text(completed.stdout)
                    stem.with_suffix(".stderr").write_text(completed.stderr)
                    stem.with_suffix(".exitcode").write_text(f"{completed.returncode}\n")
                    stem.with_suffix(".command.txt").write_text(
                        f"cwd: {ROOT}\ncommand: LD_LIBRARY_PATH={RUNTIME} {BUILD / build_kind} {length} {action}\n"
                    )
                    rows.append({
                        "build": build_kind, "action": action, "input_wchars": length,
                        "repeat": repeat, "returncode": completed.returncode,
                        "output_bytes": output_bytes, "all_x": all_x, "action_rc": action_rc,
                    })
                    assert completed.returncode == 0 and all_x == 1
                    if build_kind == "retained" or action != "destructor":
                        assert output_bytes == length
                    if action != "destructor":
                        assert action_rc == 0
    with (TABLES / "item3_wbuffer_matrix.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    for build_kind in ("retained", "removed"):
        for action in ("destructor", "pubsync", "seekoff"):
            selected = [row for row in rows if row["build"] == build_kind and row["action"] == action]
            values = {int(row["input_wchars"]): int(row["output_bytes"]) for row in selected}
            print(build_kind, action, values)
    print(f"ASSERTIONS=PASS rows={len(rows)} repeats=5")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
