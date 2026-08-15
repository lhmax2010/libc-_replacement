#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
r45_abi="$workspace/tmp/R45/runtime/x86_64/libc++abi.so.1.0"
spike_libcxx="$workspace/tmp/R44b/build/x86_64/libc++.so.1.0"
test_dir="$workspace/tmp/R44b/tests/x86_64"
runtime="$workspace/tmp/R45/aggregation/x86_64"

mkdir -p "$runtime/unpatched" "$runtime/layer1_r45" "$runtime/layer1_plus_ios"
ln -sfn "$baseline/libc++.so.1.0" "$runtime/unpatched/libc++.so.1"
ln -sfn "$baseline/libc++abi.so.1.0" "$runtime/unpatched/libc++abi.so.1"
ln -sfn "$baseline/libc++.so.1.0" "$runtime/layer1_r45/libc++.so.1"
ln -sfn "$r45_abi" "$runtime/layer1_r45/libc++abi.so.1"
ln -sfn "$spike_libcxx" "$runtime/layer1_plus_ios/libc++.so.1"
ln -sfn "$r45_abi" "$runtime/layer1_plus_ios/libc++abi.so.1"

run_cell() {
  local suite=$1
  local library_path=$2
  local binary=$3
  local repeat=$4
  local rc
  printf '\nCELL_BEGIN matrix=aggregation arch=x86_64 suite=%s test=cout repeat=%s\n' "$suite" "$repeat"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_NO_QEMU\n'
  printf 'RUN_COMMAND='
  printf '%q ' /usr/bin/timeout --verbose --preserve-status 20s "$loader" --library-path "$library_path" "$binary" cout
  printf '\n'
  /usr/bin/timeout --verbose --preserve-status 20s "$loader" --library-path "$library_path" "$binary" cout
  rc=$?
  printf 'CELL_EXIT_CODE=%d\n' "$rc"
  if (( rc > 128 )); then printf 'CELL_SIGNAL=%d\n' "$((rc - 128))"; else printf 'CELL_SIGNAL=0\n'; fi
  printf 'CELL_END matrix=aggregation arch=x86_64 suite=%s test=cout repeat=%s\n' "$suite" "$repeat"
}

printf 'RUNTIME_IDENTITIES_BEGIN\n'
sha256sum \
  "$baseline/libc++.so.1.0" "$baseline/libc++abi.so.1.0" \
  "$r45_abi" "$spike_libcxx" \
  "$test_dir/libcxx/stdlib_cancel_probe" "$test_dir/libstdcxx/stdlib_cancel_probe"
for suite in unpatched layer1_r45 layer1_plus_ios; do
  printf 'LOADER_LIST_SUITE=%s\n' "$suite"
  "$loader" --library-path "$runtime/$suite:$sysroot/usr/lib64:$sysroot/lib64" \
    --list "$test_dir/libcxx/stdlib_cancel_probe"
  printf 'LOADER_LIST_EXIT_CODE=%d\n' "$?"
done
printf 'LOADER_LIST_SUITE=libstdcxx_control\n'
"$loader" --library-path "$sysroot/usr/lib64:$sysroot/lib64" \
  --list "$test_dir/libstdcxx/stdlib_cancel_probe"
printf 'LOADER_LIST_EXIT_CODE=%d\nRUNTIME_IDENTITIES_END\n' "$?"

printf 'LD_DEBUG_IDENTITY_BEGIN\n'
LD_DEBUG=libs /usr/bin/timeout --preserve-status 20s "$loader" \
  --library-path "$runtime/layer1_plus_ios:$sysroot/usr/lib64:$sysroot/lib64" \
  "$test_dir/libcxx/stdlib_cancel_probe" control
printf 'LD_DEBUG_EXIT_CODE=%d\nLD_DEBUG_IDENTITY_END\n' "$?"

for repeat in 1 2 3; do
  run_cell unpatched "$runtime/unpatched:$sysroot/usr/lib64:$sysroot/lib64" "$test_dir/libcxx/stdlib_cancel_probe" "$repeat"
  run_cell layer1_r45 "$runtime/layer1_r45:$sysroot/usr/lib64:$sysroot/lib64" "$test_dir/libcxx/stdlib_cancel_probe" "$repeat"
  run_cell layer1_plus_ios "$runtime/layer1_plus_ios:$sysroot/usr/lib64:$sysroot/lib64" "$test_dir/libcxx/stdlib_cancel_probe" "$repeat"
  run_cell libstdcxx_control "$sysroot/usr/lib64:$sysroot/lib64" "$test_dir/libstdcxx/stdlib_cancel_probe" "$repeat"
done
exit 0
