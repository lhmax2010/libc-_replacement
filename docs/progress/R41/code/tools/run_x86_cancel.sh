#!/bin/bash
set -u
workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
runtime="$workspace/tmp/R41/runtime/x86_64"
binary="$workspace/tmp/R40/probes/diagnostic/x86_64/s1_libcxx"
library_path="$runtime:$sysroot/usr/lib64:$sysroot/lib64"
printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
printf 'RUN_COMMAND='; printf ' %q' /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" cancel; printf '\n'
/usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" cancel
rc=$?
printf 'CASE_EXIT_CODE=%d\n' "$rc"
if [ "$rc" -gt 128 ]; then printf 'CASE_SIGNAL=%d\n' "$((rc-128))"; else printf 'CASE_SIGNAL=0\n'; fi
exit 0

