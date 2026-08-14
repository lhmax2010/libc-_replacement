#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r39_s1_20260814
source_file=tmp/R39/r38_static_export_dynamic
target="$remote/r38_static_export_dynamic"

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
  if [ "$rc" -ne 0 ]; then return "$rc"; fi
  remote_rc=$(printf '%s\n' "$output" | sed -n 's/^REMOTE_COMMAND_EXIT_CODE=//p' | tail -n1 | tr -d '\r')
  if [ -z "$remote_rc" ]; then
    printf 'REMOTE_COMMAND_EXIT_CODE=NOT_OBSERVED\n'
    return 97
  fi
  return "$remote_rc"
}

encoded=$(gzip -n -c "$source_file" | base64 -w0) || exit $?
printf 'TRANSFER_SOURCE=%s\nTRANSFER_TARGET=%s\n' "$source_file" "$target"
printf 'TRANSFER_SOURCE_SHA256='
sha256sum "$source_file" | awk '{print $1}'
printf 'TRANSFER_ENCODING=gzip-n+base64\nTRANSFER_ENCODED_BYTES=%d\nTRANSFER_CHUNK_BYTES=512\n' "${#encoded}"
run_remote "test -d '$remote' && test ! -e '$target' && : > '$target.gz.b64'" || exit $?
offset=0
while [ "$offset" -lt "${#encoded}" ]; do
  chunk=${encoded:offset:512}
  run_remote "printf '%s' '$chunk' >> '$target.gz.b64'" || exit $?
  offset=$((offset + ${#chunk}))
done
run_remote "base64 -d '$target.gz.b64' > '$target.gz' && gzip -dc '$target.gz' > '$target' && rm -f '$target.gz.b64' '$target.gz' && chmod 700 '$target' && sha256sum '$target' && LD_LIBRARY_PATH='$remote' ldd '$target'" || exit $?
