#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
product_abi="$workspace/tmp/R51/runtime/x86_64/libc++abi.so.1.0"
full_libcxx="$workspace/tmp/R53/build/x86_64/libc++.so.1.0"
tests="$workspace/tmp/R52/tests/x86_64"
runtime="$workspace/tmp/R53/r52-runtime/x86_64"
syslibs="$sysroot/usr/lib64:$sysroot/lib64"

mkdir -p "$runtime/unpatched" "$runtime/layer_ab" "$runtime/full_layer_c"
ln -sfn "$baseline/libc++.so.1.0" "$runtime/unpatched/libc++.so.1"
ln -sfn "$baseline/libc++abi.so.1.0" "$runtime/unpatched/libc++abi.so.1"
ln -sfn "$baseline/libc++.so.1.0" "$runtime/layer_ab/libc++.so.1"
ln -sfn "$product_abi" "$runtime/layer_ab/libc++abi.so.1"
ln -sfn "$full_libcxx" "$runtime/full_layer_c/libc++.so.1"
ln -sfn "$product_abi" "$runtime/full_layer_c/libc++abi.so.1"

run_cell() {
  local mode=$1 suite=$2 library_path=$3 binary=$4 rc
  printf '\nCELL_BEGIN matrix=r52_recheck arch=x86_64 mode=%s suite=%s\n' "$mode" "$suite"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_NO_QEMU\nRUN_COMMAND='
  printf '%q ' /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$mode"
  printf '\n'
  /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$mode"
  rc=$?
  printf 'CELL_EXIT_CODE=%d\nCELL_SIGNAL=%d\n' "$rc" "$((rc > 128 ? rc - 128 : 0))"
  printf 'CELL_END matrix=r52_recheck arch=x86_64 mode=%s suite=%s\n' "$mode" "$suite"
}

sha256sum "$baseline/libc++.so.1.0" "$baseline/libc++abi.so.1.0" "$product_abi" \
  "$full_libcxx" "$tests/unpatched_headers" "$tests/patched_headers" "$tests/libstdcxx"
LD_DEBUG=libs /usr/bin/timeout --preserve-status 15s "$loader" \
  --library-path "$runtime/full_layer_c:$syslibs" "$tests/patched_headers" getline
printf 'LD_DEBUG_EXIT_CODE=%d\n' "$?"

for mode in getline packaged_task shrink_to_fit; do
  run_cell "$mode" unpatched "$runtime/unpatched:$syslibs" "$tests/unpatched_headers"
  run_cell "$mode" layer_ab_headers_unmodified "$runtime/layer_ab:$syslibs" "$tests/unpatched_headers"
  run_cell "$mode" full_layer_c_rebuilt_consumer "$runtime/full_layer_c:$syslibs" "$tests/patched_headers"
  run_cell "$mode" full_layer_c_stale_consumer "$runtime/full_layer_c:$syslibs" "$tests/unpatched_headers"
  run_cell "$mode" libstdcxx_control "$syslibs" "$tests/libstdcxx"
done
exit 0
