#!/usr/bin/env python3
import csv
import re
import shlex
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SERIAL = "192.168.108.25:26101"
PROBE = ROOT / "progress/R22/board/exec_probe_armv7l"
OUT = ROOT / "progress/R22/board"
TOKEN = "r22_exec_probe_20260807"
CANDIDATES = [
    "/root",
    "/opt/usr",
    "/opt/usr/home/owner/share/tmp",
    "/opt/home",
    "/home/owner",
    "/var/tmp",
    "/opt/var/tmp",
]

records = []

def run(kind, candidate, argv):
    p = subprocess.run(argv, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    records.append({
        "kind": kind,
        "candidate": candidate,
        "command": shlex.join(argv),
        "carrier_exit_code": p.returncode,
        "remote_exit_code": "NOT_APPLICABLE",
        "output": p.stdout,
    })
    return p

def shell(kind, candidate, command):
    p = run(kind, candidate, ["sdb", "-s", SERIAL, "shell", command + "; rc=$?; echo __R22_REMOTE_RC=$rc"])
    matches = re.findall(r"__R22_REMOTE_RC=([0-9]+)", p.stdout)
    remote_rc = int(matches[-1]) if matches else None
    records[-1]["remote_exit_code"] = "NOT_OBSERVED" if remote_rc is None else remote_rc
    return p, remote_rc

results = []
for candidate in CANDIDATES:
    remote_dir = candidate.rstrip("/") + "/" + TOKEN
    if not remote_dir.startswith(candidate.rstrip("/") + "/r22_exec_probe_"):
        raise RuntimeError("unsafe remote temporary path")
    pre, pre_remote_rc = shell(
        "precheck_mkdir",
        candidate,
        f"if test -d {shlex.quote(candidate)}; then ls -ldZ {shlex.quote(candidate)} 2>&1; mkdir {shlex.quote(remote_dir)}; else echo CANDIDATE_NOT_FOUND; false; fi",
    )
    push_rc = exec_rc = None
    if pre.returncode == 0 and pre_remote_rc == 0:
        remote_probe = remote_dir + "/exec_probe_armv7l"
        pushed = run("push", candidate, ["sdb", "-s", SERIAL, "push", str(PROBE), remote_probe])
        push_rc = pushed.returncode
        if pushed.returncode == 0:
            executed, exec_rc = shell(
                "chmod_execute",
                candidate,
                f"chmod 700 {shlex.quote(remote_probe)} && ls -lZ {shlex.quote(remote_probe)} 2>&1 && {shlex.quote(remote_probe)}",
            )
    cleaned, cleaned_remote_rc = shell("cleanup", candidate, f"rm -rf {shlex.quote(remote_dir)}")
    verified, verified_remote_rc = shell("cleanup_verify", candidate, f"test ! -e {shlex.quote(remote_dir)}")
    if pre.returncode != 0 or pre_remote_rc != 0:
        status = "NOT_WRITABLE_OR_NOT_FOUND"
    elif push_rc != 0:
        status = "WRITE_TRANSFER_FAILED"
    elif exec_rc == 0:
        status = "WRITABLE_AND_EXECUTABLE_CONFIRMED"
    else:
        status = "WRITABLE_EXECUTION_FAILED"
    results.append({
        "candidate": candidate,
        "mkdir_carrier_exit_code": pre.returncode,
        "mkdir_remote_exit_code": "NOT_OBSERVED" if pre_remote_rc is None else pre_remote_rc,
        "push_exit_code": "NOT_RUN" if push_rc is None else push_rc,
        "execute_exit_code": "NOT_RUN" if exec_rc is None else exec_rc,
        "cleanup_carrier_exit_code": cleaned.returncode,
        "cleanup_remote_exit_code": "NOT_OBSERVED" if cleaned_remote_rc is None else cleaned_remote_rc,
        "deletion_verify_carrier_exit_code": verified.returncode,
        "deletion_verify_remote_exit_code": "NOT_OBSERVED" if verified_remote_rc is None else verified_remote_rc,
        "status": status,
    })

with (OUT / "path_probe_commands_v2.tsv").open("w", encoding="utf-8", newline="") as f:
    w = csv.DictWriter(f, fieldnames=["kind", "candidate", "command", "carrier_exit_code", "remote_exit_code", "output"], delimiter="\t", lineterminator="\n")
    w.writeheader(); w.writerows(records)
with (OUT / "path_probe_results_v2.tsv").open("w", encoding="utf-8", newline="") as f:
    w = csv.DictWriter(f, fieldnames=list(results[0]), delimiter="\t", lineterminator="\n")
    w.writeheader(); w.writerows(results)

print("candidate_count", len(CANDIDATES))
for row in results:
    print("\t".join(str(row[k]) for k in row))
if any(r["cleanup_remote_exit_code"] != 0 or r["deletion_verify_remote_exit_code"] != 0 for r in results):
    raise SystemExit(2)
