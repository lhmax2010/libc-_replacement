#!/bin/bash
set -u

if [ "$#" -ne 1 ]; then printf 'usage: %s BUILD_DIR\n' "$0" >&2; exit 2; fi
build_dir=$1
workspace=/home/toolchain/development/libc++_replacement
map="$workspace/packaging/libcxx-llvm22.map"
runtime="$workspace/tmp/R41/runtime/x86_64"
output="$runtime/libc++abi.so.1.0"
mkdir -p "$runtime"
mapfile -t objects < <(find "$build_dir/libcxxabi/src/CMakeFiles/cxxabi_shared_objects.dir" -type f -name '*.o' | sort)
printf 'OBJECT_COUNT=%s\n' "${#objects[@]}"
test "${#objects[@]}" -gt 0 || exit 3
args=(--target=x86_64-tizen-linux-gnu -fPIC -O2 -g -DNDEBUG -shared --rtlib=libgcc
  -Wl,-z,defs -Wl,-z,nodelete -nostdlib++ -Wl,--version-script="$map"
  -Wl,-soname,libc++abi.so.1 -o "$output" "${objects[@]}" -lpthread -lc -lgcc_s)
printf 'LINK_COMMAND='; printf ' %q' nice -n 15 ionice -c 3 "$workspace/progress/R33/tools/tizen-clang++" "${args[@]}"; printf '\n'
nice -n 15 ionice -c 3 "$workspace/progress/R33/tools/tizen-clang++" "${args[@]}"
ln -sfn libc++abi.so.1.0 "$runtime/libc++abi.so.1"
ln -sfn libc++abi.so.1 "$runtime/libc++abi.so"
readelf -dW "$output"
readelf --dyn-syms -W "$output" | grep -E '__forced_unwind|__cxa_rethrow'
readelf --version-info -W "$output"
sha256sum "$output" "$map"

