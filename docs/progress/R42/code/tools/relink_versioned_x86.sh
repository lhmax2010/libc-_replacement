#!/bin/bash
set -u
workspace=/home/toolchain/development/libc++_replacement
build_dir="$workspace/tmp/R42/build-patched-x86_64"
runtime="$workspace/tmp/R42/runtime/patched/x86_64"
map="$workspace/packaging/libcxx-llvm22.map"
mkdir -p "$runtime"
mapfile -t objects < <(find "$build_dir/libcxxabi/src/CMakeFiles/cxxabi_shared_objects.dir" -type f -name '*.o' | sort)
printf 'OBJECT_COUNT=%d\n' "${#objects[@]}"
test "${#objects[@]}" -gt 0 || exit 3
cmd=(nice -n 15 ionice -c 3 "$workspace/progress/R33/tools/tizen-clang++"
  --target=x86_64-tizen-linux-gnu -fPIC -O2 -g -DNDEBUG -shared --rtlib=libgcc
  -Wl,-z,defs -Wl,-z,nodelete -nostdlib++ -Wl,--version-script="$map"
  -Wl,-soname,libc++abi.so.1 -o "$runtime/libc++abi.so.1.0"
  "${objects[@]}" -lpthread -lc -lgcc_s)
printf 'LINK_COMMAND='; printf '%q ' "${cmd[@]}"; printf '\n'
"${cmd[@]}" || exit $?
ln -sfn libc++abi.so.1.0 "$runtime/libc++abi.so.1"
ln -sfn libc++abi.so.1 "$runtime/libc++abi.so"
readelf -dW "$runtime/libc++abi.so.1.0"
readelf --dyn-syms -W "$runtime/libc++abi.so.1.0" | grep -E '__forced_unwind|__cxa_rethrow'
readelf --version-info -W "$runtime/libc++abi.so.1.0"
sha256sum "$runtime/libc++abi.so.1.0" "$map"
