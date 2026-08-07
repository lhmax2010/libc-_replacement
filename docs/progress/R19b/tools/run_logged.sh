#!/usr/bin/env bash
set -uo pipefail

if [ "$#" -lt 2 ]; then
  echo "usage: $0 LOG_FILE COMMAND [ARG ...]" >&2
  exit 64
fi

log_file=$1
shift
mkdir -p "$(dirname "$log_file")"
{
  printf 'cwd: %s\n' "$PWD"
  printf 'command:'
  printf ' %q' "$@"
  printf '\n'
  printf '%s\n' '--- stdout+stderr ---'
} >"$log_file"

set +e
"$@" >>"$log_file" 2>&1
rc=$?
set -e
{
  printf '%s\n' '--- end ---'
  printf 'exit_code: %d\n' "$rc"
} >>"$log_file"
exit "$rc"
