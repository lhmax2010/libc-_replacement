#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
product_abi="$workspace/tmp/R51/runtime/x86_64/libc++abi.so.1.0"
rebuilt_libcxx="$workspace/tmp/R52/rebuilt-libcxx/x86_64/libc++.so.1.0"
tests="$workspace/tmp/R52/tests/x86_64"
runtime="$workspace/tmp/R52/runtime/x86_64"
syslibs="$sysroot/usr/lib64:$sysroot/lib64"

mkdir -p "$runtime/unpatched" "$runtime/layer_ab" "$runtime/header_batch"
ln -sfn "$baseline/libc++.so.1.0" "$runtime/unpatched/libc++.so.1"
ln -sfn "$baseline/libc++abi.so.1.0" "$runtime/unpatched/libc++abi.so.1"
ln -sfn "$baseline/libc++.so.1.0" "$runtime/layer_ab/libc++.so.1"
ln -sfn "$product_abi" "$runtime/layer_ab/libc++abi.so.1"
ln -sfn "$rebuilt_libcxx" "$runtime/header_batch/libc++.so.1"
ln -sfn "$product_abi" "$runtime/header_batch/libc++abi.so.1"

run_cell() {
  local mode=$1 suite=$2 library_path=$3 binary=$4
  printf '\nCELL_BEGIN arch=x86_64 mode=%s suite=%s\n' "$mode" "$suite"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_NO_QEMU\n'
  printf 'RUN_COMMAND='
  printf '%q ' /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$mode"
  printf '\n'
  /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$mode"
  rc=$?
  printf 'CELL_EXIT_CODE=%d\n' "$rc"
  if (( rc > 128 )); then printf 'CELL_SIGNAL=%d\n' "$((rc - 128))"; else printf 'CELL_SIGNAL=0\n'; fi
  printf 'CELL_END arch=x86_64 mode=%s suite=%s\n' "$mode" "$suite"
}

printf 'IDENTITY_BEGIN\n'
sha256sum "$baseline/libc++.so.1.0" "$baseline/libc++abi.so.1.0" "$product_abi" \
  "$rebuilt_libcxx" "$tests/unpatched_headers" "$tests/patched_headers" "$tests/libstdcxx"
for suite in unpatched layer_ab header_batch; do
  printf 'LOADER_LIST_SUITE=%s\n' "$suite"
  "$loader" --library-path "$runtime/$suite:$syslibs" --list "$tests/unpatched_headers"
  printf 'LOADER_LIST_EXIT_CODE=%d\n' "$?"
done
printf 'LD_DEBUG_SUITE=header_fix mode=getline\n'
LD_DEBUG=libs /usr/bin/timeout --preserve-status 15s "$loader" \
  --library-path "$runtime/header_batch:$syslibs" "$tests/patched_headers" getline
printf 'LD_DEBUG_EXIT_CODE=%d\nIDENTITY_END\n' "$?"

for mode in getline packaged_task shrink_to_fit; do
  run_cell "$mode" unpatched "$runtime/unpatched:$syslibs" "$tests/unpatched_headers"
  run_cell "$mode" layer_ab_headers_unmodified "$runtime/layer_ab:$syslibs" "$tests/unpatched_headers"
  run_cell "$mode" header_fix_rebuilt_consumer "$runtime/header_batch:$syslibs" "$tests/patched_headers"
  run_cell "$mode" rebuilt_libcxx_stale_consumer "$runtime/header_batch:$syslibs" "$tests/unpatched_headers"
  run_cell "$mode" libstdcxx_control "$syslibs" "$tests/libstdcxx"
done

exit 0
