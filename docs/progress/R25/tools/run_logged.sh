#!/usr/bin/env bash
set -u

if [ "$#" -lt 2 ]; then
  echo "usage: $0 LOG_FILE COMMAND [ARG ...]" >&2
  exit 2
fi

log_file=$1
shift
mkdir -p "$(dirname "$log_file")"
{
  printf 'PWD=%q\n' "$PWD"
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
  "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  exit "$rc"
} >"$log_file" 2>&1
