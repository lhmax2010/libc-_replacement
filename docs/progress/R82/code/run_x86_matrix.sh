#!/bin/bash
set -u
ulimit -c 0

workspace=/home/toolchain/development/libc++_replacement
root="$workspace/progress/R82/build/x86_64"
rpm_root="$workspace/progress/R82/build/rpm_roots/x86_64"
library_path="$root/runtime:$rpm_root/boost-filesystem/usr/lib64:$rpm_root/boost-iostreams/usr/lib64:$rpm_root/boost-program-options/usr/lib64:$rpm_root/boost-atomic/usr/lib64"

overall=0
for case_name in filesystem iostreams program_options; do
  binary="$root/${case_name}_boundary"
  for iteration in 1 2 3 4 5; do
    printf 'CASE=%s ITERATION=%s\n' "$case_name" "$iteration"
    LD_LIBRARY_PATH="$library_path" timeout 30s "$binary"
    rc=$?
    printf 'EXIT=%s\n' "$rc"
    if [ "$rc" -ne 0 ]; then overall=1; fi
  done
done
exit "$overall"
