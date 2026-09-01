#!/bin/bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
clang_root="$workspace/progress/R80/build/cached-clang"
compiler="$clang_root/usr/bin/clang++"
libcxx_root="$workspace/tmp/R91/x86_64-rpm-extract-retry1"
include_libcxx="$libcxx_root/libc++-devel/usr/include/c++/v1"
include_boost="$workspace/tmp/corpus/extracted/boost-1.83.0-2.2.src.rpm/unpacked/boost_1_83_0.tar.bz2/boost_1.83.0"
boost_root="$workspace/progress/R82/build/rpm_roots/x86_64"
output="$workspace/progress/R82/build/x86_64"
runtime="$output/runtime"

mkdir -p "$output" "$runtime"
ln -sfn "$libcxx_root/libc++/usr/lib64/libc++.so.1.0" "$runtime/libc++.so.1.0"
ln -sfn libc++.so.1.0 "$runtime/libc++.so.1"
ln -sfn "$libcxx_root/libc++abi/usr/lib64/libc++abi.so.1.0" "$runtime/libc++abi.so.1.0"
ln -sfn libc++abi.so.1.0 "$runtime/libc++abi.so.1"

common=(
  --no-default-config --target=x86_64-linux-gnu -std=c++20 -O0 -g
  -nostdinc++ -isystem "$include_libcxx" -isystem "$include_boost"
  -nostdlib++ "$runtime/libc++.so.1" "$runtime/libc++abi.so.1"
  -Wl,-rpath,"$runtime"
)

env LD_LIBRARY_PATH="$clang_root/usr/lib64" "$compiler" "${common[@]}" \
  "$workspace/progress/R82/code/filesystem_boundary.cpp" \
  "$boost_root/boost-filesystem/usr/lib64/libboost_filesystem.so.1.83.0" \
  -o "$output/filesystem_boundary"

env LD_LIBRARY_PATH="$clang_root/usr/lib64" "$compiler" "${common[@]}" \
  "$workspace/progress/R82/code/iostreams_boundary.cpp" \
  "$boost_root/boost-iostreams/usr/lib64/libboost_iostreams.so.1.83.0" \
  -o "$output/iostreams_boundary"

env LD_LIBRARY_PATH="$clang_root/usr/lib64" "$compiler" "${common[@]}" \
  "$workspace/progress/R82/code/program_options_boundary.cpp" \
  "$boost_root/boost-program-options/usr/lib64/libboost_program_options.so.1.83.0" \
  -o "$output/program_options_boundary"

for binary in "$output/filesystem_boundary" "$output/iostreams_boundary" "$output/program_options_boundary"; do
  printf 'BUILT %s\n' "$binary"
  readelf -d "$binary" | sed -n '/NEEDED/p;/RUNPATH/p'
done
