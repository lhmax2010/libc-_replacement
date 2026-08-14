#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r39_s1_20260814
command="set +e; find '$remote' -maxdepth 2 -ls 2>&1; rm -f '$remote/r36_test' '$remote/r38_static_test' '$remote/r38_dynamic_test' '$remote/r38_static_export_dynamic' '$remote/libc++abi.so.1' '$remote/libc++.so.1' '$remote/libc++.so.1.0' '$remote/libc++.so' '$remote/'*.gz '$remote/'*.gz.b64; file_rc=\$?; rmdir '$remote'; root_rc=\$?; printf 'FILE_DELETE_RC=%d\\nROOT_RMDIR_RC=%d\\n' \"\$file_rc\" \"\$root_rc\"; test ! -e '$remote'; absent_rc=\$?; printf 'FILE_ABSENCE_ASSERT_RC=%d\\n' \"\$absent_rc\"; printf 'PROJECT_PROCESSES_BEGIN\\n'; for p in /proc/[0-9]*; do exe=\$(readlink \"\$p/exe\" 2>/dev/null) || continue; case \"\$exe\" in '$remote/'*) printf '%s %s\\n' \"\${p#/proc/}\" \"\$exe\";; esac; done; printf 'PROJECT_PROCESSES_END\\n'; exit \"\$absent_rc\""
printf 'SDB_COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "$command"
printf '\n'
"$sdb" -s "$serial" shell "$command"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
