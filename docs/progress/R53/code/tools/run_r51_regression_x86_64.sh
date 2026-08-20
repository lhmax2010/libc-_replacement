#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
libcxx="$workspace/tmp/R53/build/x86_64"
abi="$workspace/tmp/R51/runtime/x86_64"
tests="$workspace/tmp/R52/regression/x86_64"

run_cell() {
  local suite=$1 mode=$2 binary=$3 library_path=$4 rc
  printf '\nCELL_BEGIN matrix=r51_regression arch=x86_64 suite=%s test=%s\n' "$suite" "$mode"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\nRUN_COMMAND='
  printf '%q ' /usr/bin/timeout --preserve-status 20s "$loader" --library-path "$library_path" "$binary" "$mode"
  printf '\n'
  /usr/bin/timeout --preserve-status 20s "$loader" --library-path "$library_path" "$binary" "$mode"
  rc=$?
  printf 'CELL_EXIT_CODE=%d\nCELL_SIGNAL=%d\n' "$rc" "$((rc > 128 ? rc - 128 : 0))"
  printf 'CELL_END matrix=r51_regression arch=x86_64 suite=%s test=%s\n' "$suite" "$mode"
}

sha256sum "$libcxx/libc++.so.1.0" "$abi/libc++abi.so.1.0" "$tests/libcxx" "$tests/libstdcxx"
printf 'LD_DEBUG_IDENTITY_BEGIN\n'
LD_DEBUG=libs /usr/bin/timeout --preserve-status 20s "$loader" \
  --library-path "$abi:$libcxx:$sysroot/usr/lib64:$sysroot/lib64" "$tests/libcxx" real
printf 'LD_DEBUG_EXIT_CODE=%d\nLD_DEBUG_IDENTITY_END\n' "$?"

modes=(cancel exit real foreign cancel-no-catch exit-no-catch cancel-nested cancel-catchall-rethrow exit-catchall-rethrow)
for mode in "${modes[@]}"; do
  run_cell product_full_layer_c "$mode" "$tests/libcxx" "$abi:$libcxx:$sysroot/usr/lib64:$sysroot/lib64"
  run_cell libstdcxx_control "$mode" "$tests/libstdcxx" "$sysroot/usr/lib64:$sysroot/lib64"
done
exit 0
