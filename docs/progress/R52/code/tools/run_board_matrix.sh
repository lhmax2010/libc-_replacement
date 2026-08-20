#!/usr/bin/env bash
set -u
set -o pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r52_20260820

run_cell() {
  local mode=$1 suite=$2 library_path=$3 dso=$4
  command="set +e; LD_LIBRARY_PATH='$library_path' '$remote/bin/dso_launcher' '$dso' '$mode' & pid=\$!; (sleep 15; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_CELL_SIGNAL=0\\n'; fi; exit 0"
  printf '\nCELL_BEGIN arch=armv7l mode=%s suite=%s\n' "$mode" "$suite"
  printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
  printf 'SDB_COMMAND='; printf '%q ' /usr/bin/timeout --preserve-status 25s "$sdb" -s "$serial" shell "$command"; printf '\n'
  /usr/bin/timeout --preserve-status 25s "$sdb" -s "$serial" shell "$command"
  carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier_rc"
  printf 'CELL_END arch=armv7l mode=%s suite=%s\n' "$mode" "$suite"
  (( carrier_rc == 0 )) || return 90
}

printf 'IDENTITY_BEGIN\n'
identity="set +e; cd '$remote'; sha256sum -c MANIFEST.sha256; for suite in unpatched layer_ab header_batch; do echo LDD_SUITE=\$suite; LD_LIBRARY_PATH='$remote/'\$suite ldd '$remote/dso/patched_headers.so'; echo LDD_EXIT_CODE=\$?; done; echo LDD_SUITE=libstdcxx_control; LD_LIBRARY_PATH=/lib:/usr/lib ldd '$remote/dso/libstdcxx.so'; echo LDD_EXIT_CODE=\$?; exit 0"
"$sdb" -s "$serial" shell "$identity" || exit 90
printf 'LD_DEBUG_SUITE=header_fix mode=getline\n'
debug="set +e; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/header_batch' '$remote/bin/dso_launcher' '$remote/dso/patched_headers.so' getline; rc=\$?; echo LD_DEBUG_REMOTE_EXIT_CODE=\$rc; exit 0"
"$sdb" -s "$serial" shell "$debug" || exit 90
printf 'IDENTITY_END\n'

for mode in getline packaged_task; do
  run_cell "$mode" unpatched "$remote/unpatched" "$remote/dso/unpatched_headers.so" || exit $?
  run_cell "$mode" layer_ab_headers_unmodified "$remote/layer_ab" "$remote/dso/unpatched_headers.so" || exit $?
  run_cell "$mode" header_fix_rebuilt_consumer "$remote/header_batch" "$remote/dso/patched_headers.so" || exit $?
  run_cell "$mode" rebuilt_libcxx_stale_consumer "$remote/header_batch" "$remote/dso/unpatched_headers.so" || exit $?
  run_cell "$mode" libstdcxx_control /lib:/usr/lib "$remote/dso/libstdcxx.so" || exit $?
done

exit 0
