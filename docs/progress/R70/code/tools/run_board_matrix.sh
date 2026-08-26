#!/usr/bin/env bash
set -u -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r70_20260826
raw="$workspace/progress/R70/raw/armv7l/matrix"
mkdir -p "$raw"/{directed,new_cases,m2_lock,m1_concurrency,m1_reference,facility,async,async_noncancel,normal,regression,concurrency,upstream,identity}

run_case() {
  local group=$1 name=$2 variant=$3 binary=$4 mode=${5:-ignored} seconds=${6:-15}
  local log="$raw/$group/$name.log" cmd carrier=0 remote_rc
  local prefix=""
  [[ $variant == fixed || $variant == baseline ]] && prefix="LD_LIBRARY_PATH='$remote/runtime/$variant' "
  cmd="set +e; cd '$remote'; ${prefix}'$remote/$binary' '$mode' & pid=\$!; (sleep $seconds; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
  printf 'COMMAND=' > "$log"; printf '%q ' "$sdb" -s "$serial" shell "$cmd" >> "$log"; printf '\n' >> "$log"
  "$sdb" -s "$serial" shell "$cmd" >> "$log" 2>&1 || carrier=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier" >> "$log"
  ((carrier == 0)) || return 90
  remote_rc=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{v=$2} END{gsub(/[^0-9]/,"",v); print v}' "$log")
  [[ $remote_rc =~ ^[0-9]+$ ]] || return 91
  printf '%s\t%s\t%s\t%s\n' "$group" "$name" "$variant" "$remote_rc"
}

printf 'group\tcase\tvariant\texit_code\n'
for mode in f1 f2 m1; do
  run_case directed "fixed_$mode" fixed bin/r61_fixed "$mode"
  run_case directed "reference_$mode" reference bin/r61_reference "$mode"
done
run_case new_cases baseline_arithmetic_state baseline bin/r69_new_baseline arithmetic-state
run_case new_cases fixed_arithmetic_state fixed bin/r69_new_fixed arithmetic-state
run_case new_cases reference_arithmetic_state reference bin/r69_new_reference arithmetic-state
for mode in deferred-recancel waiting-side-cancel; do
  for iteration in $(seq 1 20); do
    normalized=${mode//-/_}
    run_case new_cases "fixed_${normalized}_$iteration" fixed bin/r69_new_fixed "$mode"
    run_case new_cases "reference_${normalized}_$iteration" reference bin/r69_new_reference "$mode"
  done
done
for iteration in $(seq 1 20); do
  run_case new_cases "baseline_waiting_side_cancel_$iteration" baseline bin/r69_new_baseline waiting-side-cancel
done
for iteration in $(seq 1 20); do
  run_case m2_lock "fixed_$iteration" fixed bin/m2_lock_fixed ignored
  run_case m2_lock "reference_$iteration" reference bin/m2_lock_reference ignored
  run_case m1_concurrency "fixed_$iteration" fixed bin/m1_concurrency_fixed ignored
  run_case m1_concurrency "reference_$iteration" reference bin/m1_concurrency_reference ignored
done
run_case m1_reference fixed fixed bin/async_reference_return_fixed ignored
run_case m1_reference reference reference bin/async_reference_return_reference ignored

for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case facility "baseline_$mode" baseline bin/facility_baseline "$mode"
  run_case facility "fixed_$mode" fixed bin/facility_fixed "$mode"
  run_case facility "reference_$mode" reference bin/facility_reference "$mode"
done
for iteration in $(seq 1 20); do
  run_case async "baseline_cancel_$iteration" baseline bin/async_baseline cancel
  run_case async "fixed_cancel_$iteration" fixed bin/async_fixed cancel
  run_case async "reference_cancel_$iteration" reference bin/async_reference cancel
done
for mode in normal exception; do
  run_case async_noncancel "baseline_$mode" baseline bin/async_baseline "$mode"
  run_case async_noncancel "fixed_$mode" fixed bin/async_fixed "$mode"
  run_case async_noncancel "reference_$mode" reference bin/async_reference "$mode"
done
for mode in packaged_normal packaged_exception arithmetic_format_error; do
  run_case normal "baseline_$mode" baseline bin/normal_baseline "$mode"
  run_case normal "fixed_$mode" fixed bin/normal_fixed "$mode"
  run_case normal "reference_$mode" reference bin/normal_reference "$mode"
done
for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
  run_case regression "fixed_$mode" fixed bin/regression_fixed "$mode"
  run_case regression "reference_$mode" reference bin/regression_reference "$mode"
done
for mode in multi_cancel cancel_and_exception stdlib_concurrent repeat_cancel; do
  for iteration in $(seq 1 20); do
    run_case concurrency "fixed_${mode}_$iteration" fixed bin/concurrency_fixed "$mode" 15
    run_case concurrency "reference_${mode}_$iteration" reference bin/concurrency_reference "$mode" 15
  done
done
for runtime in libcxxabi libstdcxx; do
  variant=fixed
  [[ $runtime == libstdcxx ]] && variant=reference
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
    run_case upstream "${runtime}_$test" "$variant" "upstream/$runtime/$test" ignored
  done
done
for test in future_async future_async_race future_async_race_38682 future_get_future future_task_operator future_thread_create_failure future_wait_on_destruct istream_getline istream_read istream_ws string_shrink_to_fit; do
  run_case upstream "libcxx_$test" fixed "upstream/libcxx/$test" ignored 30
done

identity="cd '$remote'; sha256sum runtime/fixed/libc++.so.1 runtime/fixed/libc++abi.so.1 bin/facility_fixed bin/m2_lock_fixed; LD_LIBRARY_PATH='$remote/runtime/fixed' ldd bin/async_fixed; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/runtime/fixed' bin/normal_fixed packaged_normal"
printf 'COMMAND=' > "$raw/identity/runtime.log"; printf '%q ' "$sdb" -s "$serial" shell "$identity" >> "$raw/identity/runtime.log"; printf '\n' >> "$raw/identity/runtime.log"
"$sdb" -s "$serial" shell "$identity" >> "$raw/identity/runtime.log" 2>&1
