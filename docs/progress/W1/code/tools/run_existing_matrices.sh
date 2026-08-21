#!/usr/bin/env bash
set -u -o pipefail

workspace=/home/toolchain/development/libc++_replacement
raw="$workspace/progress/W1/raw/existing_matrices"
fixed_libs="$workspace/tmp/W1/build-x86_64/lib"
baseline_libs="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
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

printf 'group\tcase\texit_code\n' > "$raw/results.tsv"
for mode in cout getline_free getline_member read arithmetic packaged_task async; do
  run_case facility "baseline_$mode" "$workspace/tmp/R58/probes/x86_64/facility_baseline" "$mode" "$baseline_libs" \
    >> "$raw/results.tsv"
  run_case facility "fixed_$mode" "$workspace/tmp/W1/probes/x86_64/facility_fixed" "$mode" "$fixed_libs:$baseline_libs" \
    >> "$raw/results.tsv"
  run_case facility "reference_$mode" "$workspace/tmp/R58/probes/x86_64/facility_reference" "$mode" \
    >> "$raw/results.tsv"
done

for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
  run_case regression "fixed_$mode" "$workspace/tmp/R52/regression/x86_64/libcxx" "$mode" "$fixed_libs:$baseline_libs" \
    >> "$raw/results.tsv"
  run_case regression "reference_$mode" "$workspace/tmp/R52/regression/x86_64/libstdcxx" "$mode" \
    >> "$raw/results.tsv"
done

for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
  run_case upstream "libcxx_$test" "$workspace/tmp/R51/tests/x86_64/libcxx/$test" '' "$fixed_libs:$baseline_libs" \
    >> "$raw/results.tsv"
  run_case upstream "libstdcxx_$test" "$workspace/tmp/R51/tests/x86_64/libstdcxx/$test" \
    >> "$raw/results.tsv"
done

exit 0
