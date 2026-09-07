#!/usr/bin/env python3
from __future__ import annotations

import csv
import os
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
BUILD = ROOT / "docs/progress/R111/build/item3"
OUT = ROOT / "docs/progress/R111/tables"
RUNS = BUILD / "runs"
RUNTIME = Path("/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib")
MODES = ("wait", "system_clock", "steady_clock", "custom_clock")


def run_one(binary: Path, argument: str, stem: str) -> tuple[int, str, str]:
    env = os.environ.copy()
    env["LD_LIBRARY_PATH"] = f"{BUILD}:{RUNTIME}"
    completed = subprocess.run(
        [str(binary), argument],
        cwd=ROOT,
        env=env,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=10,
    )
    (RUNS / f"{stem}.stdout").write_text(completed.stdout, encoding="utf-8")
    (RUNS / f"{stem}.stderr").write_text(completed.stderr, encoding="utf-8")
    (RUNS / f"{stem}.exitcode").write_text(f"{completed.returncode}\n", encoding="utf-8")
    (RUNS / f"{stem}.command.txt").write_text(
        f"cwd: {ROOT}\ncommand: LD_LIBRARY_PATH={BUILD}:{RUNTIME} {binary} {argument}\n",
        encoding="utf-8",
    )
    return completed.returncode, completed.stdout, completed.stderr


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    RUNS.mkdir(parents=True, exist_ok=True)
    cancel_rows: list[dict[str, object]] = []
    for build_kind in ("retained", "removed"):
        binary = BUILD / f"condition_cancel_probe_{build_kind}"
        for mode in MODES:
            for repeat in range(1, 6):
                stem = f"cancel_{build_kind}_{mode}_{repeat}"
                rc, stdout, stderr = run_one(binary, mode, stem)
                row = {
                    "build": build_kind,
                    "mode": mode,
                    "repeat": repeat,
                    "returncode": rc,
                    "wait_confirmed": int("event=wait_confirmed" in stdout),
                    "terminate": int("event=terminate" in stdout),
                    "cleanup": int("event=cleanup\n" in stdout),
                    "joined_canceled": int("canceled=1 cleanup_count=1" in stdout),
                    "unexpected_return": int("event=unexpected_return" in stdout),
                    "stderr_nonempty": int(bool(stderr)),
                }
                cancel_rows.append(row)
                if build_kind == "retained":
                    assert rc == 86 and row["wait_confirmed"] and row["terminate"] and not row["cleanup"]
                else:
                    assert rc == 0 and row["wait_confirmed"] and not row["terminate"] and row["cleanup"] and row["joined_canceled"]
                assert not row["unexpected_return"]

    with (OUT / "item3_cancel_matrix.tsv").open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(cancel_rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(cancel_rows)

    ordinary_rows: list[dict[str, object]] = []
    for build_kind in ("retained", "removed"):
        binary = BUILD / f"uncaught_error_probe_{build_kind}"
        for handler in ("instrumented", "default"):
            for repeat in range(1, 6):
                stem = f"ordinary_{build_kind}_{handler}_{repeat}"
                rc, stdout, stderr = run_one(binary, handler, stem)
                ordinary_rows.append(
                    {
                        "build": build_kind,
                        "handler": handler,
                        "repeat": repeat,
                        "returncode": rc,
                        "terminate_marker": int("event=terminate" in stdout),
                        "cleanup_marker": int("event=cleanup\n" in stdout),
                        "system_error_diagnostic": int("system_error" in stderr),
                        "mutex_not_locked_diagnostic": int("mutex not locked" in stderr),
                        # backtrace_symbols() can only name exported dynamic symbols.
                        # Record the two directly observable frame shapes; raw/038
                        # resolves their offsets against the unstripped binaries.
                        "retained_noexcept_frame": int("libc++.so.1(+0x507fb)" in stderr),
                        "removed_wait_frame": int(
                            "_ZNSt3__118condition_variable4waitERNS_11unique_lockINS_5mutexEEE" in stderr
                        ),
                    }
                )

    with (OUT / "item3_uncaught_error_matrix.tsv").open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(ordinary_rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(ordinary_rows)

    print("ASSERTIONS=PASS cancel_cases=8 repeats=5 retained_terminate=20 removed_clean_cancel=20")
    for build_kind in ("retained", "removed"):
        for handler in ("instrumented", "default"):
            rows = [row for row in ordinary_rows if row["build"] == build_kind and row["handler"] == handler]
            print(
                f"ORDINARY build={build_kind} handler={handler} repeats={len(rows)} "
                f"returncodes={sorted({row['returncode'] for row in rows})} "
                f"cleanup={sum(int(row['cleanup_marker']) for row in rows)} "
                f"system_error_diagnostic={sum(int(row['system_error_diagnostic']) for row in rows)} "
                f"retained_noexcept_frame={sum(int(row['retained_noexcept_frame']) for row in rows)} "
                f"removed_wait_frame={sum(int(row['removed_wait_frame']) for row in rows)}"
            )


if __name__ == "__main__":
    main()
