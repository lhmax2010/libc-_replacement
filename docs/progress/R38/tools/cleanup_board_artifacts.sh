#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r38_s1_20260814
command="set +e; find '$remote' -maxdepth 2 -ls 2>&1; rm -f '$remote/s1_libcxx_strict' '$remote/s1_libstdcxx_strict' '$remote/patched/libc++abi.so.1' '$remote/unpatched/libc++abi.so.1'; file_rc=\$?; rmdir '$remote/patched' '$remote/unpatched'; subdir_rc=\$?; rmdir '$remote'; root_rc=\$?; printf 'FILE_DELETE_RC=%d\\nSUBDIR_RMDIR_RC=%d\\nROOT_RMDIR_RC=%d\\n' \"\$file_rc\" \"\$subdir_rc\" \"\$root_rc\"; test ! -e '$remote'; absent_rc=\$?; printf 'FILE_ABSENCE_ASSERT_RC=%d\\n' \"\$absent_rc\"; printf 'PROJECT_PROCESSES_BEGIN\\n'; ps -ef | awk 'NR == 1 || /r38_s1_20260814|s1_libcxx_strict|s1_libstdcxx_strict/ { print }'; printf 'PROJECT_PROCESSES_END\\n'; exit \"\$absent_rc\""
printf 'SDB_COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "$command"
printf '\n'
"$sdb" -s "$serial" shell "$command"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
