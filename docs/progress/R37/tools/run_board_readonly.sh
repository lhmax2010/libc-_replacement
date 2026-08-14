#!/bin/bash
set -u

if [ "$#" -ne 2 ]; then
    printf 'usage: %s DEVICE LOCAL_SCRIPT\n' "$0" >&2
    exit 2
fi

device=$1
local_script=$2
if [ ! -r "$local_script" ]; then
    printf 'NOT_READABLE=%s\n' "$local_script" >&2
    exit 3
fi

payload=$(gzip -n -c "$local_script" | base64 -w0)
remote_command="printf '%s' '$payload' | base64 -d | gzip -dc | sh"
printf 'LOCAL_SCRIPT=%s\n' "$local_script"
sha256sum "$local_script"
printf 'REMOTE_COMMAND='
printf '%q' "$remote_command"
printf '\n'
sdb -s "$device" shell "$remote_command"
rc=$?
printf 'SDB_SHELL_RC=%d\n' "$rc"
exit "$rc"
