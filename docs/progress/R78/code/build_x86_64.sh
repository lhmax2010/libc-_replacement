#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
code="$workspace/progress/R78/code"
out="$workspace/progress/R78/build/x86_64"
wrapper="$workspace/progress/R33/tools/tizen-clang++"
new_install="$workspace/tmp/R68/build-patched-x86_64/libcxx/test-suite-install"

mkdir -p "$out/runtime"

/usr/bin/g++ -std=c++17 -O1 -g -fPIC -shared \
  "$code/old_plugin.cpp" -pthread \
  -Wl,-soname,libold_plugin.so \
  -o "$out/libold_plugin.so"

for source in new_main pen_wave_probe; do
  "$wrapper" -std=c++17 -O1 -g \
    -nostdinc++ -I "$new_install/include/c++/v1" \
    "$code/$source.cpp" \
    -nostdlib++ -L "$new_install/lib" \
    -Wl,-rpath,'$ORIGIN/runtime' \
    -Wl,--no-as-needed -lc++ -lc++abi -Wl,--as-needed \
    -pthread -ldl \
    -o "$out/$source"
done

cp -a "$new_install/lib/libc++.so.1.0" "$out/runtime/"
cp -a "$new_install/lib/libc++abi.so.1.0" "$out/runtime/"
ln -sfn libc++.so.1.0 "$out/runtime/libc++.so.1"
ln -sfn libc++abi.so.1.0 "$out/runtime/libc++abi.so.1"
