#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
stage="$workspace/tmp/R44b/deploy/armv7l"
remote=/var/tmp/r44b_20260815

run_sdb() {
  local attempt rc
  for attempt in 1 2 3; do
    printf 'SDB_ATTEMPT=%d COMMAND=' "$attempt"
    printf '%q ' "$sdb" -s "$serial" "$@"
    printf '\n'
    "$sdb" -s "$serial" "$@"
    rc=$?
    printf 'SDB_EXIT_CODE=%d\n' "$rc"
    (( rc == 0 )) && return 0
  done
  return "$rc"
}

transfer() {
  local file=$1 target=$2 attempt rc
  for attempt in 1 2 3; do
    printf 'TRANSFER_ATTEMPT=%d SOURCE=%q TARGET=%q\n' "$attempt" "$file" "$target"
    printf 'TRANSFER_COMMAND='; printf '%q ' gzip -n -c "$file"; printf '| '; printf '%q ' "$sdb" -s "$serial" shell "gzip -dcf > '$target'"; printf '\n'
    gzip -n -c "$file" | "$sdb" -s "$serial" shell "gzip -dcf > '$target'"
    rc=$?
    printf 'TRANSFER_EXIT_CODE=%d\n' "$rc"
    (( rc == 0 )) && return 0
  done
  return "$rc"
}

run_sdb shell "rm -rf '$remote' && mkdir -p '$remote/unpatched' '$remote/layer1' '$remote/aggregation' '$remote/bin'" || exit 90
files=(
  unpatched/libc++.so.1 unpatched/libc++abi.so.1
  layer1/libc++.so.1 layer1/libc++abi.so.1
  aggregation/libc++.so.1 aggregation/libc++abi.so.1
  bin/libcxx_probe bin/libstdcxx_probe
)
for rel in "${files[@]}"; do
  transfer "$stage/$rel" "$remote/$rel" || exit 90
done
run_sdb shell "chmod 0755 '$remote/bin/libcxx_probe' '$remote/bin/libstdcxx_probe'; chmod 0644 '$remote/unpatched/'*.so.1 '$remote/layer1/'*.so.1 '$remote/aggregation/'*.so.1; find '$remote' -type f -exec sha256sum {} \;" || exit 90

run_sdb shell "set +e; for suite in unpatched layer1 aggregation; do echo LOADER_LIST_SUITE=\$suite; LD_LIBRARY_PATH='$remote/'\$suite ldd '$remote/bin/libcxx_probe'; echo LDD_EXIT_CODE=\$?; done; echo LOADER_LIST_SUITE=libstdcxx_control; ldd '$remote/bin/libstdcxx_probe'; echo LDD_EXIT_CODE=\$?; exit 0" || exit 90
run_sdb shell "set +e; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/aggregation' '$remote/bin/libcxx_probe' control; rc=\$?; echo LD_DEBUG_REMOTE_EXIT_CODE=\$rc; exit 0" || exit 90
