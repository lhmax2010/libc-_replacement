#!/usr/bin/env bash
set -euo pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive=/home/toolchain/development/libc++_replacement/tmp/R54/r54_board_payload.tar
shared=/opt/usr/home/owner/share/tmp/r54_board_payload.tar
remote=/var/tmp/r54_20260821
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
printf 'TRANSFER_EXIT_CODE=0\n'
command="set -e; mkdir '$remote'; tar -C '$remote' -xf '$shared'; chmod 0755 '$remote/bin/'* '$remote/upstream/libcxx/'* '$remote/upstream/libstdcxx/'*; cd '$remote'; sha256sum -c MANIFEST.sha256"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'DEPLOY_EXIT_CODE=0\n'
