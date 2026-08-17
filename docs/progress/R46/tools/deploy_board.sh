#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
workspace=/home/toolchain/development/libc++_replacement
archive="$workspace/tmp/R46/r46_board_payload.tar"
shared=/opt/usr/home/owner/share/tmp/r46_board_payload.tar
remote=/var/tmp/r46_20260817

printf 'COMMAND='
printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"
printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
printf 'EXIT_CODE=0\n'

command="set -e; test ! -e '$remote'; mkdir -p '$remote'; tar -C '$remote' -xf '$shared'; rm -f '$shared'; test ! -e '$shared'; chmod 0755 '$remote/bin/dso_launcher' '$remote/dso/'*.so; chmod 0644 '$remote/unpatched/'*.so.1 '$remote/layer_ab/'*.so.1 '$remote/badbit_only/'*.so.1 '$remote/failbit/'*.so.1; cd '$remote'; sha256sum -c MANIFEST.sha256; find '$remote' -maxdepth 2 -type f -ls"
printf 'COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "$command"
printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'EXIT_CODE=0\n'
