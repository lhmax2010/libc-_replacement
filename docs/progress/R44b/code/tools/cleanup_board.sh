#!/usr/bin/env bash
set -u
set -o pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r44b_20260815

printf 'SDB_COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "set +e; pids=\$(ps -ef | grep '$remote' | grep -v grep | awk '{print \$2}'); if test -n \"\$pids\"; then kill \$pids; sleep 1; kill -9 \$pids 2>/dev/null; fi; rm -rf '$remote'; test ! -e '$remote'; file_rc=\$?; ps -ef | grep '$remote' | grep -v grep; process_rc=\$?; echo FILE_REMOVAL_CHECK_RC=\$file_rc; echo PROCESS_ABSENCE_GREP_RC=\$process_rc; exit \$file_rc"
printf '\n'
"$sdb" -s "$serial" shell "set +e; pids=\$(ps -ef | grep '$remote' | grep -v grep | awk '{print \$2}'); if test -n \"\$pids\"; then kill \$pids; sleep 1; kill -9 \$pids 2>/dev/null; fi; rm -rf '$remote'; test ! -e '$remote'; file_rc=\$?; ps -ef | grep '$remote' | grep -v grep; process_rc=\$?; echo FILE_REMOVAL_CHECK_RC=\$file_rc; echo PROCESS_ABSENCE_GREP_RC=\$process_rc; exit \$file_rc"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
