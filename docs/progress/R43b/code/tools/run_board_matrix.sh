#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r43b_20260815
tests=(forced_unwind1 forced_unwind2 forced_unwind3 forced_unwind4 unwind_01 unwind_02 unwind_03 unwind_04 unwind_05 unwind_06)
modes=(cout getline packaged_task shrink_to_fit control)

run_cell() {
  local section=$1 suite=$2 binary=$3 argument=$4 library_path=$5
  local remote_cmd carrier_rc
  printf '\nCELL_BEGIN section=%s arch=armv7l suite=%s test=%s\n' "$section" "$suite" "$argument"
  printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
  if [ "$section" = A ]; then
    remote_cmd="set +e; LD_LIBRARY_PATH='$library_path' '$binary'; rc=\$?; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_CELL_SIGNAL=0\\n'; fi; exit 0"
  else
    remote_cmd="set +e; LD_LIBRARY_PATH='$library_path' '$binary' '$argument'; rc=\$?; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_CELL_SIGNAL=0\\n'; fi; exit 0"
  fi
  printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$remote_cmd"; printf '\n'
  /usr/bin/timeout --preserve-status 30s "$sdb" -s "$serial" shell "$remote_cmd"
  carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\nCELL_END section=%s arch=armv7l suite=%s test=%s\n' "$carrier_rc" "$section" "$suite" "$argument"
  test "$carrier_rc" -eq 0 || return 90
}

printf 'RUNTIME_IDENTITIES_BEGIN\n'
identity="set +e; LD_LIBRARY_PATH='$remote/unpatched' ldd '$remote/libcxx/forced_unwind1'; LD_LIBRARY_PATH='$remote/patched' ldd '$remote/libcxx/forced_unwind1'; ldd '$remote/libstdcxx/forced_unwind1'; sha256sum '$remote/unpatched/libc++abi.so.1' '$remote/patched/libc++abi.so.1'; exit 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$identity"; printf '\n'
"$sdb" -s "$serial" shell "$identity" || exit 90
printf 'RUNTIME_IDENTITIES_END\n'

printf 'LD_DEBUG_IDENTITY_BEGIN\n'
debug="set +e; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/patched' '$remote/libcxx/unwind_01'; rc=\$?; printf 'LD_DEBUG_REMOTE_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$debug"; printf '\n'
"$sdb" -s "$serial" shell "$debug" || exit 90
printf 'LD_DEBUG_IDENTITY_END\n'

for test_name in "${tests[@]}"; do
  run_cell A libcxx_unpatched "$remote/libcxx/$test_name" "$test_name" "$remote/unpatched" || exit $?
  run_cell A libcxx_r42 "$remote/libcxx/$test_name" "$test_name" "$remote/patched" || exit $?
  run_cell A libstdcxx_control "$remote/libstdcxx/$test_name" "$test_name" /lib || exit $?
done
for mode in "${modes[@]}"; do
  run_cell B libcxx_unpatched "$remote/libcxx/stdlib_cancel_probe" "$mode" "$remote/unpatched" || exit $?
  run_cell B libcxx_r42 "$remote/libcxx/stdlib_cancel_probe" "$mode" "$remote/patched" || exit $?
  run_cell B libstdcxx_control "$remote/libstdcxx/stdlib_cancel_probe" "$mode" /lib || exit $?
done
exit 0
