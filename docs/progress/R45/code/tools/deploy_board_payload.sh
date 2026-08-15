#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
workspace=/home/toolchain/development/libc++_replacement
archive="$workspace/tmp/R45/r45_board_payload.tar"
shared=/opt/usr/home/owner/share/tmp/r45_board_payload.tar
remote=/var/tmp/r45_20260815

printf 'COMMAND='
printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"
printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
printf 'EXIT_CODE=0\n'

remote_cmd="set -e; test ! -e '$remote'; mkdir -p '$remote'; tar -C '$remote' -xf '$shared'; rm -f '$shared'; test ! -e '$shared'; find '$remote' -type f -exec chmod 0755 {} +; find '$remote' -type f -print | sort; sha256sum '$remote/runtime/unpatched/libc++abi.so.1' '$remote/runtime/r42/libc++abi.so.1' '$remote/runtime/r45/libc++abi.so.1' '$remote/aggregation/fixed/libc++.so.1'"
printf 'COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "$remote_cmd"
printf '\n'
"$sdb" -s "$serial" shell "$remote_cmd"
printf 'EXIT_CODE=0\n'
