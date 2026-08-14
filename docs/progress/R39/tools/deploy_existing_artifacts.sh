#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r39_s1_20260814

run_remote() {
  local command=$1
  local output remote_rc
  printf 'SDB_COMMAND='
  printf '%q ' "$sdb" -s "$serial" shell "$command"
  printf '\n'
  output=$("$sdb" -s "$serial" shell "set +e; $command; remote_rc=\$?; printf 'REMOTE_COMMAND_EXIT_CODE=%d\\n' \"\$remote_rc\"")
  local rc=$?
  printf '%s\n' "$output"
  printf 'SDB_EXIT_CODE=%d\n' "$rc"
  if [ "$rc" -ne 0 ]; then
    return "$rc"
  fi
  remote_rc=$(printf '%s\n' "$output" | sed -n 's/^REMOTE_COMMAND_EXIT_CODE=//p' | tail -n1 | tr -d '\r')
  if [ -z "$remote_rc" ]; then
    printf 'REMOTE_COMMAND_EXIT_CODE=NOT_OBSERVED\n'
    return 97
  fi
  return "$remote_rc"
}

transfer() {
  local source=$1
  local target=$2
  local encoded offset chunk
  encoded=$(gzip -n -c "$source" | base64 -w0) || return $?
  printf 'TRANSFER_SOURCE=%s\nTRANSFER_TARGET=%s\n' "$source" "$target"
  printf 'TRANSFER_SOURCE_SHA256='
  sha256sum "$source" | awk '{print $1}'
  printf 'TRANSFER_ENCODING=gzip-n+base64\nTRANSFER_ENCODED_BYTES=%d\nTRANSFER_CHUNK_BYTES=512\n' "${#encoded}"
  run_remote ": > '$target.gz.b64'" || return $?
  offset=0
  while [ "$offset" -lt "${#encoded}" ]; do
    chunk=${encoded:offset:512}
    run_remote "printf '%s' '$chunk' >> '$target.gz.b64'" || return $?
    offset=$((offset + ${#chunk}))
  done
  run_remote "base64 -d '$target.gz.b64' > '$target.gz' && gzip -dc '$target.gz' > '$target' && rm -f '$target.gz.b64' '$target.gz' && chmod 700 '$target' && sha256sum '$target'" || return $?
}

run_remote "test ! -e '$remote' && mkdir '$remote' && chmod 700 '$remote'" || exit $?
transfer tmp/R36/probes-armv7l/s1_libcxxabi "$remote/r36_test" || exit $?
transfer tmp/R38/probes-armv7l/s1_libcxx_strict "$remote/r38_static_test" || exit $?
transfer tmp/R36/build-armv7l/lib/libc++abi.so.1.0 "$remote/libc++abi.so.1" || exit $?
run_remote "find '$remote' -maxdepth 1 -ls; sha256sum '$remote/r36_test' '$remote/r38_static_test' '$remote/libc++abi.so.1'" || exit $?
