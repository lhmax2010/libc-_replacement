#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
out=$workspace/tmp/R33/s1-probes-r3
runtime=$workspace/tmp/R33/s1-runtime-r3
sysroot=$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0
build=$workspace/tmp/R33/build-s1-r3
test ! -e "$out"
test ! -e "$runtime"
mkdir -p "$out" "$runtime"
cp "$build/lib/libc++abi.so.1.0.versioned" "$runtime/libc++abi.so.1.0"
ln -s libc++abi.so.1.0 "$runtime/libc++abi.so.1"
ln -s libc++abi.so.1 "$runtime/libc++abi.so"

export CMAKE_BUILD_PARALLEL_LEVEL=4
export LLVM_PARALLEL_LINK_JOBS=1
nice -n 15 ionice -c 3 "$workspace/progress/R33/tools/tizen-clang++" \
  -O0 -g -pthread -stdlib=libc++ \
  -nostdinc++ -I"$build/include/c++/v1" \
  "$workspace/progress/R33/src/s1_forced_type_probe.cpp" \
  -L"$runtime" -Wl,-rpath,"$runtime" \
  -lc++ -lc++abi -o "$out/s1_libcxx"

nice -n 15 ionice -c 3 "$workspace/progress/R33/tools/tizen-clang++" \
  -O0 -g -pthread \
  "$workspace/progress/R33/src/s1_forced_type_probe.cpp" \
  -o "$out/s1_libstdcxx"

readelf -d "$out/s1_libcxx"
readelf -d "$out/s1_libstdcxx"
readelf --dyn-syms --wide "$out/s1_libcxx" | grep -E '__forced_unwind|__gxx_personality|UND'
readelf --dyn-syms --wide "$out/s1_libstdcxx" | grep -E '__forced_unwind|__gxx_personality|UND'
sha256sum "$out/s1_libcxx" "$out/s1_libstdcxx" "$runtime/libc++abi.so.1.0"
