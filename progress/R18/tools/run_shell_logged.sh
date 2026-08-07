#!/usr/bin/env bash
set +e
if [ "$#" -ne 2 ]; then
  echo "usage: $0 LOG SHELL_COMMAND" >&2
  exit 64
fi
log=$1
command_text=$2
mkdir -p "$(dirname "$log")"
{
  printf 'PWD='
  pwd
  printf 'COMMAND=bash -lc %q\n' "$command_text"
  bash -lc "$command_text"
  rc=$?
  printf 'EXIT_CODE=%s\n' "$rc"
  exit "$rc"
} >"$log" 2>&1
