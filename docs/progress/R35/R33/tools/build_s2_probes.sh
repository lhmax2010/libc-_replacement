#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
out=$workspace/tmp/R33/s2-probes-r2
sysroot=$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0
test ! -e "$out"
mkdir -p "$out"
export CMAKE_BUILD_PARALLEL_LEVEL=4
export LLVM_PARALLEL_LINK_JOBS=1
nice -n 15 ionice -c 3 "$workspace/progress/R33/tools/tizen-clang++" \
  -O0 -g -pthread -stdlib=libc++ \
  -nostdinc++ -I"$sysroot/usr/include/c++/v1" \
  "$workspace/progress/R33/src/s2_foreign_probe.cpp" \
  -L"$sysroot/usr/lib64" -lc++ -lc++abi \
  -o "$out/s2_libcxx"
nice -n 15 ionice -c 3 "$workspace/progress/R33/tools/tizen-clang++" \
  -O0 -g -pthread \
  "$workspace/progress/R33/src/s2_foreign_probe.cpp" \
  -o "$out/s2_libstdcxx"
readelf -d "$out/s2_libcxx"
readelf -d "$out/s2_libstdcxx"
sha256sum "$out/s2_libcxx" "$out/s2_libstdcxx"
