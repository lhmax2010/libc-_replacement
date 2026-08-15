#!/bin/bash
set -u
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r42_s1_20260815
cmd="set +e; printf 'TRACE_RUNTIME_IDENTITY_BEGIN\\n'; LD_LIBRARY_PATH='$remote/trace' ldd '$remote/s1_libcxx'; sha256sum '$remote/trace/libc++abi.so.1' '$remote/s1_libcxx'; printf 'TRACE_RUNTIME_IDENTITY_END\\n'; LD_LIBRARY_PATH='$remote/trace' LD_DEBUG=libs '$remote/s1_libcxx' cancel; rc=\$?; printf 'REMOTE_TRACE_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_TRACE_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_TRACE_SIGNAL=0\\n'; fi; exit 0"
printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
/usr/bin/timeout --preserve-status 20s "$sdb" -s "$serial" shell "$cmd"
rc=$?
printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$rc"
exit 0
