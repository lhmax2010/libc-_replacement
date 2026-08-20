#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
source_file="$workspace/progress/R52/src/noncancel_probe.cpp"
overlay="$workspace/tmp/R52/overlay/include"
out="$workspace/tmp/R52/noncancel/$arch"
mkdir -p "$out"

run() { printf 'COMMAND='; printf '%q ' "$@"; printf '\n'; nice -n 15 ionice -c 3 "$@"; printf 'EXIT_CODE=%d\n' "$?"; }

if [[ $arch == x86_64 ]]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  headers="$workspace/tmp/R51/build-product-x86_64/include/c++/v1"
  abi="$workspace/tmp/R51/runtime/x86_64"
  libcxx="$workspace/tmp/R52/rebuilt-libcxx/x86_64"
  sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
  for variant in before after; do
    includes=(-I "$headers")
    [[ $variant == after ]] && includes=(-I "$overlay" -I "$headers")
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      "${includes[@]}" "$source_file" -L "$abi" -L "$libcxx" -L "$sysroot/usr/lib64" \
      -Wl,-z,now -Wl,-rpath-link,"$libcxx" -lc++ -lc++abi -o "$out/$variant"
  done
elif [[ $arch == armv7l ]]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  headers="$workspace/tmp/R51/build-product-armv7l/include/c++/v1"
  libcxx="$workspace/tmp/R52/rebuilt-libcxx/armv7l"
  for variant in before after; do
    includes=(-I "$headers")
    [[ $variant == after ]] && includes=(-I "$overlay" -I "$headers")
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      "${includes[@]}" -fPIC -fvisibility=hidden -c "$source_file" -o "$out/$variant.o"
    run "$cxx" -shared "$out/$variant.o" -nostdlib++ -L "$libcxx" -Wl,-z,now \
      -Wl,--allow-shlib-undefined -Wl,-rpath-link,"$libcxx" -lc++ -lpthread -ldl \
      -lm -latomic -lc -lgcc_s -lgcc -o "$out/$variant.so"
  done
  run "$cxx" -x c -std=c11 -O0 -g -nostdlib++ -c "$workspace/progress/R52/src/noncancel_launcher.c" -o "$out/launcher.o"
  run "$cxx" "$out/launcher.o" -nostdlib++ -ldl -lc -lgcc_s -lgcc -o "$out/launcher"
else
  exit 2
fi

find "$out" -maxdepth 1 -type f -perm -0100 -exec file {} \; -exec sha256sum {} \; -exec readelf -dW {} \;
