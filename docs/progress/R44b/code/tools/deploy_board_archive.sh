#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive="$workspace/tmp/R44b/deploy/r44b-armv7l.tar.gz"
remote=/var/tmp/r44b_20260815
chunk_bytes=65536

run_remote() {
  local command=$1 output carrier_rc remote_rc
  printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
  output=$("$sdb" -s "$serial" shell "set +e; $command; remote_rc=\$?; printf 'REMOTE_COMMAND_EXIT_CODE=%d\\n' \"\$remote_rc\"")
  carrier_rc=$?
  printf '%s\nSDB_EXIT_CODE=%d\n' "$output" "$carrier_rc"
  (( carrier_rc == 0 )) || return "$carrier_rc"
  remote_rc=$(printf '%s\n' "$output" | sed -n 's/^REMOTE_COMMAND_EXIT_CODE=//p' | tail -n1 | tr -d '\r')
  [[ -n $remote_rc ]] || return 97
  (( remote_rc == 0 ))
}

archive_sha=$(sha256sum "$archive" | awk '{print $1}')
encoded=$(base64 -w0 "$archive")
printf 'ARCHIVE=%s\nARCHIVE_SHA256=%s\nENCODED_BYTES=%d\nCHUNK_BYTES=%d\n' \
  "$archive" "$archive_sha" "${#encoded}" "$chunk_bytes"
run_remote "rm -rf '$remote' && mkdir -p '$remote' && : > '$remote/payload.tar.gz.b64'" || exit 90
offset=0
while (( offset < ${#encoded} )); do
  chunk=${encoded:offset:chunk_bytes}
  run_remote "printf '%s' '$chunk' >> '$remote/payload.tar.gz.b64'" || exit 90
  offset=$((offset + ${#chunk}))
done
run_remote "base64 -d '$remote/payload.tar.gz.b64' > '$remote/payload.tar.gz' && rm -f '$remote/payload.tar.gz.b64' && cd '$remote' && tar -xzf payload.tar.gz && rm -f payload.tar.gz && chmod 0755 bin/libcxx_probe bin/libstdcxx_probe && chmod 0644 unpatched/*.so.1 layer1/*.so.1 aggregation/*.so.1 && sha256sum -c MANIFEST.sha256" || exit 90
run_remote "find '$remote' -maxdepth 3 -type f -ls" || exit 90

run_remote "set +e; for suite in unpatched layer1 aggregation; do echo LOADER_LIST_SUITE=\$suite; LD_LIBRARY_PATH='$remote/'\$suite ldd '$remote/bin/libcxx_probe'; echo LDD_EXIT_CODE=\$?; done; echo LOADER_LIST_SUITE=libstdcxx_control; ldd '$remote/bin/libstdcxx_probe'; echo LDD_EXIT_CODE=\$?; exit 0" || exit 90
run_remote "set +e; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/aggregation' '$remote/bin/libcxx_probe' control; rc=\$?; echo LD_DEBUG_REMOTE_EXIT_CODE=\$rc; exit 0" || exit 90
