#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r55_20260821
archive="$workspace/tmp/R55/r55-board-payload.tar"
remote_archive=/opt/usr/home/owner/share/tmp/r55-board-payload.tar

printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$remote_archive"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$remote_archive"
command="set +e; mkdir '$remote' && cd '$remote' && tar -xf '$remote_archive' && chmod 0755 bin/* runtime/*/* && sha256sum bin/* runtime/*/*; rc=\$?; printf 'REMOTE_DEPLOY_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
result=$("$sdb" -s "$serial" shell "$command")
printf '%s\n' "$result"
printf 'SDB_CARRIER_EXIT_CODE=0\n'
remote_rc=$(printf '%s\n' "$result" | awk -F= '/^REMOTE_DEPLOY_EXIT_CODE=/{value=$2} END{gsub(/[^0-9]/, "", value); print value}')
[[ $remote_rc == 0 ]]
