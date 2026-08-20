#!/usr/bin/env bash
set -euo pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive=/home/toolchain/development/libc++_replacement/tmp/R53/r53_board_supplement.tar
shared=/opt/usr/home/owner/share/tmp/r53_board_supplement.tar
remote=/var/tmp/r53_20260820
printf 'PUSH_COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
printf 'PUSH_EXIT_CODE=%d\n' "$?"
command="set -e; test -d '$remote'; mkdir -p '$remote/supplement'; tar -C '$remote/supplement' -xf '$shared'; chmod 0755 '$remote/supplement/regression/'* '$remote/supplement/r52/'*; cd '$remote/supplement'; sha256sum -c MANIFEST.sha256"
printf 'DEPLOY_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'DEPLOY_EXIT_CODE=%d\n' "$?"
