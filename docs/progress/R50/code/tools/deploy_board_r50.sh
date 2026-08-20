#!/usr/bin/env bash
set -euo pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
workspace=/home/toolchain/development/libc++_replacement
archive="$workspace/tmp/R50/r50_board_payload.tar"
shared=/opt/usr/home/owner/share/tmp/r50_board_payload.tar
remote=/var/tmp/r50_20260820
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
printf 'EXIT_CODE=0\n'
command="set -e; test ! -e '$remote'; mkdir -p '$remote'; tar -C '$remote' -xf '$shared'; rm -f '$shared'; test ! -e '$shared'; chmod 0755 '$remote/bin/dso_launcher' '$remote/dso/'*.so; cd '$remote'; sha256sum -c MANIFEST.sha256; find '$remote' -maxdepth 2 -type f -ls"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'EXIT_CODE=0\n'
