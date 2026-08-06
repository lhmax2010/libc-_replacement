#!/usr/bin/env bash
set +e
if [ "$#" -ne 2 ]; then
  echo "usage: $0 LOG REMOTE_COMMAND" >&2
  exit 64
fi
log=$1
remote_command=$2
mkdir -p "$(dirname "$log")"
{
  printf 'PWD='
  pwd
  printf 'COMMAND=sdb -s 192.168.108.25:26101 shell %q\n' "$remote_command"
  sdb -s 192.168.108.25:26101 shell "$remote_command"
  rc=$?
  printf 'EXIT_CODE=%s\n' "$rc"
  exit "$rc"
} >"$log" 2>&1
