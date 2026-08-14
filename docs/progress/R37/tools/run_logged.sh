#!/bin/bash
set -u

if [ "$#" -lt 2 ]; then
    printf 'usage: %s LOG COMMAND [ARG...]\n' "$0" >&2
    exit 2
fi

log=$1
shift
mkdir -p "$(dirname "$log")"
{
    printf 'PWD=%q\n' "$PWD"
    printf 'COMMAND='
    printf ' %q' "$@"
    printf '\n'
} > "$log"

"$@" >> "$log" 2>&1
rc=$?
printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
exit "$rc"
