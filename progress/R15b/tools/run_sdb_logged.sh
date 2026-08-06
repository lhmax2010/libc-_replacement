#!/usr/bin/env bash
set -u

if (( $# != 2 )); then
  printf 'usage: %s LOG REMOTE_SCRIPT\n' "$0" >&2
  exit 64
fi

log=$1
remote_script=$2
exec "$(dirname -- "$0")/run_logged.sh" "$log" \
  sdb -s 192.168.108.25:26101 shell "$remote_script"
