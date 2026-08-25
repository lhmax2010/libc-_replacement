#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive="$workspace/tmp/R67/r67-board-payload.tar"
shared=/opt/usr/home/owner/share/tmp/r67-board-payload.tar
remote=/var/tmp/r67_20260825

printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
cmd="set -e; test ! -e '$remote'; mkdir '$remote'; cp '$shared' /var/tmp/r67-board-payload.tar; cd '$remote'; tar -xf /var/tmp/r67-board-payload.tar; sha256sum -c MANIFEST.sha256; chmod -R u+rx bin upstream; printf 'REMOTE_DEPLOY_OK=1\\n'"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
output=$("$sdb" -s "$serial" shell "$cmd" 2>&1)
printf '%s\n' "$output"
grep -F 'REMOTE_DEPLOY_OK=1' <<< "$output" >/dev/null
