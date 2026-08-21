#!/usr/bin/env bash
set -u -o pipefail
workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r54_20260821
raw="$workspace/progress/R54/raw/armv7l"
mkdir -p "$raw/layer" "$raw/facility" "$raw/concurrency" "$raw/regression" "$raw/upstream"
run_case() {
  local group=$1 name=$2 variant=$3 binary=$4 mode=${5:-}
  local log="$raw/$group/$name.log" cmd carrier remote_rc
  cmd="set +e; cd '$remote'; LD_LIBRARY_PATH='$remote/runtime/$variant' '$remote/$binary' '$mode' & pid=\$!; (sleep 6; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
  printf 'COMMAND=' > "$log"; printf '%q ' "$sdb" -s "$serial" shell "$cmd" >> "$log"; printf '\n' >> "$log"
  set +e
  "$sdb" -s "$serial" shell "$cmd" >> "$log" 2>&1
  carrier=$?
  set -e
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier" >> "$log"
  (( carrier == 0 )) || return 90
  remote_rc=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{value=$2} END{gsub(/[^0-9]/, "", value); print value}' "$log")
  [[ $remote_rc =~ ^[0-9]+$ ]] || return 91
  printf '%s\t%s\t%s\t%d\n' "$group" "$name" "$variant" "$remote_rc"
}
printf 'group\tcase\tvariant\texit_code\n'
run_case layer baseline_catchall baseline bin/catchall ignored
run_case layer a_catchall a bin/catchall ignored
run_case layer ab_typed ab bin/abi_full cancel
run_case layer abc_typed abc bin/abi_full cancel
run_case layer bc_typed bc bin/abi_full cancel
for variant in baseline a ab; do run_case layer "${variant}_cout" "$variant" bin/facility_baseline cout; done
run_case layer abc_cout abc bin/facility_full cout
run_case layer bc_cout bc bin/facility_full cout
for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case facility "baseline_$mode" baseline bin/facility_baseline "$mode"
  run_case facility "full_$mode" abc bin/facility_full "$mode"
  run_case facility "reference_$mode" abc bin/facility_reference "$mode"
done
for mode in multi_cancel cancel_and_exception stdlib_concurrent repeat_cancel; do
  for runtime_name in full reference; do
    for iteration in $(seq 1 20); do
      run_case concurrency "${runtime_name}_${mode}_${iteration}" abc "bin/concurrency_$runtime_name" "$mode"
    done
  done
done
for runtime_name in full reference; do
  binary=bin/abi_$runtime_name
  for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
    run_case regression "${runtime_name}_$mode" abc "$binary" "$mode"
  done
done
for runtime_name in libcxx libstdcxx; do
  variant=abc
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
    run_case upstream "${runtime_name}_$test" "$variant" "upstream/$runtime_name/$test" ignored
  done
done
