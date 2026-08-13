#!/usr/bin/env bash
set -uo pipefail
nproc
free -h
uptime
df -h . tmp /home/toolchain
cpu_count=$(nproc)
available_kb=$(awk '/MemAvailable:/{print $2}' /proc/meminfo)
load1=$(awk '{print $1}' /proc/loadavg)
python3 - "$cpu_count" "$available_kb" "$load1" <<'PY'
import sys
cpus = int(sys.argv[1])
available_gib = int(sys.argv[2]) / 1024 / 1024
load1 = float(sys.argv[3])
print(f"cpu_count={cpus}")
print(f"available_gib={available_gib:.3f}")
print(f"load1={load1:.3f}")
print(f"load_limit={cpus / 2:.3f}")
if available_gib < 8:
    print("RED_STOP_RESOURCE available memory below 8 GiB")
    raise SystemExit(10)
if load1 > cpus / 2:
    print("RED_STOP_RESOURCE one-minute load exceeds half CPU count")
    raise SystemExit(11)
print("RESOURCE_PRECHECK_PASS")
PY
