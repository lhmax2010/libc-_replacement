#!/usr/bin/env bash
set +e
if [ "$#" -lt 2 ]; then
  echo "usage: $0 LOG COMMAND [ARG ...]" >&2
  exit 64
fi
log=$1
shift
mkdir -p "$(dirname "$log")"
{
  printf 'PWD='
  pwd
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
  "$@"
  rc=$?
  printf 'EXIT_CODE=%s\n' "$rc"
  exit "$rc"
} >"$log" 2>&1
