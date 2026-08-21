#!/usr/bin/env bash
set -u -o pipefail
workspace=/home/toolchain/development/libc++_replacement
runtime="$workspace/tmp/R54/runtime/x86_64/abc"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
raw="$workspace/progress/R54/raw/x86_64"
mkdir -p "$raw/regression" "$raw/upstream" "$raw/libcxx_selected" "$raw/noncancel"
run_case() {
  local group=$1 name=$2 binary=$3 arg=${4:-} log="$raw/$1/$2.log"
  printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout 15s %q %q\n' "$runtime:$baseline" "$binary" "$arg" > "$log"
  set +e
  env LD_LIBRARY_PATH="$runtime:$baseline" timeout 15s "$binary" "$arg" >> "$log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%s\t%d\n' "$group" "$name" "$rc"
}
printf 'group\tcase\texit_code\n'
for rt in libcxx libstdcxx; do
  binary="$workspace/tmp/R52/regression/x86_64/$rt"
  for mode in cancel exit real foreign cancel-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow exit-no-catch; do
    run_case regression "${rt}_$mode" "$binary" "$mode"
  done
done
for rt in libcxx libstdcxx; do
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
    run_case upstream "${rt}_$test" "$workspace/tmp/R51/tests/x86_64/$rt/$test" ignored
  done
done
for binary in "$workspace"/tmp/R53/upstream-tests/x86_64/*; do
  run_case libcxx_selected "$(basename "$binary")" "$binary" ignored
done
for version in before after; do
  for mode in getline_success getline_failure packaged_exception string_normal; do
    run_case noncancel "${version}_$mode" "$workspace/tmp/R52/noncancel/x86_64/$version" "$mode"
  done
done
