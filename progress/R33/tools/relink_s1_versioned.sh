#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
build=$workspace/tmp/R33/build-s1-r3
map=$workspace/packaging/libcxx-llvm22.map
output=$build/lib/libc++abi.so.1.0.versioned
test ! -e "$output"
mapfile -t objects < <(find "$build/libcxxabi/src/CMakeFiles/cxxabi_shared_objects.dir" -type f -name '*.o' | sort)
test "${#objects[@]}" -eq 18
export CMAKE_BUILD_PARALLEL_LEVEL=4
export LLVM_PARALLEL_LINK_JOBS=1
nice -n 15 ionice -c 3 "$workspace/progress/R33/tools/tizen-clang++" \
  --target=x86_64-tizen-linux-gnu -fPIC -O2 -g -DNDEBUG -shared \
  --rtlib=libgcc -Wl,-z,defs -Wl,-z,nodelete -nostdlib++ \
  -Wl,--version-script="$map" -Wl,-soname,libc++abi.so.1 \
  -o "$output" "${objects[@]}" -lpthread -lc -lgcc_s
readelf -d "$output"
readelf --version-info "$output"
readelf --dyn-syms --wide "$output" | grep -E '__forced_unwind|__gxx_personality'
sha256sum "$output" "$map"
