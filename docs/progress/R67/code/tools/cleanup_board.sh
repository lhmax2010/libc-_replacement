#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r67_20260825
shared=/opt/usr/home/owner/share/tmp/r67-board-payload.tar
cmd="set +e; pids=\$(ps -ef | awk '\$8 ~ /(r67_20260825|r61_fixed|m2_lock_|m1_concurrency_|facility_|async_|normal_|regression_|concurrency_|forced_unwind|unwind_0)/ {print \$2}'); test -z \"\$pids\" || kill -TERM \$pids 2>/dev/null; sleep 1; test -z \"\$pids\" || kill -KILL \$pids 2>/dev/null; rm -rf '$remote'; rm -f /var/tmp/r67-board-payload.tar '$shared'; test ! -e '$remote'; a=\$?; test ! -e '$shared'; b=\$?; count=\$(ps -ef | awk '\$8 ~ /(r67_20260825|r61_fixed|m2_lock_|m1_concurrency_|facility_|async_|normal_|regression_|concurrency_|forced_unwind|unwind_0)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_ABSENT_RC=%d\\nSHARED_ARCHIVE_ABSENT_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$a\" \"\$b\" \"\$count\"; test \"\$a\" -eq 0 -a \"\$b\" -eq 0 -a \"\$count\" -eq 0"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"
