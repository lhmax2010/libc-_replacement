#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
out="$root/docs/progress/R112/build/item4_boost_test"
source="$root/docs/progress/R112/code/item4_boost_test_actual_direction.cpp"
buildroot=/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R105-boost-aarch64/local/BUILD-ROOTS/scratch.aarch64.0
clang_root=/home/toolchain/development/libc++_replacement/progress/R80/build/cached-clang
input_root=/tmp/r110_w3_build/root
gcc_headers="$input_root/usr/lib64/gcc/aarch64-tizen-linux-gnu/14.2.0/include/c++"
libcxx="$buildroot/usr/lib64"
libstdcxx="$input_root/usr/lib64"
libgcc="$input_root/usr/lib64"
provider=/home/toolchain/development/libc++_replacement/progress/R105/artifacts/boost_aarch64_libcxx/boost-test-1.83.0-105.10.5.aarch64/usr/lib64/libboost_unit_test_framework.so.1.83.0
clang="$clang_root/usr/bin/clang++"
mkdir -p "$out"

for input in "$clang" "$source" "$provider" "$libcxx/libc++.so.1.0" \
  "$libcxx/libc++abi.so.1.0" "$libstdcxx/libstdc++.so.6.0.33" \
  "$libgcc/libgcc_s.so.1" "$gcc_headers/string"; do
  test -r "$input"
done

common=(env "LD_LIBRARY_PATH=$clang_root/usr/lib64" "$clang" --no-default-config
  --target=aarch64-tizen-linux-gnu --sysroot="$buildroot" -fuse-ld=lld
  -resource-dir=/tmp/r110_w3_build/clang_resource -std=c++20 -O0 -g0
  -nostdinc++ -nostdlib++ -pthread)

"${common[@]}" -isystem "$gcc_headers" -isystem "$gcc_headers/aarch64-tizen-linux-gnu" \
  -isystem "$gcc_headers/backward" "$source" "$libstdcxx/libstdc++.so.6.0.33" \
  "$libgcc/libgcc_s.so.1" -ldl -o "$out/libstd_consumer_libcxx_provider"

"${common[@]}" -isystem "$buildroot/usr/include/c++/v1" "$source" \
  "$libcxx/libc++.so.1.0" "$libcxx/libc++abi.so.1.0" "$libgcc/libgcc_s.so.1" \
  -ldl -o "$out/libcxx_consumer_libcxx_provider"

file "$out"/*consumer_libcxx_provider
for file in "$out"/*consumer_libcxx_provider "$provider"; do
  echo "FILE=$file"
  readelf -d "$file" | grep -E "NEEDED|BIND_NOW|FLAGS"
done
sha256sum "$out"/*consumer_libcxx_provider "$provider" "$source"
