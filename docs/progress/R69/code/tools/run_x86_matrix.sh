#!/usr/bin/env bash
set -u -o pipefail

workspace=/home/toolchain/development/libc++_replacement
bin="$workspace/tmp/R69/probes/x86_64"
fixed_libs="$workspace/tmp/R69/build-x86_64/lib"
baseline_libs="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
raw="$workspace/progress/R69/raw/x86_64/matrix"
mkdir -p "$raw"

run_case() {
  local group=$1 name=$2 binary=$3 mode=${4:-} library_path=${5:-} seconds=${6:-15} rc=0
  local log="$raw/${group}_${name}.log"
  if [[ -n $library_path ]]; then
    printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout %ss %q %q\n' "$library_path" "$seconds" "$binary" "$mode" > "$log"
    env LD_LIBRARY_PATH="$library_path" timeout "${seconds}s" "$binary" ${mode:+"$mode"} >> "$log" 2>&1 || rc=$?
  else
    printf 'COMMAND=timeout %ss %q %q\n' "$seconds" "$binary" "$mode" > "$log"
    timeout "${seconds}s" "$binary" ${mode:+"$mode"} >> "$log" 2>&1 || rc=$?
  fi
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%s\t%d\n' "$group" "$name" "$rc"
}

printf 'group\tcase\texit_code\n'
for mode in f1 f2 m1; do
  run_case directed "fixed_$mode" "$bin/r61_fixed" "$mode" "$fixed_libs:$baseline_libs"
  run_case directed "reference_$mode" "$bin/r61_reference" "$mode"
done
run_case new_cases baseline_arithmetic_state "$bin/r69_new_baseline" arithmetic-state "$baseline_libs"
run_case new_cases fixed_arithmetic_state "$bin/r69_new_fixed" arithmetic-state "$fixed_libs:$baseline_libs"
run_case new_cases reference_arithmetic_state "$bin/r69_new_reference" arithmetic-state
for mode in deferred-recancel waiting-side-cancel; do
  for iteration in $(seq 1 20); do
    normalized=${mode//-/_}
    run_case new_cases "fixed_${normalized}_$iteration" "$bin/r69_new_fixed" "$mode" "$fixed_libs:$baseline_libs"
    run_case new_cases "reference_${normalized}_$iteration" "$bin/r69_new_reference" "$mode"
  done
done
for iteration in $(seq 1 20); do
  run_case m2_lock "fixed_$iteration" "$bin/m2_lock_fixed" '' "$fixed_libs:$baseline_libs"
  run_case m2_lock "reference_$iteration" "$bin/m2_lock_reference"
  run_case m1_concurrency "fixed_$iteration" "$bin/m1_concurrency_fixed" '' "$fixed_libs:$baseline_libs"
  run_case m1_concurrency "reference_$iteration" "$bin/m1_concurrency_reference"
done
run_case m1_reference fixed "$bin/async_reference_return_fixed" '' "$fixed_libs:$baseline_libs"
run_case m1_reference reference "$bin/async_reference_return_reference"

for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case facility "baseline_$mode" "$bin/facility_baseline" "$mode" "$baseline_libs"
  run_case facility "fixed_$mode" "$bin/facility_fixed" "$mode" "$fixed_libs:$baseline_libs"
  run_case facility "reference_$mode" "$bin/facility_reference" "$mode"
done

for iteration in $(seq 1 20); do
  run_case async "baseline_cancel_$iteration" "$bin/async_baseline" cancel "$baseline_libs"
  run_case async "fixed_cancel_$iteration" "$bin/async_fixed" cancel "$fixed_libs:$baseline_libs"
  run_case async "reference_cancel_$iteration" "$bin/async_reference" cancel
done
for mode in normal exception; do
  run_case async_noncancel "baseline_$mode" "$bin/async_baseline" "$mode" "$baseline_libs"
  run_case async_noncancel "fixed_$mode" "$bin/async_fixed" "$mode" "$fixed_libs:$baseline_libs"
  run_case async_noncancel "reference_$mode" "$bin/async_reference" "$mode"
done

for mode in packaged_normal packaged_exception arithmetic_format_error; do
  run_case normal "baseline_$mode" "$bin/normal_baseline" "$mode" "$baseline_libs"
  run_case normal "fixed_$mode" "$bin/normal_fixed" "$mode" "$fixed_libs:$baseline_libs"
  run_case normal "reference_$mode" "$bin/normal_reference" "$mode"
done

for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
  run_case regression "fixed_$mode" "$bin/regression_fixed" "$mode" "$fixed_libs:$baseline_libs"
  run_case regression "reference_$mode" "$bin/regression_reference" "$mode"
done

for mode in multi_cancel cancel_and_exception stdlib_concurrent repeat_cancel; do
  for iteration in $(seq 1 20); do
    run_case concurrency "fixed_${mode}_$iteration" "$bin/concurrency_fixed" "$mode" "$fixed_libs:$baseline_libs"
    run_case concurrency "reference_${mode}_$iteration" "$bin/concurrency_reference" "$mode"
  done
done

sha256sum "$fixed_libs/libc++.so.1" "$fixed_libs/libc++abi.so.1" "$bin/facility_fixed" "$bin/m2_lock_fixed" \
  > "$raw/runtime_sha256.txt"
ldd "$bin/facility_fixed" > "$raw/ldd_default.txt" 2>&1
LD_DEBUG=libs LD_LIBRARY_PATH="$fixed_libs:$baseline_libs" "$bin/async_fixed" normal \
  > "$raw/ld_debug_fixed.log" 2>&1
exit 0
