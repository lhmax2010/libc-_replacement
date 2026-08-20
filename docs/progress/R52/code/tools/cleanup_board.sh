#!/usr/bin/env bash
set -u
set -o pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r52_20260820
shared=/opt/usr/home/owner/share/tmp/r52_board_payload.tar
command="set +e; pids=\$(ps -ef | awk '\$8 ~ /(r52_20260820|dso_launcher|header_batch_probe)/ {print \$2}'); if test -n \"\$pids\"; then kill \$pids 2>/dev/null; sleep 1; kill -9 \$pids 2>/dev/null; fi; rm -rf '$remote'; rm -f '$shared'; test ! -e '$remote'; file_rc=\$?; test ! -e '$shared'; shared_rc=\$?; count=\$(ps -ef | awk '\$8 ~ /(r52_20260820|dso_launcher|header_batch_probe)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_REMOVED_RC=%d\\nSHARED_ARCHIVE_REMOVED_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$file_rc\" \"\$shared_rc\" \"\$count\"; test \"\$file_rc\" -eq 0 -a \"\$shared_rc\" -eq 0 -a \"\$count\" -eq 0"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
rc=$?
printf 'EXIT_CODE=%d\n' "$rc"
exit "$rc"
