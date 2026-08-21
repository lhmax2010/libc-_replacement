#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive="$workspace/tmp/R58/r58-board-payload.tar"
shared=/opt/usr/home/owner/share/tmp/r58-board-payload.tar
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
cmd="set -e; mkdir /var/tmp/r58_20260821; cp '$shared' /var/tmp/r58-board-payload.tar; cd /var/tmp/r58_20260821; tar -xf /var/tmp/r58-board-payload.tar; sha256sum -c MANIFEST.sha256; chmod -R u+rx bin upstream; printf 'REMOTE_DEPLOY_OK=1\\n'"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"

