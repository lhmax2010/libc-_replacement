#!/usr/bin/env python3
import csv
import os
import resource
import shlex
import signal
import subprocess
import sys
from pathlib import Path

WORKSPACE = Path("/home/toolchain/development/libc++_replacement")
TMP = WORKSPACE / "tmp/R14b"
PROGRESS = WORKSPACE / "progress/R14b/L2"
AUDIT = TMP / "L2/bin/r14b_audit.so"


def shell_exit(returncode: int) -> int:
    return 128 + (-returncode) if returncode < 0 else returncode


def parse_check(stdout: str):
    values = {
        "caught_origin": "NOT_OBSERVED",
        "expected_origin": "NOT_OBSERVED",
        "type_check": "NOT_OBSERVED",
        "cleanup_count_check": "NOT_OBSERVED",
        "order_check": "NOT_OBSERVED",
        "event_count": "NOT_OBSERVED",
    }
    for line in stdout.splitlines():
        fields = line.split("\t")
        if fields and fields[0] == "RESULT":
            for field in fields[1:]:
                key, value = field.split("=", 1)
                values[key] = value
        elif fields and fields[0] == "CHECK":
            mapping = {
                "type": "type_check",
                "cleanup_count": "cleanup_count_check",
                "order": "order_check",
                "event_count": "event_count",
            }
            for field in fields[1:]:
                key, value = field.split("=", 1)
                values[mapping[key]] = value
    return values


def cells_for(stage: str):
    if stage == "same":
        directions = ("new_new", "legacy_legacy")
    elif stage == "cross":
        directions = ("new_legacy", "legacy_new")
    else:
        raise ValueError(stage)
    for scheme in ("B", "A"):
        for order in ("legacy_new", "new_legacy"):
            for binding in ("lazy", "now"):
                for direction in directions:
                    yield scheme, "dt_needed", order, "na", binding, direction
        for order in ("legacy_new", "new_legacy"):
            for scope in ("global", "local"):
                for binding in ("lazy", "now"):
                    for direction in directions:
                        yield scheme, "dlopen", order, scope, binding, direction


