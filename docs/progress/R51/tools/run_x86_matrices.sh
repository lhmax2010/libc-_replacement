#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
prestrip="$workspace/tmp/R45/runtime/x86_64"
product="$workspace/tmp/R51/runtime/x86_64"
tests="$workspace/tmp/R51/tests/x86_64"

run_cell() {
  local matrix=$1 suite=$2 test_name=$3 binary=$4 library_path=$5
  shift 5
  local cmd rc
  cmd=(/usr/bin/timeout --preserve-status 20s "$loader" --library-path "$library_path" "$binary" "$@")
  printf '\nCELL_BEGIN matrix=%s arch=x86_64 suite=%s test=%s\n' "$matrix" "$suite" "$test_name"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
  printf 'RUN_COMMAND='
  printf '%q ' "${cmd[@]}"
  printf '\n'
  "${cmd[@]}"
  rc=$?
  printf 'CELL_EXIT_CODE=%d\n' "$rc"
  if (( rc > 128 )); then printf 'CELL_SIGNAL=%d\n' "$((rc - 128))"; else printf 'CELL_SIGNAL=0\n'; fi
  printf 'CELL_END matrix=%s arch=x86_64 suite=%s test=%s\n' "$matrix" "$suite" "$test_name"
}

printf 'RUNTIME_IDENTITIES_BEGIN\n'
sha256sum \
  "$baseline/libc++.so.1.0" "$baseline/libc++abi.so.1.0" \
  "$prestrip/libc++abi.so.1.0" "$product/libc++abi.so.1.0"
for suite in unpatched prestrip product; do
  case "$suite" in
    unpatched) libpath="$baseline:$sysroot/usr/lib64:$sysroot/lib64" ;;
    prestrip) libpath="$prestrip:$baseline:$sysroot/usr/lib64:$sysroot/lib64" ;;
    product) libpath="$product:$baseline:$sysroot/usr/lib64:$sysroot/lib64" ;;
  esac
  printf 'LOADER_LIST_SUITE=%s\n' "$suite"
  "$loader" --library-path "$libpath" --list "$tests/libcxx/forced_unwind1"
  printf 'LOADER_LIST_EXIT_CODE=%d\n' "$?"
done
printf 'LD_DEBUG_IDENTITY_BEGIN\n'
LD_DEBUG=libs /usr/bin/timeout --preserve-status 20s "$loader" \
  --library-path "$product:$baseline:$sysroot/usr/lib64:$sysroot/lib64" \
  "$tests/libcxx/unwind_01"
printf 'LD_DEBUG_EXIT_CODE=%d\nLD_DEBUG_IDENTITY_END\n' "$?"
printf 'RUNTIME_IDENTITIES_END\n'

upstream_tests=(forced_unwind1 forced_unwind2 forced_unwind3 forced_unwind4
                unwind_01 unwind_02 unwind_03 unwind_04 unwind_05 unwind_06)
for test_name in "${upstream_tests[@]}"; do
  run_cell upstream unpatched "$test_name" "$tests/libcxx/$test_name" \
    "$baseline:$sysroot/usr/lib64:$sysroot/lib64"
  run_cell upstream prestrip "$test_name" "$tests/libcxx/$test_name" \
    "$prestrip:$baseline:$sysroot/usr/lib64:$sysroot/lib64"
  run_cell upstream product "$test_name" "$tests/libcxx/$test_name" \
    "$product:$baseline:$sysroot/usr/lib64:$sysroot/lib64"
  run_cell upstream libstdcxx_control "$test_name" "$tests/libstdcxx/$test_name" \
    "$sysroot/usr/lib64:$sysroot/lib64"
done

regression_modes=(cancel exit real foreign cancel-no-catch exit-no-catch
                  cancel-nested cancel-catchall-rethrow exit-catchall-rethrow)
for mode in "${regression_modes[@]}"; do
  run_cell regression prestrip "$mode" "$tests/libcxx/s1_rethrow_probe" \
    "$prestrip:$baseline:$sysroot/usr/lib64:$sysroot/lib64" "$mode"
  run_cell regression product "$mode" "$tests/libcxx/s1_rethrow_probe" \
    "$product:$baseline:$sysroot/usr/lib64:$sysroot/lib64" "$mode"
  run_cell regression libstdcxx_control "$mode" "$tests/libstdcxx/s1_rethrow_probe" \
    "$sysroot/usr/lib64:$sysroot/lib64" "$mode"
done
run_cell regression unpatched cancel "$tests/libcxx/unpatched_cancel_probe" \
  "$baseline:$sysroot/usr/lib64:$sysroot/lib64"
exit 0
