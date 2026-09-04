#!/bin/bash
set -u

if [ "$#" -lt 3 ]; then
  echo "usage: $0 NUMBER LABEL COMMAND [ARG ...]" >&2
  exit 2
fi

number=$1
label=$2
shift 2
log_root=/home/toolchain/development/libc++_replacement/progress/R112/raw
mkdir -p "$log_root"
prefix="$log_root/${number}_${label}"
printf '%q ' "$@" >"${prefix}.command.txt"
printf '\n' >>"${prefix}.command.txt"
"$@" >"${prefix}.stdout.txt" 2>"${prefix}.stderr.txt"
rc=$?
printf '%s\n' "$rc" >"${prefix}.exitcode"
printf 'log=%s rc=%s\n' "$prefix" "$rc"
exit "$rc"
