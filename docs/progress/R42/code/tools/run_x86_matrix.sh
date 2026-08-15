#!/bin/bash
set -u
workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
patched="$workspace/tmp/R42/runtime/patched/x86_64"
probes="$workspace/tmp/R42/probes/x86_64"

run_cell() {
  suite=$1
  binary=$2
  mode=$3
  library_path=$4
  printf '\nCELL_BEGIN suite=%s mode=%s\n' "$suite" "$mode"
  if [ "$mode" = NO_ARGUMENT ]; then
    cmd=(/usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary")
  else
    cmd=(/usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$mode")
  fi
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
  printf 'RUN_COMMAND='; printf '%q ' "${cmd[@]}"; printf '\n'
  "${cmd[@]}"
  rc=$?
  printf 'CELL_EXIT_CODE=%d\n' "$rc"
  if [ "$rc" -gt 128 ]; then printf 'CELL_SIGNAL=%d\n' "$((rc-128))"; else printf 'CELL_SIGNAL=0\n'; fi
  printf 'CELL_END suite=%s mode=%s\n' "$suite" "$mode"
}

printf 'RUNTIME_IDENTITY_PATCHED_BEGIN\n'
env LD_LIBRARY_PATH="$patched:$sysroot/usr/lib64:$sysroot/lib64" ldd "$probes/s1_libcxx"
sha256sum "$patched/libc++abi.so.1.0" "$probes/s1_libcxx"
printf 'RUNTIME_IDENTITY_PATCHED_END\n'

modes=(cancel exit real foreign cancel-no-catch exit-no-catch cancel-nested)
for mode in "${modes[@]}"; do
  run_cell libcxx_patched "$probes/s1_libcxx" "$mode" "$patched:$sysroot/usr/lib64:$sysroot/lib64"
done
for mode in "${modes[@]}"; do
  run_cell libstdcxx_control "$probes/s1_libstdcxx" "$mode" "$sysroot/usr/lib64:$sysroot/lib64"
done
run_cell libcxx_unpatched "$probes/unpatched_libcxx" NO_ARGUMENT "$sysroot/usr/lib64:$sysroot/lib64"
exit 0
