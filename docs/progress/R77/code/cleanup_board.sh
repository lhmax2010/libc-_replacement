#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote_root=/opt/usr/home/owner/share/tmp/r77_lit_20260829

snapshot=$("$sdb" -s "$serial" shell "ps -ef" | tr -d '\r')
if grep -E 'r77_lit_20260829|sdb_executor\.py|lit\.py' <<<"$snapshot" >/dev/null; then
  printf 'ERROR: R77-related process exists; refusing cleanup\n' >&2
  printf '%s\n' "$snapshot" >&2
  exit 12
fi

"$sdb" -s "$serial" shell "if test -e '$remote_root'; then find '$remote_root' -maxdepth 3 -print; else echo ABSENT; fi"
"$sdb" -s "$serial" shell "rm -rf '$remote_root'"

root_state=$("$sdb" -s "$serial" shell "if test -e '$remote_root'; then echo PRESENT; else echo ABSENT; fi" | tr -d '\r')
after=$("$sdb" -s "$serial" shell "ps -ef" | tr -d '\r')
if [[ "$root_state" != "ABSENT" ]]; then
  printf 'ERROR: task root still present: %s\n' "$root_state" >&2
  exit 13
fi
if grep -E 'r77_lit_20260829|sdb_executor\.py|lit\.py' <<<"$after" >/dev/null; then
  printf 'ERROR: R77-related process remains after cleanup\n' >&2
  printf '%s\n' "$after" >&2
  exit 14
fi
printf 'ROOT_STATE=%s\nPROCESS_STATE=ABSENT\nRESULT=PASS\n' "$root_state"
