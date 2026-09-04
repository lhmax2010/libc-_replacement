#!/bin/bash
set -u

if [ "$#" -lt 4 ]; then
  echo "usage: $0 RAW_DIRECTORY NUMBER LABEL COMMAND [ARG ...]" >&2
  exit 2
fi

root=$1
number=$2
label=$3
shift 3
mkdir -p "$root"
prefix="$root/${number}_${label}"
printf '%q ' "$@" >"${prefix}.command.txt"
printf '\n' >>"${prefix}.command.txt"
"$@" >"${prefix}.stdout.txt" 2>"${prefix}.stderr.txt"
rc=$?
printf '%s\n' "$rc" >"${prefix}.exitcode"
printf 'log=%s rc=%s\n' "$prefix" "$rc"
exit "$rc"
