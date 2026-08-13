#!/usr/bin/env bash
set -uo pipefail
log=$1
shift
mkdir -p "$(dirname "$log")"
{
    printf 'PWD=%q\n' "$PWD"
    printf 'COMMAND='; printf '%q ' "$@"; printf '\n'
} >"$log"
"$@" >>"$log" 2>&1
rc=$?
printf 'EXIT_CODE=%s\n' "$rc" >>"$log"
exit "$rc"
