#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r43b_20260815
names='forced_unwind1 forced_unwind2 forced_unwind3 forced_unwind4 unwind_01 unwind_02 unwind_03 unwind_04 unwind_05 unwind_06 stdlib_cancel_probe'
cmd="set +e; all_pids=''; for name in $names; do pids=\$(pidof \"\$name\" 2>/dev/null); printf 'PIDOF_BEFORE %s=%s\\n' \"\$name\" \"\$pids\"; all_pids=\"\$all_pids \$pids\"; done; if test -n \"\$(printf '%s' \"\$all_pids\" | tr -d ' ')\"; then kill \$all_pids; sleep 1; fi; rm -rf '$remote'; rm_rc=\$?; printf 'REMOVE_EXIT_CODE=%d\\n' \"\$rm_rc\"; if test -e '$remote'; then printf 'FILE_CLEANUP=FAIL\\n'; else printf 'FILE_CLEANUP=PASS\\n'; fi; for name in $names; do pids=\$(pidof \"\$name\" 2>/dev/null); printf 'PIDOF_AFTER %s=%s\\n' \"\$name\" \"\$pids\"; done; exit 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
