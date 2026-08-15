#!/usr/bin/env bash
set -u
set -o pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r44b_20260815
mode=${1:-cout}

run_cell() {
  local suite=$1 library_path=$2 binary=$3
  local remote_command carrier_rc
  remote_command="set +e; LD_LIBRARY_PATH='$library_path' '$binary' '$mode' & pid=\$!; (sleep 20; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_CELL_SIGNAL=0\\n'; fi; exit 0"
  printf '\nCELL_BEGIN arch=armv7l suite=%s mode=%s\n' "$suite" "$mode"
  printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
  printf 'SDB_COMMAND='
  printf '%q ' /usr/bin/timeout --preserve-status 30s "$sdb" -s "$serial" shell "$remote_command"
  printf '\n'
  /usr/bin/timeout --preserve-status 30s "$sdb" -s "$serial" shell "$remote_command"
  carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier_rc"
  printf 'CELL_END arch=armv7l suite=%s mode=%s\n' "$suite" "$mode"
  (( carrier_rc == 0 )) || return 90
}

run_cell unpatched "$remote/unpatched" "$remote/bin/libcxx_probe" || exit $?
run_cell layer1 "$remote/layer1" "$remote/bin/libcxx_probe" || exit $?
run_cell aggregation "$remote/aggregation" "$remote/bin/libcxx_probe" || exit $?
run_cell libstdcxx_control /lib:/usr/lib "$remote/bin/libstdcxx_probe" || exit $?
exit 0
