#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r64_20260825
raw="$workspace/progress/R64/raw/arm_deferred_matrix"
summary="$workspace/progress/R64/tables/arm_deferred_matrix.tsv"
mkdir -p "$raw" "$(dirname "$summary")"
printf 'runtime\tmode\titeration\tremote_exit\tsdb_exit\tlog\n' > "$summary"

run_case() {
  local runtime=$1 mode=$2 iteration=$3 binary lib_prefix log command carrier=0 remote_rc
  if [[ $runtime == fixed ]]; then
    binary="$remote/deferred/m2_deferred_fixed"
    lib_prefix="LD_LIBRARY_PATH='$remote/original/lib'"
  else
    binary="$remote/deferred/m2_deferred_reference"
    lib_prefix=env
  fi
  log="$raw/${runtime}_${mode}_${iteration}.log"
  command="set +e; $lib_prefix '$binary' '$mode' & pid=\$!; (sleep 12; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
  printf 'COMMAND=' > "$log"
  printf '%q ' "$sdb" -s "$serial" shell "$command" >> "$log"
  printf '\n' >> "$log"
  "$sdb" -s "$serial" shell "$command" >> "$log" 2>&1 || carrier=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier" >> "$log"
  (( carrier == 0 )) || return 90
  remote_rc=$(awk -F= '/REMOTE_CELL_EXIT_CODE=/{value=$2} END{gsub(/[^0-9]/,"",value); print value}' "$log")
  [[ $remote_rc =~ ^[0-9]+$ ]] || return 91
  printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$runtime" "$mode" "$iteration" "$remote_rc" "$carrier" "$log" >> "$summary"
}

for runtime in fixed reference; do
  for mode in m2 wait_for multi_waiters; do
    for iteration in $(seq 1 20); do
      run_case "$runtime" "$mode" "$iteration"
    done
  done
done

awk -F '\t' 'NR > 1 {key=$1 FS $2; total[key]++; if ($4 == 0) pass[key]++} END {for (key in total) print key, pass[key]+0, total[key]}' OFS='\t' "$summary" | sort
