#!/usr/bin/env bash
set -u
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r53_20260820
shared=/opt/usr/home/owner/share/tmp/r53_board_payload.tar
executables=/opt/usr/home/owner/share/tmp/r53_executables.tar
supplement=/opt/usr/home/owner/share/tmp/r53_board_supplement.tar
command="set +e; pids=\$(ps -ef | awk '\$8 ~ /(r53_20260820|dso_launcher|r53_run)/ {print \$2}'); test -z \"\$pids\" || kill \$pids 2>/dev/null; sleep 1; pids=\$(ps -ef | awk '\$8 ~ /(r53_20260820|dso_launcher|r53_run)/ {print \$2}'); test -z \"\$pids\" || kill -9 \$pids 2>/dev/null; rm -rf '$remote'; rm -f '$shared' '$executables' '$supplement'; test ! -e '$remote'; a=\$?; test ! -e '$shared'; b=\$?; test ! -e '$executables'; c=\$?; test ! -e '$supplement'; d=\$?; count=\$(ps -ef | awk '\$8 ~ /(r53_20260820|dso_launcher|r53_run)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_REMOVED_RC=%d\\nSHARED_ARCHIVE_REMOVED_RC=%d\\nEXECUTABLES_ARCHIVE_REMOVED_RC=%d\\nSUPPLEMENT_ARCHIVE_REMOVED_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$a\" \"\$b\" \"\$c\" \"\$d\" \"\$count\"; test \"\$a\" -eq 0 -a \"\$b\" -eq 0 -a \"\$c\" -eq 0 -a \"\$d\" -eq 0 -a \"\$count\" -eq 0"
"$sdb" -s "$serial" shell "$command"; rc=$?; printf 'EXIT_CODE=%d\n' "$rc"; exit "$rc"
