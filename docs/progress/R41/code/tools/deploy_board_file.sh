#!/bin/bash
set -u
if [ "$#" -ne 2 ]; then printf 'usage: %s LOCAL REMOTE\n' "$0" >&2; exit 2; fi
local_file=$1
remote_file=$2
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
printf 'TRANSFER_METHOD=gzip_n_base64_chunks_1024_via_sdb_shell\n'
printf 'LOCAL_SHA256='; sha256sum "$local_file"
encoded=$(gzip -n -c "$local_file" | base64 -w0) || exit $?
printf 'TRANSFER_ENCODED_BYTES=%d\nTRANSFER_CHUNK_BYTES=1024\n' "${#encoded}"
"$sdb" -s "$serial" shell "mkdir -p '$(dirname "$remote_file")'; chmod 700 '$(dirname "$remote_file")'; rm -f '$remote_file.gz.b64' '$remote_file.gz' '$remote_file'; : > '$remote_file.gz.b64'" || exit $?
offset=0
while [ "$offset" -lt "${#encoded}" ]; do
  chunk=${encoded:offset:1024}
  "$sdb" -s "$serial" shell "printf '%s' '$chunk' >> '$remote_file.gz.b64'" || exit $?
  offset=$((offset + ${#chunk}))
done
"$sdb" -s "$serial" shell "base64 -d '$remote_file.gz.b64' > '$remote_file.gz' && gzip -dc '$remote_file.gz' > '$remote_file'; rc=\$?; rm -f '$remote_file.gz.b64' '$remote_file.gz'; chmod 700 '$remote_file'; sha256sum '$remote_file'; exit \$rc"
rc=$?
exit "$rc"
