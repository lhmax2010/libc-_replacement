#!/usr/bin/env bash
set -uo pipefail

if [[ $# -lt 2 ]]; then
  echo "usage: $0 LOG_FILE COMMAND [ARG ...]" >&2
  exit 64
fi

log_file=$1
shift
mkdir -p "$(dirname "$log_file")"

{
  printf 'COMMAND:'
  printf ' %q' "$@"
  printf '\n'
  printf 'PWD: %q\n' "$PWD"
  printf 'START_UTC: %s\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
} >"$log_file"

set +e
"$@" >>"$log_file" 2>&1
rc=$?
set -e

{
  printf 'EXIT_CODE: %s\n' "$rc"
  printf 'END_UTC: %s\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
} >>"$log_file"

exit "$rc"
