#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
build="$root/docs/progress/R110/build/w2"
mkdir -p "$build"

extracted=/home/toolchain/development/libc++_replacement/tmp/R11/extracted
qemu="$extracted/41/4154ea903c230bf1f887ef759c43e5f7dcb48de108fd2445550ce1eaff3af553/usr/bin/qemu-aarch64"
glibc="$extracted/76/7606e2955a44b9255ca19db9bfc1ff9b96c047c5dd1aee7a85472a0a67af84a7"
libcpp="$extracted/7b/7b8399da9e44581207c907dd5a25d65c31a63eab074a9760d3bc3a11372e8657"
libgcc="$extracted/c3/c3c00b54b4714c9aa4e3ab87b1eca43af33dbc62c4d49067be27270d8733023c"
libstdcpp="$extracted/d3/d303775042ed2f62dbc343c0c2d00fa1fc564925a67db35676c62c458dade500"
cross_root="$extracted/ec/ecf4cac5f6fa7dd52dbb3535013f06d02fa72ccca9bead9241f44ff772a9445d/emul"

for input in \
  "$qemu" \
  "$glibc/lib64/ld-linux-aarch64.so.1" \
  "$glibc/lib64/libc.so.6" \
  "$libcpp/usr/lib64/libc++.so.1.0" \
  "$libcpp/usr/lib64/libc++abi.so.1.0" \
  "$libgcc/usr/lib64/libgcc_s.so.1" \
  "$libstdcpp/usr/lib64/libstdc++.so.6.0.33"; do
  test -r "$input"
done

"$qemu" --version
llvm-mc -filetype=obj -triple=aarch64-linux-gnu \
  "$root/docs/progress/R110/code/w2_aarch64_coexist.S" \
  -o "$build/w2_aarch64_coexist.o"
  "$cross_root/usr/lib64/ld-linux-x86-64.so.2" \
  --library-path "$cross_root/usr/lib64" \
  "$cross_root/usr/bin/ld" -m aarch64linux -e _start \
  --allow-shlib-undefined \
  --dynamic-linker /lib/ld-linux-aarch64.so.1 \
  -o "$build/w2_aarch64_coexist" \
  "$build/w2_aarch64_coexist.o" \
  --no-as-needed \
  "$libcpp/usr/lib64/libc++.so.1.0" \
  "$libstdcpp/usr/lib64/libstdc++.so.6.0.33"

file "$build/w2_aarch64_coexist"
readelf -h -d -l "$build/w2_aarch64_coexist"
readelf -d "$build/w2_aarch64_coexist" | rg -q 'Shared library: \[libc[+][+][.]so[.]1\]'
readelf -d "$build/w2_aarch64_coexist" | rg -q 'Shared library: \[libstdc[+][+][.]so[.]6\]'

library_path="$libcpp/usr/lib64:$libstdcpp/usr/lib64:$libgcc/usr/lib64:$glibc/lib64"
"$qemu" "$glibc/lib64/ld-linux-aarch64.so.1" \
  --library-path "$library_path" \
  "$build/w2_aarch64_coexist" >"$build/maps.stdout"

rg -q 'libc[+][+][.]so[.]1[.]0' "$build/maps.stdout"
rg -q 'libstdc[+][+][.]so[.]6[.]0[.]33' "$build/maps.stdout"
rg -q '^RESULT=PASS maps_dump_complete=1$' "$build/maps.stdout"
rg 'libc[+][+]|libstdc[+][+]|RESULT=' "$build/maps.stdout"
printf 'ASSERTIONS=PASS qemu_aarch64=1 both_cpp_runtimes_mapped=1 exact_marker=1\n'
