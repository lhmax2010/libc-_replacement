#!/bin/bash
set -eu
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
workspace=/home/toolchain/development/libc++_replacement
remote=/var/tmp/r42_s1_20260815
arm="$workspace/tmp/R42/probes/armv7l"
chunk_bytes=512

run_remote() {
  local command=$1 output carrier_rc remote_rc
  printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
  output=$("$sdb" -s "$serial" shell "set +e; $command; remote_rc=\$?; printf 'REMOTE_COMMAND_EXIT_CODE=%d\\n' \"\$remote_rc\"")
  carrier_rc=$?
  printf '%s\nSDB_EXIT_CODE=%d\n' "$output" "$carrier_rc"
  test "$carrier_rc" -eq 0 || return "$carrier_rc"
  remote_rc=$(printf '%s\n' "$output" | sed -n 's/^REMOTE_COMMAND_EXIT_CODE=//p' | tail -n1 | tr -d '\r')
  test -n "$remote_rc" || return 97
  test "$remote_rc" -eq 0
}

transfer() {
  local source=$1 target=$2 encoded offset chunk local_sha remote_sha
  local_sha=$(sha256sum "$source" | awk '{print $1}')
  encoded=$(gzip -n -c "$source" | base64 -w0)
  printf 'TRANSFER_SOURCE=%s\nTRANSFER_TARGET=%s\nTRANSFER_SOURCE_SHA256=%s\n' "$source" "$target" "$local_sha"
  printf 'TRANSFER_ENCODING=gzip-n+base64\nTRANSFER_ENCODED_BYTES=%d\nTRANSFER_CHUNK_BYTES=%d\n' "${#encoded}" "$chunk_bytes"
  run_remote ": > '$target.gz.b64'"
  offset=0
  while [ "$offset" -lt "${#encoded}" ]; do
    chunk=${encoded:offset:chunk_bytes}
    run_remote "printf '%s' '$chunk' >> '$target.gz.b64'"
    offset=$((offset + ${#chunk}))
  done
  run_remote "base64 -d '$target.gz.b64' > '$target.gz' && gzip -dc '$target.gz' > '$target' && rm -f '$target.gz.b64' '$target.gz' && chmod 700 '$target'"
  remote_sha=$("$sdb" -s "$serial" shell "sha256sum '$target'" | awk '{print $1}' | tr -d '\r')
  printf 'TRANSFER_REMOTE_SHA256=%s\n' "$remote_sha"
  if [ "$remote_sha" != "$local_sha" ]; then
    printf 'TRANSFER_IDENTITY=MISMATCH\n'
    return 98
  fi
  printf 'TRANSFER_IDENTITY=MATCH\n'
}

run_remote "rm -rf '$remote' && mkdir -p '$remote/patched' '$remote/trace' '$remote/unpatched' && chmod 700 '$remote' '$remote/patched' '$remote/trace' '$remote/unpatched'"
transfer "$arm/s1_libcxx" "$remote/s1_libcxx"
transfer "$arm/s1_libstdcxx" "$remote/s1_libstdcxx"
transfer "$arm/unpatched_libcxx" "$remote/unpatched_libcxx"
transfer "$workspace/tmp/R42/deploy/armv7l/patched/libc++abi.so.1" "$remote/patched/libc++abi.so.1"
transfer "$workspace/tmp/R42/deploy/armv7l/trace/libc++abi.so.1" "$remote/trace/libc++abi.so.1"
transfer "$workspace/tmp/R42/deploy/armv7l/unpatched/libc++abi.so.1" "$remote/unpatched/libc++abi.so.1"
run_remote "find '$remote' -maxdepth 2 -ls"
