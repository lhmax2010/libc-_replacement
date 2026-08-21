#!/usr/bin/env bash
set -euo pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
cmd="set -e; uname -a; cat /etc/tizen-release; test ! -e /var/tmp/r58_20260821; count=\$(ps -ef | awk '\$8 ~ /(r58_20260821|facility_|async_|concurrency_|normal_|abi_full|abi_reference)/ {n++} END {print n+0}'); printf 'R58_PROCESS_COUNT=%s\\n' \"\$count\"; test \"\$count\" -eq 0; df -h /var/tmp"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"

