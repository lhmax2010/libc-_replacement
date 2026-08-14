#!/bin/bash
set -u

if [ "$#" -ne 3 ]; then
  printf 'usage: %s BINARY LIBCXXABI CASE\n' "$0" >&2
  exit 2
fi
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r40_s1_20260814
binary="$remote/$1"
abi="$remote/$2"
case_name=$3
command="set +e; ln -sfn '$abi' '$remote/libc++abi.so.1'; printf 'RUNTIME_IDENTITY_BEGIN\\n'; LD_LIBRARY_PATH='$remote' ldd '$binary'; ldd_rc=\$?; sha256sum '$binary' '$abi'; printf 'LDD_EXIT_CODE=%d\\nRUNTIME_IDENTITY_END\\n' \"\$ldd_rc\"; LD_LIBRARY_PATH='$remote' LD_DEBUG=libs '$binary' '$case_name'; case_rc=\$?; printf 'REMOTE_CASE_EXIT_CODE=%d\\n' \"\$case_rc\"; if test \"\$case_rc\" -gt 128; then printf 'REMOTE_CASE_SIGNAL=%d\\n' \"\$((case_rc-128))\"; else printf 'REMOTE_CASE_SIGNAL=0\\n'; fi; exit 0"
printf 'EXECUTION_CARRIER=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
rc=$?
printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$rc"
exit "$rc"
