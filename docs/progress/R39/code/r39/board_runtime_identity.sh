#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r39_s1_20260814

command="set +e; for f in '$remote/r36_test' '$remote/r38_static_test'; do echo ====\$f====; LD_LIBRARY_PATH='$remote' ldd \$f; echo LDD_RC=\$?; done; echo ====LIBRARY====; readelf -d '$remote/libc++abi.so.1' | grep -E 'SONAME|NEEDED'; echo READELF_RC=\$?; exit 0"
printf 'SDB_COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "$command"
printf '\n'
"$sdb" -s "$serial" shell "$command"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
