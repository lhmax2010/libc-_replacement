#!/bin/bash
set -u

log=$1
shift
mkdir -p "$(dirname "$log")"
{
  printf 'PWD=%s\n' "$PWD"
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
  "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
} >"$log" 2>&1
exit "$rc"
