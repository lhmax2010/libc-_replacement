#!/bin/bash
set -u

if [ "$#" -ne 2 ]; then
  printf 'usage: %s VARIANT CASE\n' "$0" >&2
  exit 2
fi

variant=$1
case_name=$2
workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
runtime="$workspace/tmp/R40/runtime/$variant/x86_64"
binary="$workspace/tmp/R40/probes/$variant/x86_64/s1_libcxx"
library_path="$runtime:$sysroot/usr/lib64:$sysroot/lib64"

printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
printf 'VARIANT=%s\nCASE=%s\nEXPORT_DYNAMIC=NO\n' "$variant" "$case_name"
printf 'RUNTIME_IDENTITY_COMMAND='; printf '%q ' "$loader" --list --library-path "$library_path" "$binary"; printf '\n'
"$loader" --list --library-path "$library_path" "$binary"
printf 'RUNTIME_SHA256_BEGIN\n'
sha256sum "$binary" "$runtime/libc++abi.so.1.0"
printf 'RUNTIME_SHA256_END\n'
printf 'RUN_COMMAND='; printf '%q ' env LD_DEBUG=libs /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$case_name"; printf '\n'
env LD_DEBUG=libs /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$case_name"
case_rc=$?
printf 'CASE_EXIT_CODE=%d\n' "$case_rc"
if [ "$case_rc" -gt 128 ]; then
  printf 'CASE_SIGNAL=%d\n' "$((case_rc - 128))"
else
  printf 'CASE_SIGNAL=0\n'
fi
exit 0
