#!/usr/bin/env bash
set -u
set -o pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r52_regression_20260820

run_cell() {
  local suite=$1 mode=$2 binary=$3 libpath=$4 carrier_rc remote_cmd
  remote_cmd="set +e; LD_LIBRARY_PATH='$libpath' '$binary' '$mode' & pid=\$!; (sleep 20; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\nREMOTE_CELL_SIGNAL=%d\\n' \"\$rc\" \"\$((rc > 128 ? rc - 128 : 0))\"; exit 0"
  printf '\nCELL_BEGIN matrix=r51_regression arch=armv7l suite=%s test=%s\n' "$suite" "$mode"
  printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\nSDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$remote_cmd"; printf '\n'
  /usr/bin/timeout --preserve-status 30s "$sdb" -s "$serial" shell "$remote_cmd"
  carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\nCELL_END matrix=r51_regression arch=armv7l suite=%s test=%s\n' "$carrier_rc" "$suite" "$mode"
  ((carrier_rc == 0)) || return 90
}

identity="set +e; sha256sum '$remote/bin/libcxx' '$remote/bin/libstdcxx' '$remote/runtime/libc++abi.so.1'; LD_LIBRARY_PATH='$remote/runtime' ldd '$remote/bin/libcxx'; echo LDD_EXIT_CODE=\$?; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/runtime' '$remote/bin/libcxx' real; echo LD_DEBUG_REMOTE_EXIT_CODE=\$?; exit 0"
printf 'IDENTITY_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$identity"; printf '\n'
"$sdb" -s "$serial" shell "$identity" || exit 90

modes=(cancel exit real foreign cancel-no-catch exit-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow)
for mode in "${modes[@]}"; do
  run_cell product_header_batch "$mode" "$remote/bin/libcxx" "$remote/runtime" || exit $?
  run_cell libstdcxx_control "$mode" "$remote/bin/libstdcxx" /lib:/usr/lib || exit $?
done
exit 0
