#!/bin/bash
set -eu
workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r42_s1_20260815
arm="$workspace/tmp/R42/probes/armv7l"

run() {
  printf 'COMMAND='; printf '%q ' "$@"; printf '\n'
  "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

run "$sdb" -s "$serial" shell "mkdir -p '$remote/patched' '$remote/trace' '$remote/unpatched'"
run "$sdb" -s "$serial" push "$arm/s1_libcxx" "$remote/s1_libcxx"
run "$sdb" -s "$serial" push "$arm/s1_libstdcxx" "$remote/s1_libstdcxx"
run "$sdb" -s "$serial" push "$arm/unpatched_libcxx" "$remote/unpatched_libcxx"
run "$sdb" -s "$serial" push "$workspace/tmp/R42/build-patched-armv7l/lib/libc++abi.so.1.0" "$remote/patched/libc++abi.so.1"
run "$sdb" -s "$serial" push "$workspace/tmp/R42/build-trace-armv7l/lib/libc++abi.so.1.0" "$remote/trace/libc++abi.so.1"
run "$sdb" -s "$serial" push "$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++abi.so.1.0" "$remote/unpatched/libc++abi.so.1"
run "$sdb" -s "$serial" shell "chmod 755 '$remote/s1_libcxx' '$remote/s1_libstdcxx' '$remote/unpatched_libcxx' '$remote/patched/libc++abi.so.1' '$remote/trace/libc++abi.so.1' '$remote/unpatched/libc++abi.so.1'; sha256sum '$remote/s1_libcxx' '$remote/s1_libstdcxx' '$remote/unpatched_libcxx' '$remote/patched/libc++abi.so.1' '$remote/trace/libc++abi.so.1' '$remote/unpatched/libc++abi.so.1'"
