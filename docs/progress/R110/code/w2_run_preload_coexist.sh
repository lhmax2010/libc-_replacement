#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
out="$root/docs/progress/R110/build/w2"
mkdir -p "$out"

extracted=/home/toolchain/development/libc++_replacement/tmp/R11/extracted
qemu="$extracted/41/4154ea903c230bf1f887ef759c43e5f7dcb48de108fd2445550ce1eaff3af553/usr/bin/qemu-aarch64"
glibc="$extracted/76/7606e2955a44b9255ca19db9bfc1ff9b96c047c5dd1aee7a85472a0a67af84a7"
libcpp="$extracted/7b/7b8399da9e44581207c907dd5a25d65c31a63eab074a9760d3bc3a11372e8657"
libgcc="$extracted/c3/c3c00b54b4714c9aa4e3ab87b1eca43af33dbc62c4d49067be27270d8733023c"
libstdcpp="$extracted/d3/d303775042ed2f62dbc343c0c2d00fa1fc564925a67db35676c62c458dade500"
program="$glibc/usr/libexec/getconf/getconf"
loader="$glibc/lib64/ld-linux-aarch64.so.1"
library_path="$libcpp/usr/lib64:$libstdcpp/usr/lib64:$libgcc/usr/lib64:$glibc/lib64"
preload="$libcpp/usr/lib64/libc++.so.1.0:$libstdcpp/usr/lib64/libstdc++.so.6.0.33"

for input in "$qemu" "$program" "$loader" \
  "$libcpp/usr/lib64/libc++.so.1.0" \
  "$libcpp/usr/lib64/libc++abi.so.1.0" \
  "$libstdcpp/usr/lib64/libstdc++.so.6.0.33" \
  "$libgcc/usr/lib64/libgcc_s.so.1"; do
  test -r "$input"
done

file "$qemu" "$program" "$loader" \
  "$libcpp/usr/lib64/libc++.so.1.0" \
  "$libstdcpp/usr/lib64/libstdc++.so.6.0.33"
"$qemu" --version

"$qemu" -E LD_DEBUG=files "$loader" \
  --library-path "$library_path" \
  --preload "$preload" \
  "$program" GNU_LIBC_VERSION \
  >"$out/preload_coexist.stdout" \
  2>"$out/preload_coexist.stderr"

test "$(cat "$out/preload_coexist.stdout")" = "glibc 2.40"
rg -q "file=.*/libc[+][+][.]so[.]1[.]0 .*generating link map" "$out/preload_coexist.stderr"
rg -q "file=.*/libstdc[+][+][.]so[.]6[.]0[.]33 .*generating link map" "$out/preload_coexist.stderr"
rg -q "calling init: .*/libc[+][+][.]so[.]1[.]0" "$out/preload_coexist.stderr"
rg -q "calling init: .*/libstdc[+][+][.]so[.]6[.]0[.]33" "$out/preload_coexist.stderr"
rg -q "calling fini: .*/libc[+][+][.]so[.]1[.]0" "$out/preload_coexist.stderr"
rg -q "calling fini: .*/libstdc[+][+][.]so[.]6[.]0[.]33" "$out/preload_coexist.stderr"

cat "$out/preload_coexist.stdout"
rg 'libc[+][+][.]so|libstdc[+][+][.]so|calling init|calling fini' "$out/preload_coexist.stderr" >&2
printf 'ASSERTIONS=PASS arch=aarch64 qemu_user=1 libcxx_loaded=1 libstdcxx_loaded=1 init_fini_both=1 exact_value="glibc 2.40"\n'
