#!/usr/bin/env bash
set -euo pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
cmd="set +e; pids=\$(ps -ef | awk '\$8 ~ /(r58_20260821|facility_|async_|concurrency_|normal_|abi_full|abi_reference)/ {print \$2}'); test -z \"\$pids\" || kill -TERM \$pids 2>/dev/null; sleep 1; test -z \"\$pids\" || kill -KILL \$pids 2>/dev/null; rm -rf /var/tmp/r58_20260821; rm -f /var/tmp/r58-board-payload.tar /opt/usr/home/owner/share/tmp/r58-board-payload.tar; test ! -e /var/tmp/r58_20260821; a=\$?; test ! -e /opt/usr/home/owner/share/tmp/r58-board-payload.tar; b=\$?; count=\$(ps -ef | awk '\$8 ~ /(r58_20260821|facility_|async_|concurrency_|normal_|abi_full|abi_reference)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_ABSENT_RC=%d\\nSHARED_ARCHIVE_ABSENT_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$a\" \"\$b\" \"\$count\"; test \"\$a\" -eq 0 -a \"\$b\" -eq 0 -a \"\$count\" -eq 0"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"

