#!/usr/bin/env bash
set -u -o pipefail

workspace=/home/toolchain/development/libc++_replacement
bin="$workspace/tmp/R60/probes/x86_64"
libs="$workspace/tmp/R60/build-x86_64/lib"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
raw="$workspace/progress/R60/raw/matrix/x86_64"
mkdir -p "$raw"

run_case() {
  local name=$1 binary=$2 mode=${3:-} path=${4:-} rc=0
  local log="$raw/$name.log"
  if [[ -n $path ]]; then
    printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout 12s %q %q\n' "$path" "$binary" "$mode" > "$log"
    env LD_LIBRARY_PATH="$path" timeout 12s "$binary" ${mode:+"$mode"} >> "$log" 2>&1 || rc=$?
  else
    printf 'COMMAND=timeout 12s %q %q\n' "$binary" "$mode" > "$log"
    timeout 12s "$binary" ${mode:+"$mode"} >> "$log" 2>&1 || rc=$?
  fi
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%d\n' "$name" "$rc"
}

printf 'case\texit_code\n' > "$raw/results.tsv"
for mode in f1 f2 m1 m2; do
  run_case "${mode}_current" "$bin/r60_current" "$mode" "$libs:$baseline" >> "$raw/results.tsv"
  run_case "${mode}_reference" "$bin/r60_reference" "$mode" >> "$raw/results.tsv"
done
for iteration in $(seq 1 20); do
  run_case "m1_concurrency_current_${iteration}" "$bin/m1_concurrency_current" '' "$libs:$baseline" >> "$raw/results.tsv"
  run_case "m1_concurrency_reference_${iteration}" "$bin/m1_concurrency_reference" >> "$raw/results.tsv"
done

sha256sum "$libs/libc++.so.1" "$libs/libc++abi.so.1" "$bin"/* > "$raw/sha256.txt"
LD_LIBRARY_PATH="$libs:$baseline" ldd "$bin/r60_current" > "$raw/ldd_current.txt" 2>&1
LD_DEBUG=libs LD_LIBRARY_PATH="$libs:$baseline" "$bin/r60_current" f2 > "$raw/ld_debug_current.log" 2>&1 || true
