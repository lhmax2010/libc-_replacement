#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
tmp_root="$workspace/tmp/R14"

g++ --version
gcc --version
ld --version
readelf --version
ldd --version
uname -a
cat /proc/sys/kernel/core_pattern
ulimit -c

for scheme in A B; do
  for elf in \
    "$tmp_root/runtime/$scheme/libnew_$scheme.so" \
    "$tmp_root/runtime/$scheme/liblegacy.so" \
    "$tmp_root/runtime/$scheme/libc++abi.so.1.0" \
    "$tmp_root/runtime/$scheme/libgcc_s.so.1"; do
    printf '===== ELF %s =====\n' "$elf"
    file "$elf"
    sha256sum "$elf"
    readelf -d "$elf"
    readelf --dyn-syms --wide "$elf" | awk 'NR <= 3 || /_Unwind_/'
    readelf --version-info --wide "$elf"
  done
done

elf="$tmp_root/runtime/B/libunwind.so.1.0"
printf '===== ELF %s =====\n' "$elf"
file "$elf"
sha256sum "$elf"
readelf -d "$elf"
readelf --dyn-syms --wide "$elf" | awk 'NR <= 3 || /_Unwind_/'
readelf --version-info --wide "$elf"

for executable in \
  "$tmp_root/bin/main_dlopen" \
  "$tmp_root/bin/main_dt_B_legacy_new" \
  "$tmp_root/bin/main_dt_B_new_legacy" \
  "$tmp_root/bin/main_dt_A_legacy_new" \
  "$tmp_root/bin/main_dt_A_new_legacy"; do
  printf '===== EXECUTABLE %s =====\n' "$executable"
  file "$executable"
  sha256sum "$executable"
  readelf -d "$executable"
done
