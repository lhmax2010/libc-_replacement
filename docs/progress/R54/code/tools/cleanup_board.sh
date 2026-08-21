#!/usr/bin/env bash
set -euo pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r54_20260821
shared=/opt/usr/home/owner/share/tmp/r54_board_payload.tar
supplement=/opt/usr/home/owner/share/tmp/r54_noncancel_supplement.tar
command="set +e; active=\$(ps -ef | awk '\$8 ~ /(r54_20260821|facility_probe|concurrency_probe|catchall_rethrow_probe)/ {print \$2}'); if test -n \"\$active\"; then kill -TERM \$active; sleep 1; fi; rm -rf '$remote'; rm -f '$shared' '$supplement'; test ! -e '$remote'; path_rc=\$?; test ! -e '$shared'; shared_rc=\$?; test ! -e '$supplement'; supplement_rc=\$?; count=\$(ps -ef | awk '\$8 ~ /(r54_20260821|facility_probe|concurrency_probe|catchall_rethrow_probe)/ {n++} END {print n+0}'); printf 'REMOTE_ABSENT_RC=%d\\nSHARED_ABSENT_RC=%d\\nSUPPLEMENT_ABSENT_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$path_rc\" \"\$shared_rc\" \"\$supplement_rc\" \"\$count\"; test \"\$path_rc\" -eq 0 -a \"\$shared_rc\" -eq 0 -a \"\$supplement_rc\" -eq 0 -a \"\$count\" -eq 0"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'EXIT_CODE=0\n'
