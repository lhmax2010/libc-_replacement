#!/bin/bash
set -u

if [ "$#" -lt 3 ]; then
  echo "usage: $0 NUMBER LABEL COMMAND..." >&2
  exit 2
fi

number=$1
label=$2
shift 2
raw_dir=$(cd "$(dirname "$0")/../raw" && pwd)
prefix="$raw_dir/${number}_${label}"

{
  printf 'cwd: %s\n' "$PWD"
  printf 'command:'
  printf ' %q' "$@"
  printf '\n'
} >"${prefix}.command.txt"
set +e
"$@" >"${prefix}.stdout" 2>"${prefix}.stderr"
rc=$?
printf '%s\n' "$rc" >"${prefix}.exitcode"
cat "${prefix}.stdout"
cat "${prefix}.stderr" >&2
exit "$rc"
