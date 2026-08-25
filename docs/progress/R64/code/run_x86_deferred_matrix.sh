#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
raw="$workspace/progress/R64/raw/x86_deferred_matrix"
summary="$workspace/progress/R64/tables/x86_deferred_matrix.tsv"
fixed="$workspace/tmp/R64/probes/x86_64/m2_deferred_fixed"
reference="$workspace/tmp/R64/probes/x86_64/m2_deferred_reference"
fixed_lib="$workspace/tmp/W1/build-x86_64/lib"
baseline_lib="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
mkdir -p "$raw" "$(dirname "$summary")"
printf 'runtime\tmode\titeration\texit_code\tlog\n' > "$summary"

run_case() {
  local runtime=$1 mode=$2 iteration=$3 binary log rc=0
  if [[ $runtime == fixed ]]; then
    binary=$fixed
  else
    binary=$reference
  fi
  log="$raw/${runtime}_${mode}_${iteration}.log"
  printf 'COMMAND=' > "$log"
  if [[ $runtime == fixed ]]; then
    printf 'LD_LIBRARY_PATH=%q timeout 12s %q %q\n' "$fixed_lib:$baseline_lib" "$binary" "$mode" >> "$log"
    LD_LIBRARY_PATH="$fixed_lib:$baseline_lib" timeout 12s "$binary" "$mode" >> "$log" 2>&1 || rc=$?
  else
    printf 'timeout 12s %q %q\n' "$binary" "$mode" >> "$log"
    timeout 12s "$binary" "$mode" >> "$log" 2>&1 || rc=$?
  fi
  printf 'CELL_EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%s\t%s\t%s\t%s\n' "$runtime" "$mode" "$iteration" "$rc" "$log" >> "$summary"
}

for runtime in fixed reference; do
  for mode in m2 wait_for multi_waiters; do
    for iteration in $(seq 1 20); do
      run_case "$runtime" "$mode" "$iteration"
    done
  done
done

awk -F '\t' 'NR > 1 {key=$1 FS $2; total[key]++; if ($4 == 0) pass[key]++} END {for (key in total) print key, pass[key]+0, total[key]}' OFS='\t' "$summary" | sort
