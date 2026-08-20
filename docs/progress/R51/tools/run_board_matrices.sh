#!/usr/bin/env bash
set -u
set -o pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r51_20260820

run_cell() {
  local matrix=$1 suite=$2 test_name=$3 binary=$4 library_path=$5
  shift 5
  local args='' arg remote_cmd carrier_rc
  for arg in "$@"; do printf -v args '%s %q' "$args" "$arg"; done
  remote_cmd="set +e; LD_LIBRARY_PATH='$library_path' '$binary'$args & pid=\$!; (sleep 20; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_CELL_SIGNAL=0\\n'; fi; exit 0"
  printf '\nCELL_BEGIN matrix=%s arch=armv7l suite=%s test=%s\n' "$matrix" "$suite" "$test_name"
  printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
  printf 'SDB_COMMAND='
  printf '%q ' /usr/bin/timeout --preserve-status 30s "$sdb" -s "$serial" shell "$remote_cmd"
  printf '\n'
  /usr/bin/timeout --preserve-status 30s "$sdb" -s "$serial" shell "$remote_cmd"
  carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier_rc"
  printf 'CELL_END matrix=%s arch=armv7l suite=%s test=%s\n' "$matrix" "$suite" "$test_name"
  (( carrier_rc == 0 )) || return 90
}

printf 'RUNTIME_IDENTITIES_BEGIN\n'
identity="set +e; for suite in unpatched prestrip product; do echo LDD_SUITE=\$suite; LD_LIBRARY_PATH='$remote/runtime/'\$suite ldd '$remote/bin/libcxx/forced_unwind1'; echo LDD_EXIT_CODE=\$?; done; sha256sum '$remote/runtime/unpatched/libc++abi.so.1' '$remote/runtime/prestrip/libc++abi.so.1' '$remote/runtime/product/libc++abi.so.1'; exit 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$identity"; printf '\n'
"$sdb" -s "$serial" shell "$identity" || exit 90
printf 'LD_DEBUG_IDENTITY_BEGIN\n'
debug="set +e; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/runtime/product' '$remote/bin/libcxx/unwind_01'; rc=\$?; printf 'LD_DEBUG_REMOTE_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$debug"; printf '\n'
"$sdb" -s "$serial" shell "$debug" || exit 90
printf 'LD_DEBUG_IDENTITY_END\nRUNTIME_IDENTITIES_END\n'

upstream_tests=(forced_unwind1 forced_unwind2 forced_unwind3 forced_unwind4
                unwind_01 unwind_02 unwind_03 unwind_04 unwind_05 unwind_06)
for test_name in "${upstream_tests[@]}"; do
  run_cell upstream unpatched "$test_name" "$remote/bin/libcxx/$test_name" "$remote/runtime/unpatched" || exit $?
  run_cell upstream prestrip "$test_name" "$remote/bin/libcxx/$test_name" "$remote/runtime/prestrip" || exit $?
  run_cell upstream product "$test_name" "$remote/bin/libcxx/$test_name" "$remote/runtime/product" || exit $?
  run_cell upstream libstdcxx_control "$test_name" "$remote/bin/libstdcxx/$test_name" /lib:/usr/lib || exit $?
done

regression_modes=(cancel exit real foreign cancel-no-catch exit-no-catch
                  cancel-nested cancel-catchall-rethrow exit-catchall-rethrow)
for mode in "${regression_modes[@]}"; do
  run_cell regression prestrip "$mode" "$remote/bin/libcxx/s1_rethrow_probe" "$remote/runtime/prestrip" "$mode" || exit $?
  run_cell regression product "$mode" "$remote/bin/libcxx/s1_rethrow_probe" "$remote/runtime/product" "$mode" || exit $?
  run_cell regression libstdcxx_control "$mode" "$remote/bin/libstdcxx/s1_rethrow_probe" /lib:/usr/lib "$mode" || exit $?
done
run_cell regression unpatched cancel "$remote/bin/libcxx/unpatched_cancel_probe" "$remote/runtime/unpatched" || exit $?

exit 0
