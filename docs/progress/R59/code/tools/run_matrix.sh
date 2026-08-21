#!/usr/bin/env bash
set -u -o pipefail
workspace=/home/toolchain/development/libc++_replacement
runtime="$workspace/tmp/R58/runtime/x86_64"
baseline_lib="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
bin="$workspace/tmp/R59/bin"
raw="$workspace/progress/R59/raw/matrix"
mkdir -p "$raw" "$workspace/progress/R59/raw/identity"
run_case() {
  local defect=$1 variant=$2 mode=$3 binary=$4 path=$5
  local log="$raw/${defect}_${variant}.log" rc=0
  if [[ -n $path ]]; then
    printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout 8s %q %q\n' "$path" "$binary" "$mode" > "$log"
    env LD_LIBRARY_PATH="$path" timeout 8s "$binary" "$mode" >> "$log" 2>&1 || rc=$?
  else
    printf 'COMMAND=timeout 8s %q %q\n' "$binary" "$mode" > "$log"
    timeout 8s "$binary" "$mode" >> "$log" 2>&1 || rc=$?
  fi
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%s\t%d\n' "$defect" "$variant" "$rc"
}
printf 'defect\tvariant\texit_code\n'
for mode in f1 f2 m1 m2; do
  run_case "${mode^^}" current "$mode" "$bin/current" "$runtime/abc:$baseline_lib"
  run_case "${mode^^}" baseline "$mode" "$bin/baseline" "$runtime/baseline:$baseline_lib"
  run_case "${mode^^}" reference "$mode" "$bin/reference" ''
done
sha256sum "$runtime/abc/libc++.so.1" "$runtime/abc/libc++abi.so.1" "$runtime/baseline/libc++.so.1" "$runtime/baseline/libc++abi.so.1" "$bin"/* > "$workspace/progress/R59/raw/identity/sha256.txt"
LD_LIBRARY_PATH="$runtime/abc:$baseline_lib" ldd "$bin/current" > "$workspace/progress/R59/raw/identity/ldd_current.txt" 2>&1
LD_DEBUG=libs LD_LIBRARY_PATH="$runtime/abc:$baseline_lib" "$bin/current" f2 > "$workspace/progress/R59/raw/identity/ld_debug_current.log" 2>&1 || true
