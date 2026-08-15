#!/bin/bash
set -u
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r41_s1_20260815
cmd="set +e; printf 'RUNTIME_IDENTITY_BEGIN\\n'; LD_LIBRARY_PATH='$remote' ldd '$remote/s1_libcxx'; ldd_rc=\$?; sha256sum '$remote/s1_libcxx' '$remote/libc++abi.so.1'; printf 'LDD_EXIT_CODE=%d\\nRUNTIME_IDENTITY_END\\n' \"\$ldd_rc\"; LD_LIBRARY_PATH='$remote' LD_DEBUG=libs '$remote/s1_libcxx' cancel; case_rc=\$?; printf 'REMOTE_CASE_EXIT_CODE=%d\\n' \"\$case_rc\"; if test \"\$case_rc\" -gt 128; then printf 'REMOTE_CASE_SIGNAL=%d\\n' \"\$((case_rc-128))\"; else printf 'REMOTE_CASE_SIGNAL=0\\n'; fi; exit 0"
printf 'EXECUTION_CARRIER=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"

