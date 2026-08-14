#!/bin/bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
script=/home/toolchain/development/libc++_replacement/progress/R36/tools/run_board_matrix.sh
encoded=$(gzip -c "$script" | base64 -w0)
printf 'EXECUTION_CARRIER=BOARD_NATIVE_ARMV7L\n'
printf 'QEMU_USED_FOR_TEST_EXECUTION=NO\n'
printf 'REMOTE_SCRIPT=%s\n' "$script"
printf 'REMOTE_SCRIPT_SHA256='
sha256sum "$script" | awk '{print $1}'
printf 'SDB_COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "printf '%s' '$encoded' | base64 -d | gzip -dc | sh"
printf '\n'
"$sdb" -s "$serial" shell "printf '%s' '$encoded' | base64 -d | gzip -dc | sh"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
