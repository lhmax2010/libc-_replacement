#!/usr/bin/env bash
set -euo pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive=/home/toolchain/development/libc++_replacement/tmp/R53/r53_board_payload.tar
shared=/opt/usr/home/owner/share/tmp/r53_board_payload.tar
remote=/var/tmp/r53_20260820
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"; printf 'PUSH_EXIT_CODE=0\n'
command="set -e; mkdir '$remote'; tar -C '$remote' -xf '$shared'; chmod 0755 '$remote/bin/dso_launcher' '$remote/dso/'*.so; cd '$remote'; sha256sum -c MANIFEST.sha256"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"; printf 'DEPLOY_EXIT_CODE=0\n'

