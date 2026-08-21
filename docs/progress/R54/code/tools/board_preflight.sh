#!/usr/bin/env bash
set -euo pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r54_20260821
command="set +e; uname -a; uname -m; test ! -e '$remote'; path_rc=\$?; count=\$(ps -ef | awk '\$8 ~ /(r54_20260821|facility_probe|concurrency_probe|catchall_rethrow_probe)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_ABSENT_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$path_rc\" \"\$count\"; test \"\$path_rc\" -eq 0 -a \"\$count\" -eq 0"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'EXIT_CODE=0\n'
