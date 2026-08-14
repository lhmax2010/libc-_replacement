#!/usr/bin/env bash
set -uo pipefail

workspace=/home/toolchain/development/libc++_replacement
sysroot=$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0
runtime=$workspace/tmp/R34B/runtime
loader=$sysroot/lib64/ld-linux-x86-64.so.2
library_path=$runtime:$sysroot/usr/lib64:$sysroot/lib64
result_dir=$workspace/progress/R34B/results/matrix
summary=$workspace/progress/R34B/tables/matrix_raw.tsv

mkdir -p "$result_dir"
printf 'stdlib\tcase\texit_code\tsignal\tlog\n' >"$summary"

for stdlib in libcxx libstdcxx; do
  binary=$workspace/tmp/R34B/probes/s1_$stdlib
  for case_name in cancel exit real foreign cancel-no-catch exit-no-catch; do
    log=$result_dir/${stdlib}_${case_name}.log
    {
      printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
      printf 'PWD=%s\n' "$workspace"
      printf 'COMMAND='; printf '%q ' /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$case_name"; printf '\n'
    } >"$log"
    /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$library_path" "$binary" "$case_name" >>"$log" 2>&1
    rc=$?
    signal=0
    if (( rc >= 128 && rc <= 255 )); then
      signal=$((rc - 128))
    fi
    printf 'EXIT_CODE=%s\nSIGNAL=%s\n' "$rc" "$signal" >>"$log"
    printf '%s\t%s\t%s\t%s\t%s\n' "$stdlib" "$case_name" "$rc" "$signal" "results/matrix/${stdlib}_${case_name}.log" >>"$summary"
  done
done
