#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
out="$workspace/tmp/R58/probes/$arch"
source="$workspace/progress/R58/src/normal_paths_probe.cpp"
if [[ $arch == x86_64 ]]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
elif [[ $arch == armv7l ]]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
else
  exit 2
fi
baseline="$sysroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
full_headers="$workspace/tmp/R58/build-full-$arch/include/c++/v1"
full_libs="$workspace/tmp/R58/build-full-$arch/lib"

run() { printf 'COMMAND='; printf '%q ' "$@"; printf '\n'; nice -n 15 ionice -c 3 "$@"; printf 'EXIT_CODE=%d\n' "$?"; }
if [[ $arch == x86_64 ]]; then
  for variant in baseline full; do
    if [[ $variant == full ]]; then headers=$full_headers; libs=$full_libs; else headers="$baseline/include/c++/v1"; libs="$baseline/lib"; fi
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$headers" "$source" -L"$libs" -L"$baseline/lib" -Wl,-z,now \
      -Wl,-rpath-link,"$libs" -Wl,-rpath-link,"$baseline/lib" -lc++ -lc++abi \
      -o "$out/normal_$variant"
  done
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread "$source" -Wl,-z,now \
    -o "$out/normal_reference"
else
  for variant in baseline full; do
    if [[ $variant == full ]]; then headers=$full_headers; libs=$full_libs; else headers="$baseline/include/c++/v1"; libs="$baseline/lib"; fi
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$headers" -c "$source" -o "$out/normal_$variant.o"
    run "$cxx" "$out/normal_$variant.o" -nostdlib++ -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
      -o "$out/normal_$variant"
  done
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -c "$source" -o "$out/normal_reference.o"
  run "$cxx" "$out/normal_reference.o" -pthread -Wl,-z,now -o "$out/normal_reference"
fi
sha256sum "$out"/normal_*

