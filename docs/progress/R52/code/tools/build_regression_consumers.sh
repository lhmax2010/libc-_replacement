#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
source_file="$workspace/progress/R51/src/s1_rethrow_probe.cpp"
overlay="$workspace/tmp/R52/overlay/include"
out="$workspace/tmp/R52/regression/$arch"
mkdir -p "$out"

run() {
  printf 'COMMAND='; printf '%q ' "$@"; printf '\n'
  set +e
  nice -n 15 ionice -c 3 "$@"
  rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

if [[ $arch == x86_64 ]]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  headers="$workspace/tmp/R51/build-product-x86_64/include/c++/v1"
  abi="$workspace/tmp/R51/runtime/x86_64"
  libcxx="$workspace/tmp/R52/rebuilt-libcxx/x86_64"
  sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I "$overlay" -I "$headers" "$source_file" \
    -L "$abi" -L "$libcxx" -L "$sysroot/usr/lib64" -Wl,-z,now \
    -Wl,-rpath-link,"$libcxx" -lc++ -lc++abi -o "$out/libcxx"
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread "$source_file" \
    -Wl,-z,now -o "$out/libstdcxx"
elif [[ $arch == armv7l ]]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  headers="$workspace/tmp/R51/build-product-armv7l/include/c++/v1"
  abi="$workspace/tmp/R51/build-product-armv7l/lib"
  static_libcxx="$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++.a"
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I "$overlay" -I "$headers" \
    -Dprivate_1=unwinder_cache.reserved1 -c "$source_file" -o "$out/libcxx.o"
  run "$cxx" "$out/libcxx.o" -nostdlib++ "$static_libcxx" -L "$abi" \
    -Wl,-z,now -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
    -o "$out/libcxx"
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
    -Dprivate_1=unwinder_cache.reserved1 -c "$source_file" -o "$out/libstdcxx.o"
  run "$cxx" "$out/libstdcxx.o" -pthread -Wl,-z,now -o "$out/libstdcxx"
else
  printf 'ERROR: unsupported architecture: %s\n' "$arch" >&2
  exit 2
fi

for binary in "$out/libcxx" "$out/libstdcxx"; do
  file "$binary"
  sha256sum "$binary"
  readelf -dW "$binary"
done
