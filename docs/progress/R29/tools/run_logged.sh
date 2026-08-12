#!/bin/bash
set -uo pipefail
if [ "$#" -lt 2 ]; then
  echo "usage: $0 LOG COMMAND [ARG ...]" >&2
  exit 64
fi
log=$1
shift
mkdir -p "$(dirname "$log")"
{
  printf 'PWD=%q\n' "$PWD"
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
} >"$log"
set +e
"$@" >>"$log" 2>&1
rc=$?
set -e
printf 'EXIT_CODE=%d\n' "$rc" >>"$log"
exit "$rc"
