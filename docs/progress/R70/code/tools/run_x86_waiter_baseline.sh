#!/usr/bin/env bash
set -u -o pipefail

workspace=/home/toolchain/development/libc++_replacement
binary="$workspace/tmp/R69/probes/x86_64/r69_new_baseline"
baseline_libs="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
raw="$workspace/progress/R70/raw/x86_64/waiter_baseline"
mkdir -p "$raw"

printf 'iteration\texit_code\n'
for iteration in $(seq 1 20); do
  log="$raw/baseline_waiting_side_cancel_${iteration}.log"
  rc=0
  printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout 15s %q waiting-side-cancel\n' \
    "$baseline_libs" "$binary" >"$log"
  env LD_LIBRARY_PATH="$baseline_libs" timeout 15s "$binary" waiting-side-cancel \
    >>"$log" 2>&1 || rc=$?
  printf 'EXIT_CODE=%d\n' "$rc" >>"$log"
  printf '%d\t%d\n' "$iteration" "$rc"
done
