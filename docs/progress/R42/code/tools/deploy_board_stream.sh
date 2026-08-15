#!/bin/bash
set -eu -o pipefail
workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r42_s1_20260815
arm="$workspace/tmp/R42/probes/armv7l"

remote_run() {
  command=$1
  printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
  "$sdb" -s "$serial" shell "$command"
  rc=$?
  printf 'SDB_EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

transfer() {
  source=$1
  target=$2
  printf 'TRANSFER_SOURCE=%s\nTRANSFER_TARGET=%s\n' "$source" "$target"
  printf 'TRANSFER_SOURCE_SHA256='; sha256sum "$source" | awk '{print $1}'
  printf 'TRANSFER_COMMAND='; printf '%q ' gzip -n -c "$source"; printf '| '; printf '%q ' "$sdb" -s "$serial" shell "gzip -dc > '$target'"; printf '\n'
  gzip -n -c "$source" | "$sdb" -s "$serial" shell "gzip -dc > '$target'"
  rc=$?
  printf 'TRANSFER_EXIT_CODE=%d\n' "$rc"
  test "$rc" -eq 0
  remote_run "chmod 700 '$target' && sha256sum '$target'"
}

remote_run "rm -rf '$remote' && mkdir -p '$remote/patched' '$remote/trace' '$remote/unpatched' && chmod 700 '$remote' '$remote/patched' '$remote/trace' '$remote/unpatched'"
transfer "$arm/s1_libcxx" "$remote/s1_libcxx"
transfer "$arm/s1_libstdcxx" "$remote/s1_libstdcxx"
transfer "$arm/unpatched_libcxx" "$remote/unpatched_libcxx"
transfer "$workspace/tmp/R42/build-patched-armv7l/lib/libc++abi.so.1.0" "$remote/patched/libc++abi.so.1"
transfer "$workspace/tmp/R42/build-trace-armv7l/lib/libc++abi.so.1.0" "$remote/trace/libc++abi.so.1"
transfer "$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++abi.so.1.0" "$remote/unpatched/libc++abi.so.1"
remote_run "find '$remote' -maxdepth 2 -ls"
