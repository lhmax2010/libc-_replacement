#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
runtime="$workspace/tmp/R54/runtime/x86_64/abc"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
raw="$workspace/progress/R55/raw/x86_64"
mkdir -p "$raw/regression" "$raw/upstream"

run_case() {
  local log=$1 binary=$2 arg=$3 rc=0
  printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout 15s %q %q\n' "$runtime:$baseline" "$binary" "$arg" > "$log"
  env LD_LIBRARY_PATH="$runtime:$baseline" timeout 15s "$binary" "$arg" >> "$log" 2>&1 || rc=$?
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
}

for rt in libcxx libstdcxx; do
  binary="$workspace/tmp/R52/regression/x86_64/$rt"
  for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
    run_case "$raw/regression/${rt}_$mode.log" "$binary" "$mode"
  done
done
for rt in libcxx libstdcxx; do
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
    run_case "$raw/upstream/${rt}_$test.log" "$workspace/tmp/R51/tests/x86_64/$rt/$test" ignored
  done
done
