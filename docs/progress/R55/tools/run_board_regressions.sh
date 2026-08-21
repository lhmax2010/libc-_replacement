#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r55_20260821
raw="$workspace/progress/R55/raw/armv7l"
mkdir -p "$raw/regression" "$raw/upstream"

run_case() {
  local log=$1 binary=$2 mode=$3 cmd carrier remote_rc
  cmd="set +e; cd '$remote'; LD_LIBRARY_PATH='$remote/runtime/full' '$remote/$binary' '$mode' & pid=\$!; (sleep 10; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
  printf 'COMMAND=' > "$log"; printf '%q ' "$sdb" -s "$serial" shell "$cmd" >> "$log"; printf '\n' >> "$log"
  carrier=0
  "$sdb" -s "$serial" shell "$cmd" >> "$log" 2>&1 || carrier=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier" >> "$log"
  (( carrier == 0 )) || return 90
  remote_rc=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{value=$2} END{gsub(/[^0-9]/, "", value); print value}' "$log")
  [[ $remote_rc =~ ^[0-9]+$ ]] || return 91
  [[ $remote_rc != 126 && $remote_rc != 127 ]] || return 92
}

for rt in libcxx libstdcxx; do
  for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
    run_case "$raw/regression/${rt}_$mode.log" "regression/$rt" "$mode"
  done
done
for rt in libcxx libstdcxx; do
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
    run_case "$raw/upstream/${rt}_$test.log" "upstream/$rt/$test" ignored
  done
done
