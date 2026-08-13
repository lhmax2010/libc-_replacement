#!/usr/bin/env bash
set -uo pipefail
workspace=/home/toolchain/development/libc++_replacement
out=$workspace/tmp/R33/s2-probes-r2
sysroot=$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0
loader=$sysroot/usr/lib64/ld-linux-x86-64.so.2
libpath=$sysroot/usr/lib64:$sysroot/lib64
mkdir -p "$workspace/progress/R33/results/s2_matrix"
printf 'stdlib\tcase\texit_code\texecution\n' > "$workspace/progress/R33/tables/s2_matrix.tsv"
for stdlib in libcxx libstdcxx; do
  for case_name in foreign native; do
    log="$workspace/progress/R33/results/s2_matrix/${stdlib}_${case_name}.log"
    {
      printf 'EXECUTION=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
      printf 'COMMAND=%q --library-path %q %q %q\n' "$loader" "$libpath" "$out/s2_$stdlib" "$case_name"
      "$loader" --library-path "$libpath" "$out/s2_$stdlib" "$case_name"
      rc=$?
      printf 'EXIT_CODE=%s\n' "$rc"
    } >"$log" 2>&1
    rc=$(awk -F= '/^EXIT_CODE=/{v=$2} END{print v}' "$log")
    printf '%s\t%s\t%s\tHOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n' "$stdlib" "$case_name" "$rc" >> "$workspace/progress/R33/tables/s2_matrix.tsv"
  done
done
cat "$workspace/progress/R33/tables/s2_matrix.tsv"
cat "$workspace/progress/R33/results/s2_matrix/"*.log
