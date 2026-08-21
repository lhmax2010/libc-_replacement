#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r55_20260821
raw="$workspace/progress/R55/raw/armv7l"
mkdir -p "$raw/async20" "$raw/facility" "$raw/noncancel"

run_case() {
  local log=$1 variant=$2 binary=$3 mode=$4
  local cmd carrier remote_rc
  cmd="set +e; cd '$remote'; LD_LIBRARY_PATH='$remote/runtime/$variant' '$remote/bin/$binary' '$mode' & pid=\$!; (sleep 4; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
  printf 'COMMAND=' > "$log"; printf '%q ' "$sdb" -s "$serial" shell "$cmd" >> "$log"; printf '\n' >> "$log"
  carrier=0
  "$sdb" -s "$serial" shell "$cmd" >> "$log" 2>&1 || carrier=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier" >> "$log"
  (( carrier == 0 )) || return 90
  remote_rc=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{value=$2} END{gsub(/[^0-9]/, "", value); print value}' "$log")
  [[ $remote_rc =~ ^[0-9]+$ ]] || return 91
  [[ $remote_rc != 126 && $remote_rc != 127 ]] || return 92
}

for iteration in $(seq 1 20); do
  run_case "$raw/async20/baseline_$iteration.log" baseline async_baseline cancel
  run_case "$raw/async20/prefix_$iteration.log" full async_prefix cancel
  run_case "$raw/async20/fixed_$iteration.log" full async_fixed cancel
  run_case "$raw/async20/libstdcxx_$iteration.log" full async_libstdcxx cancel
done

for mode in normal exception; do
  run_case "$raw/noncancel/baseline_$mode.log" baseline async_baseline "$mode"
  run_case "$raw/noncancel/prefix_$mode.log" full async_prefix "$mode"
  run_case "$raw/noncancel/fixed_$mode.log" full async_fixed "$mode"
  run_case "$raw/noncancel/libstdcxx_$mode.log" full async_libstdcxx "$mode"
done

for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case "$raw/facility/baseline_$mode.log" baseline facility_baseline "$mode"
  run_case "$raw/facility/prefix_$mode.log" full facility_prefix "$mode"
  run_case "$raw/facility/fixed_$mode.log" full facility_fixed "$mode"
  run_case "$raw/facility/libstdcxx_$mode.log" full facility_libstdcxx "$mode"
done

identity="set +e; cd '$remote'; LD_LIBRARY_PATH='$remote/runtime/full' ldd '$remote/bin/async_fixed'; ldd_rc=\$?; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/runtime/full' '$remote/bin/async_fixed' normal; debug_rc=\$?; printf 'LDD_EXIT_CODE=%d\\nLD_DEBUG_EXIT_CODE=%d\\n' \"\$ldd_rc\" \"\$debug_rc\"; exit 0"
printf 'COMMAND=' > "$raw/identity.log"; printf '%q ' "$sdb" -s "$serial" shell "$identity" >> "$raw/identity.log"; printf '\n' >> "$raw/identity.log"
"$sdb" -s "$serial" shell "$identity" >> "$raw/identity.log" 2>&1
printf 'SDB_CARRIER_EXIT_CODE=0\n' >> "$raw/identity.log"
