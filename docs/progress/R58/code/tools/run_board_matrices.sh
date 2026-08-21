#!/usr/bin/env bash
set -u -o pipefail
workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r58_20260821
raw="$workspace/progress/R58/raw/armv7l"
mkdir -p "$raw"/{layer,facility,async,noncancel,regression,upstream,concurrency,identity}
run_case() {
  local group=$1 name=$2 variant=$3 binary=$4 mode=${5:-ignored} seconds=${6:-12}
  local log="$raw/$group/$name.log" cmd carrier=0 remote_rc
  cmd="set +e; cd '$remote'; LD_LIBRARY_PATH='$remote/runtime/$variant' '$remote/$binary' '$mode' & pid=\$!; (sleep $seconds; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
  printf 'COMMAND=' > "$log"; printf '%q ' "$sdb" -s "$serial" shell "$cmd" >> "$log"; printf '\n' >> "$log"
  "$sdb" -s "$serial" shell "$cmd" >> "$log" 2>&1 || carrier=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier" >> "$log"
  ((carrier == 0)) || return 90
  remote_rc=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{v=$2} END{gsub(/[^0-9]/,"",v); print v}' "$log")
  [[ $remote_rc =~ ^[0-9]+$ ]] || return 91
  printf '%s\t%s\t%s\t%s\n' "$group" "$name" "$variant" "$remote_rc"
}
printf 'group\tcase\tvariant\texit_code\n'
run_case layer baseline_catchall baseline bin/catchall_baseline ignored
run_case layer a_catchall a bin/catchall_baseline ignored
run_case layer ab_typed ab bin/abi_full cancel
run_case layer abc_typed abc bin/abi_full cancel
run_case layer bc_typed bc bin/abi_full cancel
for v in baseline a ab; do run_case layer "${v}_cout" "$v" bin/facility_baseline cout; done
run_case layer abc_cout abc bin/facility_full cout
run_case layer bc_cout bc bin/facility_full cout
for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case facility "baseline_$mode" baseline bin/facility_baseline "$mode"
  run_case facility "full_$mode" abc bin/facility_full "$mode"
  run_case facility "reference_$mode" abc bin/facility_reference "$mode"
done
for i in $(seq 1 20); do
  run_case async "baseline_cancel_$i" baseline bin/async_baseline cancel
  run_case async "full_cancel_$i" abc bin/async_full cancel
  run_case async "reference_cancel_$i" abc bin/async_reference cancel
done
for mode in normal exception; do
  run_case noncancel "async_baseline_$mode" baseline bin/async_baseline "$mode"
  run_case noncancel "async_full_$mode" abc bin/async_full "$mode"
  run_case noncancel "async_reference_$mode" abc bin/async_reference "$mode"
done
for mode in packaged_normal packaged_exception arithmetic_format_error; do
  run_case noncancel "baseline_$mode" baseline bin/normal_baseline "$mode"
  run_case noncancel "full_$mode" abc bin/normal_full "$mode"
  run_case noncancel "reference_$mode" abc bin/normal_reference "$mode"
done
for rt in full reference; do
  for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do run_case regression "${rt}_$mode" abc "bin/abi_$rt" "$mode"; done
done
for rt in libcxx libstdcxx; do
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do run_case upstream "${rt}_$test" abc "upstream/$rt/$test" ignored; done
done
for mode in multi_cancel cancel_and_exception stdlib_concurrent repeat_cancel; do
  for rt in full reference; do
    for i in $(seq 1 20); do run_case concurrency "${rt}_${mode}_$i" abc "bin/concurrency_$rt" "$mode" 15; done
  done
done
identity="cd '$remote'; sha256sum runtime/abc/libc++.so.1 runtime/abc/libc++abi.so.1 bin/facility_full bin/async_full; LD_LIBRARY_PATH='$remote/runtime/abc' ldd bin/async_full; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/runtime/abc' bin/normal_full packaged_normal"
printf 'COMMAND=' > "$raw/identity/runtime.log"; printf '%q ' "$sdb" -s "$serial" shell "$identity" >> "$raw/identity/runtime.log"; printf '\n' >> "$raw/identity/runtime.log"
"$sdb" -s "$serial" shell "$identity" >> "$raw/identity/runtime.log" 2>&1

