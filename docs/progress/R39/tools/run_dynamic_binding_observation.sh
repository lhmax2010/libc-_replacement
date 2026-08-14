#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r39_s1_20260814
binary="$remote/r38_dynamic_test"
command="set +e; LD_LIBRARY_PATH='$remote' LD_DEBUG=bindings,symbols '$binary' cancel; case_rc=\$?; printf 'REMOTE_CASE_EXIT_CODE=%d\\n' \"\$case_rc\"; if test \"\$case_rc\" -gt 128; then printf 'REMOTE_CASE_SIGNAL=%d\\n' \"\$((case_rc - 128))\"; else printf 'REMOTE_CASE_SIGNAL=0\\n'; fi; exit 0"
printf 'EXECUTION_CARRIER=BOARD_NATIVE_ARMV7L\n'
printf 'SDB_COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "$command"
printf '\n'
"$sdb" -s "$serial" shell "$command"
rc=$?
printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$rc"
exit "$rc"
