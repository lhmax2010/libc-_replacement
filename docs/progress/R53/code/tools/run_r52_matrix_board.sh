#!/usr/bin/env bash
set -u
set -o pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r53_20260820

run_cell() {
  local mode=$1 suite=$2 library_path=$3 dso=$4 carrier_rc command
  command="set +e; LD_LIBRARY_PATH='$library_path' '$remote/supplement/r52/dso_launcher' '$dso' '$mode' & pid=\$!; (sleep 15; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\nREMOTE_CELL_SIGNAL=%d\\n' \"\$rc\" \"\$((rc > 128 ? rc - 128 : 0))\"; exit 0"
  printf '\nCELL_BEGIN matrix=r52_recheck arch=armv7l mode=%s suite=%s\n' "$mode" "$suite"
  printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\nSDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
  /usr/bin/timeout --preserve-status 25s "$sdb" -s "$serial" shell "$command"
  carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\nCELL_END matrix=r52_recheck arch=armv7l mode=%s suite=%s\n' "$carrier_rc" "$mode" "$suite"
  ((carrier_rc == 0)) || return 90
}

identity="set +e; cd '$remote/supplement'; sha256sum -c MANIFEST.sha256; LD_LIBRARY_PATH='$remote/full' ldd '$remote/supplement/r52/patched_headers.so'; echo LDD_EXIT_CODE=\$?; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/full' '$remote/supplement/r52/dso_launcher' '$remote/supplement/r52/patched_headers.so' getline; echo LD_DEBUG_REMOTE_EXIT_CODE=\$?; exit 0"
printf 'IDENTITY_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$identity"; printf '\n'
"$sdb" -s "$serial" shell "$identity" || exit 90

for mode in getline packaged_task; do
  run_cell "$mode" unpatched "$remote/unpatched" "$remote/supplement/r52/unpatched_headers.so" || exit $?
  run_cell "$mode" layer_ab_headers_unmodified "$remote/layer_ab" "$remote/supplement/r52/unpatched_headers.so" || exit $?
  run_cell "$mode" full_layer_c_rebuilt_consumer "$remote/full" "$remote/supplement/r52/patched_headers.so" || exit $?
  run_cell "$mode" full_layer_c_stale_consumer "$remote/full" "$remote/supplement/r52/unpatched_headers.so" || exit $?
  run_cell "$mode" libstdcxx_control /lib:/usr/lib "$remote/supplement/r52/libstdcxx.so" || exit $?
done
exit 0
