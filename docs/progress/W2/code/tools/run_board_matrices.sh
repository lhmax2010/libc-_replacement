#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/w2_r62_20260821
raw="$workspace/progress/W2/raw/board"
mkdir -p "$raw"/{directed,concurrency,facility,regression,upstream,noncancel,identity}

run_case() {
  local group=$1 name=$2 runtime=$3 binary=$4 mode=${5:-} seconds=${6:-15}
  local log="$raw/$group/$name.log" cmd carrier=0 remote_rc libpath
  if [[ $runtime == fixed ]]; then
    libpath="LD_LIBRARY_PATH='$remote/lib'"
  else
    libpath='env'
  fi
  cmd="set +e; cd '$remote'; $libpath '$remote/$binary' '$mode' & pid=\$!; (sleep $seconds; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
  printf 'COMMAND=' > "$log"
  printf '%q ' "$sdb" -s "$serial" shell "$cmd" >> "$log"
  printf '\n' >> "$log"
  "$sdb" -s "$serial" shell "$cmd" >> "$log" 2>&1 || carrier=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier" >> "$log"
  ((carrier == 0)) || return 90
  remote_rc=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{v=$2} END{gsub(/[^0-9]/,"",v); print v}' "$log")
  [[ $remote_rc =~ ^[0-9]+$ ]] || return 91
  printf '%s\t%s\t%s\t%s\n' "$group" "$name" "$runtime" "$remote_rc"
}

printf 'group\tcase\truntime\texit_code\n'

for mode in f1 f2 m1 m2; do
  run_case directed "fixed_$mode" fixed bin/r61_fixed "$mode"
  run_case directed "reference_$mode" reference bin/r61_reference "$mode"
done
for mode in m2 wait_for void exception; do
  run_case directed "m2_fixed_$mode" fixed bin/m2_fixed "$mode"
  run_case directed "m2_reference_$mode" reference bin/m2_reference "$mode"
done

for iteration in $(seq 1 20); do
  run_case concurrency "deferred_fixed_$iteration" fixed bin/m1_concurrency_fixed '' 20
  run_case concurrency "deferred_reference_$iteration" reference bin/m1_concurrency_reference '' 20
  run_case concurrency "async_fixed_$iteration" fixed bin/m2_fixed multi_waiters 20
  run_case concurrency "async_reference_$iteration" reference bin/m2_reference multi_waiters 20
done

for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case facility "fixed_$mode" fixed bin/facility_fixed "$mode" 20
  run_case facility "reference_$mode" reference bin/facility_reference "$mode" 20
done

for runtime in fixed reference; do
  for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
    run_case regression "${runtime}_$mode" "$runtime" "regression/$([[ $runtime == fixed ]] && printf libcxx || printf libstdcxx)" "$mode" 20
  done
done

for runtime in fixed reference; do
  suite=$([[ $runtime == fixed ]] && printf libcxx || printf libstdcxx)
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
    run_case upstream "${runtime}_$test" "$runtime" "upstream/$suite/$test" '' 20
  done
done

for mode in normal exception; do
  run_case noncancel "async_fixed_$mode" fixed bin/async_fixed "$mode"
  run_case noncancel "async_reference_$mode" reference bin/async_reference "$mode"
done
for mode in getline_success getline_failure packaged_exception string_normal; do
  run_case noncancel "fixed_$mode" fixed bin/noncancel_fixed "$mode"
  run_case noncancel "reference_$mode" reference bin/noncancel_reference "$mode"
done
for mode in packaged_normal packaged_exception arithmetic_format_error; do
  run_case noncancel "normal_fixed_$mode" fixed bin/normal_fixed "$mode"
  run_case noncancel "normal_reference_$mode" reference bin/normal_reference "$mode"
done

identity="set +e; cd '$remote'; sha256sum lib/libc++.so.1 lib/libc++abi.so.1 bin/r61_fixed bin/m2_fixed; echo LDD_FIXED; LD_LIBRARY_PATH='$remote/lib' ldd bin/m2_fixed; echo LDD_REFERENCE; ldd bin/m2_reference; echo LD_DEBUG_FIXED; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/lib' bin/m2_fixed exception; echo LD_DEBUG_EXIT=\$?; exit 0"
identity_log="$raw/identity/runtime.log"
printf 'COMMAND=' > "$identity_log"
printf '%q ' "$sdb" -s "$serial" shell "$identity" >> "$identity_log"
printf '\n' >> "$identity_log"
carrier=0
"$sdb" -s "$serial" shell "$identity" >> "$identity_log" 2>&1 || carrier=$?
printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier" >> "$identity_log"
((carrier == 0)) || exit 90
