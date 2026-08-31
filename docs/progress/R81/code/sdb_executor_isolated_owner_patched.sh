#!/usr/bin/env bash
set -euo pipefail

prefix=()
command=()
seen_separator=0
for argument in "$@"; do
  if [[ "$seen_separator" -eq 0 && "$argument" == "--" ]]; then
    seen_separator=1
  elif [[ "$seen_separator" -eq 0 ]]; then
    prefix+=("$argument")
  else
    command+=("$argument")
  fi
done
if [[ "$seen_separator" -ne 1 || "${#command[@]}" -eq 0 ]]; then
  printf 'ERROR: executor wrapper did not receive -- and a command\n' >&2
  exit 10
fi

exec /home/toolchain/development/libc++_replacement/docs/progress/R77/code/sdb_executor.py \
  --env TMPDIR=/opt/usr/r81_fixture/owner_patched/tmp \
        TMP=/opt/usr/r81_fixture/owner_patched/tmp \
        TEMP=/opt/usr/r81_fixture/owner_patched/tmp \
  "${prefix[@]}" -- su owner -c 'exec "$0" "$@"' "${command[@]}"