def main() -> int:
    if len(sys.argv) != 2 or sys.argv[1] not in {"same", "cross"}:
        print(f"usage: {sys.argv[0]} same|cross", file=sys.stderr)
        return 64
    stage = sys.argv[1]
    raw_root = PROGRESS / f"raw/matrix_{stage}"
    command_root = PROGRESS / f"commands/matrix_{stage}"
    table = PROGRESS / f"tables/matrix_{stage}.tsv"
    raw_root.mkdir(parents=True, exist_ok=True)
    command_root.mkdir(parents=True, exist_ok=True)
    table.parent.mkdir(parents=True, exist_ok=True)
    core_pattern = Path("/proc/sys/kernel/core_pattern").read_text().strip()
    core_soft, core_hard = resource.getrlimit(resource.RLIMIT_CORE)
    rows = []

    for number, (scheme, mode, order, scope, binding, direction) in enumerate(
        cells_for(stage), start=1
    ):
        case_id = (
            f"{stage[0].upper()}{number:03d}_{scheme}_{mode}_{order}_"
            f"{scope}_{binding}_{direction}"
        )
        case_raw = raw_root / case_id
        case_cwd = TMP / f"L2/cells/{case_id}"
        case_raw.mkdir(parents=True, exist_ok=False)
        case_cwd.mkdir(parents=True, exist_ok=False)
        runtime = TMP / f"runtime/L2{scheme}"
        if mode == "dlopen":
            executable = TMP / "L2/bin/main_dlopen"
        else:
            executable = TMP / f"L2/bin/main_dt_{scheme}_{order}"
        argv = [str(executable), scheme, mode, order, scope, direction]
        env = os.environ.copy()
        env.pop("LD_BIND_NOW", None)
        env.update(
            {
                "LC_ALL": "C",
                "LD_LIBRARY_PATH": str(runtime),
                "LD_AUDIT": str(AUDIT),
                "LD_DEBUG": "bindings,versions",
                "LD_DEBUG_OUTPUT": str(case_raw / "ld_debug"),
            }
        )
        if binding == "now":
            env["LD_BIND_NOW"] = "1"
        environment_words = [
            f"LC_ALL={env['LC_ALL']}",
            f"LD_LIBRARY_PATH={env['LD_LIBRARY_PATH']}",
            f"LD_AUDIT={env['LD_AUDIT']}",
            f"LD_DEBUG={env['LD_DEBUG']}",
            f"LD_DEBUG_OUTPUT={env['LD_DEBUG_OUTPUT']}",
        ]
        if binding == "now":
            environment_words.append("LD_BIND_NOW=1")
        command_text = "env " + " ".join(
            shlex.quote(word) for word in environment_words
        ) + " " + shlex.join(argv)
        before_cores = {path.name for path in case_cwd.glob("core*")}
        timed_out = False
        try:
            completed = subprocess.run(
                argv,
                cwd=case_cwd,
                env=env,
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                timeout=30,
                check=False,
            )
            actual_return = completed.returncode
            stdout = completed.stdout
            stderr = completed.stderr
        except subprocess.TimeoutExpired as error:
            timed_out = True
            actual_return = -signal.SIGKILL
            stdout = error.stdout or ""
            stderr = error.stderr or ""
            if isinstance(stdout, bytes):
                stdout = stdout.decode(errors="replace")
            if isinstance(stderr, bytes):
                stderr = stderr.decode(errors="replace")
        (case_raw / "program.stdout").write_text(stdout)
        (case_raw / "program.stderr").write_text(stderr)
        debug_files = sorted(case_raw.glob("ld_debug.*"))
        after_cores = {path.name for path in case_cwd.glob("core*")}
        new_cores = sorted(after_cores - before_cores)
        signal_number = -actual_return if actual_return < 0 else 0
        signal_name = signal.Signals(signal_number).name if signal_number else "NONE"
        exit_code = shell_exit(actual_return)
        if timed_out:
            outcome = "TIMEOUT_SIGNAL_KILL"
        elif signal_number:
            outcome = f"SIGNAL_{signal_name}"
        elif exit_code == 0:
            outcome = "NORMAL_EXIT_0"
        else:
            outcome = f"NORMAL_EXIT_{exit_code}"
        if new_cores:
            core_status = "CORE_FILE_PRESENT:" + ",".join(new_cores)
        elif core_soft == 0:
            core_status = "NO_CORE_RLIMIT_ZERO"
        else:
            core_status = "NO_CORE_FILE_OBSERVED"
        check = parse_check(stdout)
        audit_calls = [
            line for line in stderr.splitlines() if line.startswith("AUDIT_CALL\t")
        ]
        command_log = command_root / f"{case_id}.log"
        command_log.write_text(
            f"PWD={shlex.quote(str(case_cwd))}\n"
            f"COMMAND={command_text}\n"
            f"STDOUT={case_raw / 'program.stdout'}\n"
            f"STDERR={case_raw / 'program.stderr'}\n"
            f"LD_DEBUG_FILES={','.join(str(path) for path in debug_files) or 'NOT_OBSERVED'}\n"
            f"ACTUAL_RETURN={actual_return}\nEXIT_CODE={exit_code}\n"
            f"SIGNAL_NUMBER={signal_number}\nSIGNAL_NAME={signal_name}\n"
            f"CORE_PATTERN={core_pattern}\nCORE_RLIMIT_SOFT={core_soft}\n"
            f"CORE_RLIMIT_HARD={core_hard}\nCORE_STATUS={core_status}\n"
        )
        rows.append(
            {
                "case_id": case_id,
                "scheme": scheme,
                "mode": mode,
                "order": order,
                "scope": scope,
                "binding": binding,
                "direction": direction,
                "outcome": outcome,
                "actual_return": actual_return,
                "exit_code": exit_code,
                "signal_number": signal_number,
                "signal_name": signal_name,
                "core_status": core_status,
                "caught_origin": check["caught_origin"],
                "expected_origin": check["expected_origin"],
                "type_check": check["type_check"],
                "cleanup_count_check": check["cleanup_count_check"],
                "order_check": check["order_check"],
                "event_count": check["event_count"],
                "audit_call_count": len(audit_calls),
                "audit_calls": " || ".join(audit_calls) or "NOT_OBSERVED",
                "ld_debug_file_count": len(debug_files),
                "command_log": str(command_log.relative_to(WORKSPACE)),
                "raw_directory": str(case_raw.relative_to(WORKSPACE)),
            }
        )

    with table.open("w", newline="") as stream:
        writer = csv.DictWriter(
            stream, fieldnames=list(rows[0]), delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(rows)
    print(f"STAGE={stage}")
    print(f"EXECUTED_CELLS={len(rows)}")
    print("FULL_CARTESIAN_CELLS=128")
    print("MEANINGFUL_CELLS=96")
    print("STAGE_CELLS=48")
    print("REDUCTION=DT_NEEDED_SCOPE_COLLAPSED_TO_NA")
    print(f"TABLE={table}")
    if len(rows) != 48 or any(row["ld_debug_file_count"] != 1 for row in rows):
        return 2
    if stage == "same":
        failed = [
            row
            for row in rows
            if row["outcome"] != "NORMAL_EXIT_0"
            or row["type_check"] != "PASS"
            or row["cleanup_count_check"] != "PASS"
            or row["order_check"] != "PASS"
            or row["event_count"] != "5"
        ]
        print(f"SAME_SIDE_GATE_FAILED={len(failed)}")
        if failed:
            for row in failed:
                print(f"FAILED_CASE={row['case_id']} outcome={row['outcome']}")
            return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
