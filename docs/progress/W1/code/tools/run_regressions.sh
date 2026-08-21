#!/usr/bin/env bash
set -u -o pipefail

workspace=/home/toolchain/development/libc++_replacement
bin="$workspace/tmp/W1/probes/x86_64"
fixed_libs="$workspace/tmp/W1/build-x86_64/lib"
baseline_libs="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
raw="$workspace/progress/W1/raw/regressions"
mkdir -p "$raw"

run_case() {
  local name=$1 binary=$2 mode=$3 library_path=${4:-} rc=0
  local log="$raw/$name.log"
  if [[ -n $library_path ]]; then
    printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout 12s %q %q\n' "$library_path" "$binary" "$mode" > "$log"
    env LD_LIBRARY_PATH="$library_path" timeout 12s "$binary" "$mode" >> "$log" 2>&1 || rc=$?
  else
    printf 'COMMAND=timeout 12s %q %q\n' "$binary" "$mode" > "$log"
    timeout 12s "$binary" "$mode" >> "$log" 2>&1 || rc=$?
  fi
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%s\t%d\n' "$name" "$mode" "$rc"
}

printf 'case\tmode\texit_code\n' > "$raw/results.tsv"
for mode in f1 f2 m1; do
  for iteration in $(seq 1 20); do
    run_case "fixed_${mode}_${iteration}" "$bin/r61_fixed" "$mode" "$fixed_libs:$baseline_libs" >> "$raw/results.tsv"
    run_case "reference_${mode}_${iteration}" "$bin/r61_reference" "$mode" >> "$raw/results.tsv"
  done
done

exit 0
