#!/usr/bin/env bash
# 单格异常不终止矩阵；SDB 载体失败才停止。
set -u
set -o pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r50_20260820

run_cell() {
  local mode=$1 suite=$2 library_path=$3 dso=$4 debug=${5:-0}
  local command carrier_rc
  printf '\nCELL_BEGIN arch=armv7l mode=%s suite=%s\n' "$mode" "$suite"
  printf 'EXECUTION_MODE=BOARD_NATIVE_ARMV7L_NO_QEMU\n'
  if [[ $debug == 1 ]]; then
    echo 'KEY_LIBRARY_SHA256_BEGIN'
    "$sdb" -s "$serial" shell "sha256sum '$remote/key/libc++.so.1' '$remote/key/libc++abi.so.1'"
    echo 'KEY_LIBRARY_SHA256_END'
    echo 'LD_DEBUG_BEGIN'
    "$sdb" -s "$serial" shell "set +e; LD_DEBUG=libs LD_LIBRARY_PATH='$library_path' '$remote/bin/dso_launcher' '$dso' '$mode'; rc=\$?; echo LD_DEBUG_REMOTE_EXIT_CODE=\$rc; exit 0"
    echo 'LD_DEBUG_END'
  fi
  command="set +e; LD_LIBRARY_PATH='$library_path' '$remote/bin/dso_launcher' '$dso' '$mode' & pid=\$!; (sleep 15; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else printf 'REMOTE_CELL_SIGNAL=0\\n'; fi; exit 0"
  printf 'SDB_COMMAND='; printf '%q ' /usr/bin/timeout --preserve-status 25s "$sdb" -s "$serial" shell "$command"; printf '\n'
  /usr/bin/timeout --preserve-status 25s "$sdb" -s "$serial" shell "$command"
  carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier_rc"
  printf 'CELL_END arch=armv7l mode=%s suite=%s\n' "$mode" "$suite"
  (( carrier_rc == 0 )) || return 90
}

echo 'IDENTITY_BEGIN'
"$sdb" -s "$serial" shell "set +e; cd '$remote'; sha256sum -c MANIFEST.sha256; for suite in unpatched layer_ab key; do echo LDD_SUITE=\$suite; LD_LIBRARY_PATH='$remote/'\$suite ldd '$remote/dso/unpatched.so'; echo LDD_EXIT_CODE=\$?; done; echo LDD_SUITE=libstdcxx; LD_LIBRARY_PATH=/lib:/usr/lib ldd '$remote/dso/libstdcxx.so'; echo LDD_EXIT_CODE=\$?; exit 0" || exit 90
echo 'IDENTITY_END'

for mode in getline_member read_member; do
  run_cell "$mode" unpatched "$remote/unpatched" "$remote/dso/unpatched.so" || exit $?
  run_cell "$mode" layer_ab "$remote/layer_ab" "$remote/dso/unpatched.so" || exit $?
  run_cell "$mode" key_library_only "$remote/key" "$remote/dso/unpatched.so" 1 || exit $?
  run_cell "$mode" mirror_patched_consumer_old_library "$remote/layer_ab" "$remote/dso/patched.so" || exit $?
  run_cell "$mode" libstdcxx_control /lib:/usr/lib "$remote/dso/libstdcxx.so" || exit $?
done
