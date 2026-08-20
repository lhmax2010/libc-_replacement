#!/usr/bin/env bash
set -u
workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
libcxx="$workspace/tmp/R52/rebuilt-libcxx/x86_64"
abi="$workspace/tmp/R51/runtime/x86_64"
tests="$workspace/tmp/R52/noncancel/x86_64"
for mode in getline_success getline_failure packaged_exception string_normal; do
  for variant in before after; do
    printf '\nCELL_BEGIN matrix=noncancel arch=x86_64 variant=%s test=%s\n' "$variant" "$mode"
    printf 'RUN_COMMAND='; printf '%q ' "$loader" --library-path "$abi:$libcxx:$sysroot/usr/lib64:$sysroot/lib64" "$tests/$variant" "$mode"; printf '\n'
    "$loader" --library-path "$abi:$libcxx:$sysroot/usr/lib64:$sysroot/lib64" "$tests/$variant" "$mode"
    rc=$?
    printf 'CELL_EXIT_CODE=%d\nCELL_END matrix=noncancel arch=x86_64 variant=%s test=%s\n' "$rc" "$variant" "$mode"
  done
done
