#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
raw="$workspace/progress/R58/raw/x86_64"
runtime="$workspace/tmp/R58/runtime/x86_64"
baseline_lib="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
probes="$workspace/tmp/R58/probes/x86_64"
mkdir -p "$raw"/{layer,facility,async,noncancel,regression,upstream,concurrency,identity}

run_case() {
  local group=$1 name=$2 variant=$3 binary=$4 mode=${5:-ignored} seconds=${6:-12}
  local log="$raw/$group/$name.log" rc=0
  printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout %ss %q %q\n' \
    "$runtime/$variant:$baseline_lib" "$seconds" "$binary" "$mode" > "$log"
  env LD_LIBRARY_PATH="$runtime/$variant:$baseline_lib" timeout "${seconds}s" \
    "$binary" "$mode" >> "$log" 2>&1 || rc=$?
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%s\t%s\t%d\n' "$group" "$name" "$variant" "$rc"
}

printf 'group\tcase\tvariant\texit_code\n'
typed="$workspace/tmp/R52/regression/x86_64/libcxx"
run_case layer baseline_catchall baseline "$probes/catchall_baseline"
run_case layer a_catchall a "$probes/catchall_baseline"
run_case layer ab_typed ab "$typed" cancel
run_case layer abc_typed abc "$typed" cancel
run_case layer bc_typed bc "$typed" cancel
for variant in baseline a ab; do run_case layer "${variant}_cout" "$variant" "$probes/facility_baseline" cout; done
run_case layer abc_cout abc "$probes/facility_full" cout
run_case layer bc_cout bc "$probes/facility_full" cout

for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case facility "baseline_$mode" baseline "$probes/facility_baseline" "$mode"
  run_case facility "full_$mode" abc "$probes/facility_full" "$mode"
  run_case facility "reference_$mode" abc "$probes/facility_reference" "$mode"
done

for iteration in $(seq 1 20); do
  run_case async "baseline_cancel_$iteration" baseline "$probes/async_baseline" cancel
  run_case async "full_cancel_$iteration" abc "$probes/async_full" cancel
  run_case async "reference_cancel_$iteration" abc "$probes/async_reference" cancel
done

for mode in normal exception; do
  run_case noncancel "async_baseline_$mode" baseline "$probes/async_baseline" "$mode"
  run_case noncancel "async_full_$mode" abc "$probes/async_full" "$mode"
  run_case noncancel "async_reference_$mode" abc "$probes/async_reference" "$mode"
done
for mode in getline_success getline_failure packaged_exception string_normal; do
  run_case noncancel "baseline_$mode" baseline "$probes/noncancel_baseline" "$mode"
  run_case noncancel "full_$mode" abc "$probes/noncancel_full" "$mode"
  run_case noncancel "reference_$mode" abc "$probes/noncancel_reference" "$mode"
done
for mode in packaged_normal packaged_exception arithmetic_format_error; do
  run_case noncancel "baseline_$mode" baseline "$probes/normal_baseline" "$mode"
  run_case noncancel "full_$mode" abc "$probes/normal_full" "$mode"
  run_case noncancel "reference_$mode" abc "$probes/normal_reference" "$mode"
done

for runtime_name in full reference; do
  binary="$workspace/tmp/R52/regression/x86_64/$([[ $runtime_name == full ]] && echo libcxx || echo libstdcxx)"
  for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
    run_case regression "${runtime_name}_$mode" abc "$binary" "$mode" 15
  done
done
for runtime_name in libcxx libstdcxx; do
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
    run_case upstream "${runtime_name}_$test" abc "$workspace/tmp/R51/tests/x86_64/$runtime_name/$test" ignored 15
  done
done

for mode in multi_cancel cancel_and_exception stdlib_concurrent repeat_cancel; do
  for runtime_name in full reference; do
    binary="$probes/concurrency_$runtime_name"
    for iteration in $(seq 1 20); do
      run_case concurrency "${runtime_name}_${mode}_$iteration" abc "$binary" "$mode" 15
    done
  done
done

sha256sum "$runtime/abc/libc++.so.1" "$runtime/abc/libc++abi.so.1" \
  "$probes/facility_full" "$probes/async_full" > "$raw/identity/sha256.txt"
ldd "$probes/facility_full" > "$raw/identity/ldd_default.txt" 2>&1
LD_DEBUG=libs LD_LIBRARY_PATH="$runtime/abc:$baseline_lib" \
  "$probes/async_full" normal > "$raw/identity/ld_debug_full.log" 2>&1

