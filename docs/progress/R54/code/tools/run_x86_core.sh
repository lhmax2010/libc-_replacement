#!/usr/bin/env bash
set -u -o pipefail
workspace=/home/toolchain/development/libc++_replacement
raw="$workspace/progress/R54/raw/x86_64"
mkdir -p "$raw/layer" "$raw/facility" "$raw/concurrency"
runtime="$workspace/tmp/R54/runtime/x86_64"
baseline_lib="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
probe_base="$workspace/tmp/R54/probes/x86_64/baseline/facility_probe_libcxx"
probe_full="$workspace/tmp/R54/probes/x86_64/full/facility_probe_libcxx"
abi_probe="$workspace/tmp/R52/regression/x86_64/libcxx"
catchall_probe="$workspace/tmp/R54/probes/x86_64/baseline/catchall_rethrow_probe_libcxx"
run_case() {
  local group=$1 name=$2 variant=$3 binary=$4 mode=$5
  local log="$raw/$group/$name.log"
  printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout 12s %q %q\n' "$runtime/$variant:$baseline_lib" "$binary" "$mode" > "$log"
  set +e
  env LD_LIBRARY_PATH="$runtime/$variant:$baseline_lib" timeout 12s "$binary" "$mode" >> "$log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%s\t%s\t%d\n' "$group" "$name" "$variant" "$rc"
}
printf 'group\tcase\tvariant\texit_code\n'
run_case layer baseline_catchall baseline "$catchall_probe" ignored
run_case layer a_catchall a "$catchall_probe" ignored
run_case layer ab_typed ab "$abi_probe" cancel
run_case layer abc_typed abc "$abi_probe" cancel
run_case layer bc_typed bc "$abi_probe" cancel
for variant in baseline a ab; do
  run_case layer "${variant}_cout" "$variant" "$probe_base" cout
done
run_case layer abc_cout abc "$probe_full" cout
run_case layer bc_cout bc "$probe_full" cout
for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case facility "baseline_$mode" baseline "$probe_base" "$mode"
  run_case facility "full_$mode" abc "$probe_full" "$mode"
  run_case facility "libstdcxx_$mode" abc "$workspace/tmp/R54/probes/x86_64/full/facility_probe_libstdcxx" "$mode"
done
for mode in multi_cancel cancel_and_exception stdlib_concurrent repeat_cancel; do
  for runtime_name in full libstdcxx; do
    if [[ $runtime_name == full ]]; then
      binary="$workspace/tmp/R54/probes/x86_64/full/concurrency_probe_libcxx"
    else
      binary="$workspace/tmp/R54/probes/x86_64/full/concurrency_probe_libstdcxx"
    fi
    for iteration in $(seq 1 20); do
      run_case concurrency "${runtime_name}_${mode}_${iteration}" abc "$binary" "$mode"
    done
  done
done
