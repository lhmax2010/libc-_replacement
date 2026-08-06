#!/usr/bin/env bash
set +e

if (( $# < 2 )); then
  printf 'usage: %s LOG COMMAND [ARG ...]\n' "$0" >&2
  exit 64
fi

log=$1
shift
mkdir -p -- "$(dirname -- "$log")"

{
  printf 'PWD='
  printf '%q' "$PWD"
  printf '\nCOMMAND='
  printf ' %q' "$@"
  printf '\n'
  "$@"
  rc=$?
  printf '\nEXIT_CODE=%d\n' "$rc"
  exit "$rc"
} >"$log" 2>&1
