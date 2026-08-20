#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive=/home/toolchain/development/libc++_replacement/tmp/R52/r52_board_payload.tar
shared=/opt/usr/home/owner/share/tmp/r52_board_payload.tar
remote=/var/tmp/r52_20260820

printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
printf 'PUSH_EXIT_CODE=%d\n' "$?"
command="set -e; mkdir '$remote'; tar -C '$remote' -xf '$shared'; chmod 0755 '$remote/bin/dso_launcher' '$remote/dso/'*.so; cd '$remote'; sha256sum -c MANIFEST.sha256"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'DEPLOY_EXIT_CODE=%d\n' "$?"
