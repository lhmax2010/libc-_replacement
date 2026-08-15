#!/bin/bash
set -u
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r42_s1_20260815

run_cell() {
  suite=$1
  library_dir=$2
  binary=$3
  mode=$4
  printf '\nCELL_BEGIN suite=%s mode=%s\n' "$suite" "$mode"
  if [ "$mode" = NO_ARGUMENT ]; then
    remote_cmd="set +e; LD_LIBRARY_PATH='$library_dir' '$binary'; rc=\$?; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_CELL_SIGNAL=0\\n'; fi; exit 0"
  else
    remote_cmd="set +e; LD_LIBRARY_PATH='$library_dir' '$binary' '$mode'; rc=\$?; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_CELL_SIGNAL=0\\n'; fi; exit 0"
  fi
  printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
  printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$remote_cmd"; printf '\n'
  /usr/bin/timeout --preserve-status 20s "$sdb" -s "$serial" shell "$remote_cmd"
  carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\nCELL_END suite=%s mode=%s\n' "$carrier_rc" "$suite" "$mode"
}

printf 'RUNTIME_IDENTITY_PATCHED_BEGIN\n'
identity="set +e; LD_LIBRARY_PATH='$remote/patched' ldd '$remote/s1_libcxx'; ldd_rc=\$?; sha256sum '$remote/s1_libcxx' '$remote/patched/libc++abi.so.1'; printf 'LDD_EXIT_CODE=%d\\n' \"\$ldd_rc\"; exit 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$identity"; printf '\n'
"$sdb" -s "$serial" shell "$identity"
printf 'RUNTIME_IDENTITY_PATCHED_END\n'

modes=(cancel exit real foreign cancel-no-catch exit-no-catch cancel-nested)
for mode in "${modes[@]}"; do
  run_cell libcxx_patched "$remote/patched" "$remote/s1_libcxx" "$mode"
done
for mode in "${modes[@]}"; do
  run_cell libstdcxx_control /lib "$remote/s1_libstdcxx" "$mode"
done
run_cell libcxx_unpatched "$remote/unpatched" "$remote/unpatched_libcxx" NO_ARGUMENT
exit 0
