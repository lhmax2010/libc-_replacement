#!/bin/bash
set -u

workspace=/home/toolchain/development/libc++_replacement
executor=/usr/bin/qemu-arm-static
sysroot="$workspace/tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0"
root="$workspace/progress/R82/build/armv7l"
rpm_root="$workspace/progress/R82/build/rpm_roots/armv7l"
guest_library_path="$rpm_root/boost-filesystem/usr/lib:$rpm_root/boost-iostreams/usr/lib:$rpm_root/boost-program-options/usr/lib:$rpm_root/boost-atomic/usr/lib:$sysroot/usr/lib:$sysroot/lib"

overall=0
for case_name in filesystem iostreams program_options; do
  binary="$root/${case_name}_boundary"
  for iteration in 1 2 3 4 5; do
    printf 'CASE=%s ITERATION=%s\n' "$case_name" "$iteration"
    timeout 30s "$executor" -L "$sysroot" -E "LD_LIBRARY_PATH=$guest_library_path" "$binary"
    rc=$?
    printf 'EXIT=%s\n' "$rc"
    if [ "$rc" -ne 0 ]; then overall=1; fi
  done
done
exit "$overall"
