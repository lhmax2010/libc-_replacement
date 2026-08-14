#!/bin/bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r36_s1_20260814
local_dir=/home/toolchain/development/libc++_replacement/tmp/R36/probes-armv7l

run_remote() {
  local command=$1
  printf 'SDB_COMMAND='
  printf '%q ' "$sdb" -s "$serial" shell "$command"
  printf '\n'
  "$sdb" -s "$serial" shell "$command"
  local rc=$?
  printf 'SDB_EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

transfer_base64_chunks() {
  local source=$1
  local target=$2
  local encoded offset chunk
  encoded=$(gzip -c "$source" | base64 -w0)
  printf 'TRANSFER_SOURCE=%s\nTRANSFER_TARGET=%s\nTRANSFER_SOURCE_SHA256=' "$source" "$target"
  sha256sum "$source" | awk '{print $1}'
  printf 'TRANSFER_ENCODING=gzip+base64\nTRANSFER_ENCODED_BYTES=%d\nTRANSFER_CHUNK_BYTES=512\n' "${#encoded}"
  run_remote ": > '$target.gz.b64'"
  offset=0
  while [ "$offset" -lt "${#encoded}" ]; do
    chunk=${encoded:offset:512}
    run_remote "printf '%s' '$chunk' >> '$target.gz.b64'"
    offset=$((offset + ${#chunk}))
  done
  run_remote "base64 -d '$target.gz.b64' > '$target.gz' && gzip -dc '$target.gz' > '$target' && unlink '$target.gz.b64' && unlink '$target.gz' && chmod 700 '$target' && sha256sum '$target'"
}

run_remote "test ! -e '$remote' && mkdir '$remote' && chmod 700 '$remote'"
transfer_base64_chunks "$local_dir/libc++abi.so.1.0" "$remote/libc++abi.so.1.0"
transfer_base64_chunks "$local_dir/s1_libcxxabi" "$remote/s1_libcxxabi"
transfer_base64_chunks "$local_dir/s1_libstdcxx" "$remote/s1_libstdcxx"
run_remote "ln -s libc++abi.so.1.0 '$remote/libc++abi.so.1' && ln -s libc++abi.so.1 '$remote/libc++abi.so'"
run_remote "ls -laZ '$remote' && sha256sum '$remote/libc++abi.so.1.0' '$remote/s1_libcxxabi' '$remote/s1_libstdcxx'"
