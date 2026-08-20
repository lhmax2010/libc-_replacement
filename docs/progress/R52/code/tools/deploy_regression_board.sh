#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive=/home/toolchain/development/libc++_replacement/tmp/R52/r52_regression_board_payload.tar
shared=/opt/usr/home/owner/share/tmp/r52_regression_board_payload.tar
remote=/var/tmp/r52_regression_20260820

preflight="set +e; test ! -e '$remote'; path_rc=\$?; count=\$(ps -ef | awk '\$8 ~ /(r52_regression_20260820|s1_rethrow_probe)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_ABSENT_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$path_rc\" \"\$count\"; test \"\$path_rc\" -eq 0 -a \"\$count\" -eq 0"
printf 'PREFLIGHT_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$preflight"; printf '\n'
"$sdb" -s "$serial" shell "$preflight"

printf 'PUSH_COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
printf 'PUSH_EXIT_CODE=%d\n' "$?"

deploy="set -e; mkdir '$remote'; tar -C '$remote' -xf '$shared'; chmod 0755 '$remote/bin/libcxx' '$remote/bin/libstdcxx'; cd '$remote'; sha256sum -c MANIFEST.sha256"
printf 'DEPLOY_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$deploy"; printf '\n'
"$sdb" -s "$serial" shell "$deploy"
printf 'DEPLOY_EXIT_CODE=%d\n' "$?"
