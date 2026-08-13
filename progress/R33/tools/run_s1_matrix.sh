#!/usr/bin/env bash
set -uo pipefail
workspace=/home/toolchain/development/libc++_replacement
out=$workspace/tmp/R33/s1-probes-r3
runtime=$workspace/tmp/R33/s1-runtime-r3
sysroot=$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0
loader=$sysroot/usr/lib64/ld-linux-x86-64.so.2
libpath=$runtime:$sysroot/usr/lib64:$sysroot/lib64
mkdir -p "$workspace/progress/R33/results/s1_matrix_r3"
printf 'stdlib\tcase\texit_code\texecution\n' > "$workspace/progress/R33/tables/s1_matrix_r3.tsv"
for stdlib in libcxx libstdcxx; do
  for case_name in cancel real exit; do
    log="$workspace/progress/R33/results/s1_matrix_r3/${stdlib}_${case_name}.log"
    {
      printf 'EXECUTION=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
      printf 'COMMAND=%q --library-path %q %q %q\n' "$loader" "$libpath" "$out/s1_$stdlib" "$case_name"
      "$loader" --library-path "$libpath" "$out/s1_$stdlib" "$case_name"
      rc=$?
      printf 'EXIT_CODE=%s\n' "$rc"
    } >"$log" 2>&1
    rc=$(awk -F= '/^EXIT_CODE=/{v=$2} END{print v}' "$log")
    printf '%s\t%s\t%s\tHOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n' "$stdlib" "$case_name" "$rc" >> "$workspace/progress/R33/tables/s1_matrix_r3.tsv"
  done
done
cat "$workspace/progress/R33/tables/s1_matrix_r3.tsv"
cat "$workspace/progress/R33/results/s1_matrix_r3/"*.log
