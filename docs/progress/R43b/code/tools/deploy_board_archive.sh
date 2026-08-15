#!/bin/bash
set -eu

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
workspace=/home/toolchain/development/libc++_replacement
archive="$workspace/tmp/R43b/r43b-armv7l.tar.gz"
remote=/var/tmp/r43b_20260815
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

local_sha=$(sha256sum "$archive" | awk '{print $1}')
encoded=$(base64 -w0 "$archive")
printf 'ARCHIVE=%s\nARCHIVE_SHA256=%s\nENCODED_BYTES=%d\nCHUNK_BYTES=%d\n' \
  "$archive" "$local_sha" "${#encoded}" "$chunk_bytes"
run_remote "rm -rf '$remote' && mkdir -p '$remote' && chmod 700 '$remote' && : > '$remote/payload.tar.gz.b64'"
offset=0
while [ "$offset" -lt "${#encoded}" ]; do
  chunk=${encoded:offset:chunk_bytes}
  run_remote "printf '%s' '$chunk' >> '$remote/payload.tar.gz.b64'"
  offset=$((offset + ${#chunk}))
done
run_remote "base64 -d '$remote/payload.tar.gz.b64' > '$remote/payload.tar.gz' && rm -f '$remote/payload.tar.gz.b64' && cd '$remote' && tar -xzf payload.tar.gz && rm -f payload.tar.gz && chmod -R u+rwX,go-rwx '$remote'"
remote_manifest=$("$sdb" -s "$serial" shell "cd '$remote' && sha256sum -c MANIFEST.sha256" | tr -d '\r')
printf 'REMOTE_MANIFEST_CHECK_BEGIN\n%s\nREMOTE_MANIFEST_CHECK_END\n' "$remote_manifest"
run_remote "find '$remote' -maxdepth 3 -type f -ls"
