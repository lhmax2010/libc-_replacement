#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
code="$workspace/progress/R78/code"
out="$workspace/progress/R78/build/armv7l"
wrapper="$workspace/progress/R68/tools/armv7l_lit_host_clangxx.sh"
old_root="$workspace/tmp/GBS-ROOT/R30-semantic-default-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0"
new_install="$workspace/tmp/R68/build-patched-armv7l/libcxx/test-suite-install"
gcc_cxx="$old_root/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/c++"

mkdir -p "$out"

"$wrapper" -std=c++17 -O1 -g -fPIC -shared \
  -nostdinc++ \
  -I "$gcc_cxx" \
  -I "$gcc_cxx/armv7l-tizen-linux-gnueabi" \
  -I "$gcc_cxx/backward" \
  "$code/old_plugin.cpp" \
  -nostdlib++ -L "$old_root/usr/lib" -lstdc++ \
  -pthread -Wl,-soname,libold_plugin.so \
  -o "$out/libold_plugin.so"

for source in new_main pen_wave_probe icu_probe; do
  "$wrapper" -std=c++17 -O1 -g \
    -nostdinc++ -I "$new_install/include/c++/v1" \
    "$code/$source.cpp" \
    -nostdlib++ -L "$new_install/lib" \
    -Wl,-rpath,'$ORIGIN/runtime' \
    -Wl,--no-as-needed -lc++ -lc++abi -Wl,--as-needed \
    -pthread -ldl -latomic \
    -o "$out/$source"
done
