#!/bin/bash
set -u
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r42_s1_20260815
cmd="set +e; all_pids=''; for name in s1_libcxx s1_libstdcxx unpatched_libcxx; do pids=\$(pidof \"\$name\" 2>/dev/null); printf 'PIDOF_BEFORE %s=%s\\n' \"\$name\" \"\$pids\"; all_pids=\"\$all_pids \$pids\"; done; if test -n \"\$(printf '%s' \"\$all_pids\" | tr -d ' ')\"; then kill \$all_pids; sleep 1; fi; rm -rf '$remote'; rm_rc=\$?; printf 'REMOVE_EXIT_CODE=%d\\n' \"\$rm_rc\"; if test -e '$remote'; then printf 'FILE_CLEANUP=FAIL\\n'; else printf 'FILE_CLEANUP=PASS\\n'; fi; for name in s1_libcxx s1_libstdcxx unpatched_libcxx; do pids=\$(pidof \"\$name\" 2>/dev/null); printf 'PIDOF_AFTER %s=%s\\n' \"\$name\" \"\$pids\"; done; exit 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
