#!/bin/bash
set -u
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r42_s1_20260815
cmd="set +e; pids=\$(ps -ef | grep -E '[r]42_s1_20260815|[s]1_libcxx|[s]1_libstdcxx|[u]npatched_libcxx' | awk '{print \$2}'); printf 'MATCHED_PIDS_BEFORE=%s\\n' \"\$pids\"; if test -n \"\$pids\"; then kill \$pids; sleep 1; fi; rm -rf '$remote'; rm_rc=\$?; printf 'REMOVE_EXIT_CODE=%d\\n' \"\$rm_rc\"; if test -e '$remote'; then printf 'FILE_CLEANUP=FAIL\\n'; else printf 'FILE_CLEANUP=PASS\\n'; fi; ps -ef | grep -E '[r]42_s1_20260815|[s]1_libcxx|[s]1_libstdcxx|[u]npatched_libcxx' || true; exit 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
