#!/bin/bash
set -u

workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
patched="$workspace/tmp/R42/runtime/patched/x86_64"
unpatched="$sysroot/usr/lib64"
build="$workspace/progress/R43b/build/x86_64"
tests=(forced_unwind1 forced_unwind2 forced_unwind3 forced_unwind4 unwind_01 unwind_02 unwind_03 unwind_04 unwind_05 unwind_06)
modes=(cout getline packaged_task shrink_to_fit control)

run_cell() {
  local section=$1 suite=$2 binary=$3 argument=$4 library_path=$5
  local cmd rc
  printf '\nCELL_BEGIN section=%s arch=x86_64 suite=%s test=%s\n' "$section" "$suite" "$argument"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
  if [ "$section" = A ]; then
    cmd=(/usr/bin/timeout --preserve-status 20s "$loader" --library-path "$library_path" "$binary")
  else
    cmd=(/usr/bin/timeout --preserve-status 20s "$loader" --library-path "$library_path" "$binary" "$argument")
  fi
  printf 'RUN_COMMAND='; printf '%q ' "${cmd[@]}"; printf '\n'
  "${cmd[@]}"
  rc=$?
  printf 'CELL_EXIT_CODE=%d\n' "$rc"
  if [ "$rc" -gt 128 ]; then printf 'CELL_SIGNAL=%d\n' "$((rc-128))"; else printf 'CELL_SIGNAL=0\n'; fi
  printf 'CELL_END section=%s arch=x86_64 suite=%s test=%s\n' "$section" "$suite" "$argument"
}

printf 'RUNTIME_IDENTITIES_BEGIN\n'
sha256sum "$unpatched/libc++abi.so.1.0" "$patched/libc++abi.so.1.0" "$unpatched/libc++.so.1.0"
"$loader" --library-path "$unpatched:$sysroot/lib64" --list "$build/libcxx/forced_unwind1"
"$loader" --library-path "$patched:$unpatched:$sysroot/lib64" --list "$build/libcxx/forced_unwind1"
"$loader" --library-path "$unpatched:$sysroot/lib64" --list "$build/libstdcxx/forced_unwind1"
printf 'RUNTIME_IDENTITIES_END\n'

printf 'LD_DEBUG_IDENTITY_BEGIN\n'
LD_DEBUG=libs /usr/bin/timeout --preserve-status 20s "$loader" \
  --library-path "$patched:$unpatched:$sysroot/lib64" "$build/libcxx/unwind_01"
printf 'LD_DEBUG_EXIT_CODE=%d\nLD_DEBUG_IDENTITY_END\n' "$?"

for test_name in "${tests[@]}"; do
  run_cell A libcxx_unpatched "$build/libcxx/$test_name" "$test_name" "$unpatched:$sysroot/lib64"
  run_cell A libcxx_r42 "$build/libcxx/$test_name" "$test_name" "$patched:$unpatched:$sysroot/lib64"
  run_cell A libstdcxx_control "$build/libstdcxx/$test_name" "$test_name" "$unpatched:$sysroot/lib64"
done

for mode in "${modes[@]}"; do
  run_cell B libcxx_unpatched "$build/libcxx/stdlib_cancel_probe" "$mode" "$unpatched:$sysroot/lib64"
  run_cell B libcxx_r42 "$build/libcxx/stdlib_cancel_probe" "$mode" "$patched:$unpatched:$sysroot/lib64"
  run_cell B libstdcxx_control "$build/libstdcxx/stdlib_cancel_probe" "$mode" "$unpatched:$sysroot/lib64"
done
exit 0
