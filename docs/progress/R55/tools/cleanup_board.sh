#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r55_20260821
command="set +e; pids=\$(ps -ef | awk '\$8 ~ /(r55_20260821|async_probe|facility_probe)/ {print \$2}'); if test -n \"\$pids\"; then kill -TERM \$pids 2>/dev/null; sleep 1; kill -KILL \$pids 2>/dev/null; fi; rm -rf '$remote'; rm -f /opt/usr/home/owner/share/tmp/r55-board-payload.tar; rm -f /var/tmp/r55-board-payload.tar; test ! -e '$remote'; path_rc=\$?; test ! -e /opt/usr/home/owner/share/tmp/r55-board-payload.tar; archive_rc=\$?; count=\$(ps -ef | awk '\$8 ~ /(r55_20260821|async_probe|facility_probe)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_ABSENT_RC=%d\\nSHARED_ARCHIVE_ABSENT_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$path_rc\" \"\$archive_rc\" \"\$count\"; test \"\$path_rc\" -eq 0 -a \"\$archive_rc\" -eq 0 -a \"\$count\" -eq 0"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'EXIT_CODE=0\n'
