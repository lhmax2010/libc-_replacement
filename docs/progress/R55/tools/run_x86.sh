#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
raw="$workspace/progress/R55/raw/x86_64"
probes="$workspace/tmp/R55/probes/x86_64"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
runtime_full="$workspace/tmp/R54/runtime/x86_64/abc:$baseline"
runtime_baseline="$workspace/tmp/R54/runtime/x86_64/baseline:$baseline"
mkdir -p "$raw/async20" "$raw/facility" "$raw/noncancel"

run_case() {
  local log=$1
  local library_path=$2
  shift 2
  {
    printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout 4s ' "$library_path"
    printf '%q ' "$@"
    printf '\n'
  } > "$log"
  local rc=0
  env LD_LIBRARY_PATH="$library_path" timeout 4s "$@" >> "$log" 2>&1 || rc=$?
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  return 0
}

for iteration in $(seq 1 20); do
  run_case "$raw/async20/baseline_$iteration.log" "$runtime_baseline" \
    "$probes/async_probe_baseline_libcxx" cancel
  run_case "$raw/async20/prefix_$iteration.log" "$runtime_full" \
    "$probes/async_probe_prefix_libcxx" cancel
  run_case "$raw/async20/fixed_$iteration.log" "$runtime_full" \
    "$probes/async_probe_fixed_libcxx" cancel
  run_case "$raw/async20/libstdcxx_$iteration.log" "$runtime_full" \
    "$probes/async_probe_libstdcxx" cancel
done

for mode in normal exception; do
  run_case "$raw/noncancel/baseline_$mode.log" "$runtime_baseline" \
    "$probes/async_probe_baseline_libcxx" "$mode"
  run_case "$raw/noncancel/prefix_$mode.log" "$runtime_full" \
    "$probes/async_probe_prefix_libcxx" "$mode"
  run_case "$raw/noncancel/fixed_$mode.log" "$runtime_full" \
    "$probes/async_probe_fixed_libcxx" "$mode"
  run_case "$raw/noncancel/libstdcxx_$mode.log" "$runtime_full" \
    "$probes/async_probe_libstdcxx" "$mode"
done

for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case "$raw/facility/baseline_$mode.log" "$runtime_baseline" \
    "$probes/facility_probe_baseline_libcxx" "$mode"
  run_case "$raw/facility/prefix_$mode.log" "$runtime_full" \
    "$workspace/tmp/R54/probes/x86_64/full/facility_probe_libcxx" "$mode"
  run_case "$raw/facility/fixed_$mode.log" "$runtime_full" \
    "$probes/facility_probe_fixed_libcxx" "$mode"
  run_case "$raw/facility/libstdcxx_$mode.log" "$runtime_full" \
    "$probes/facility_probe_libstdcxx" "$mode"
done

run_case "$raw/ldd_fixed.log" "$runtime_full" ldd "$probes/async_probe_fixed_libcxx"
run_case "$raw/ld_debug_fixed.log" "$runtime_full" env LD_DEBUG=libs \
  "$probes/async_probe_fixed_libcxx" normal
