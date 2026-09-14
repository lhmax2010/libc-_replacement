#!/bin/bash
set -u

if [ "$#" -lt 3 ]; then
  echo "usage: $0 NUMBER LABEL COMMAND [ARG ...]" >&2
  exit 2
fi

number=$1
label=$2
shift 2

base=/home/toolchain/development/libc++_replacement/docs/progress/BUILD_W2_0912/raw
mkdir -p "$base"
prefix="$base/${number}_${label}"
printf '%q ' "$@" >"${prefix}.command.txt"
printf '\n' >>"${prefix}.command.txt"
"$@" >"${prefix}.stdout.txt" 2>"${prefix}.stderr.txt"
rc=$?
printf '%s\n' "$rc" >"${prefix}.exitcode"
exit "$rc"
