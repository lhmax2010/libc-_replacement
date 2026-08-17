#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r46_20260817
command="set +e; uname -a; uname -m; test ! -e '$remote'; path_rc=\$?; count=\$(ps -ef | awk '\$8 ~ /(r46_20260817|dso_launcher|stdlib_header_site_probe)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_ABSENT_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$path_rc\" \"\$count\"; ls -ld /home/owner/r44b_push_probe 2>&1; test \"\$path_rc\" -eq 0 -a \"\$count\" -eq 0"
printf 'COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "$command"
printf '\n'
"$sdb" -s "$serial" shell "$command"
rc=$?
printf 'EXIT_CODE=%d\n' "$rc"
exit "$rc"
