#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
arch=${1:?arch required}
variant=${2:?variant required: baseline or full}
out="$workspace/tmp/R54/probes/$arch/$variant"
mkdir -p "$out"
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
if [[ $variant == full ]]; then
  headers="$workspace/tmp/R54/build-all-$arch/include/c++/v1"
  libs="$workspace/tmp/R54/build-all-$arch/lib"
elif [[ $variant == baseline ]]; then
  headers="$baseline/include/c++/v1"
  libs="$baseline/lib"
else
  exit 2
fi
run() { printf 'COMMAND='; printf '%q ' "$@"; printf '\n'; nice -n 15 ionice -c 3 "$@"; rc=$?; printf 'EXIT_CODE=%d\n' "$rc"; return "$rc"; }
for source in facility_probe concurrency_probe catchall_rethrow_probe; do
  if [[ $arch == armv7l ]]; then
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$headers" -c "$workspace/progress/R54/src/$source.cpp" -o "$out/${source}_libcxx.o"
    run "$cxx" "$out/${source}_libcxx.o" -nostdlib++ -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
      -o "$out/${source}_libcxx"
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      -c "$workspace/progress/R54/src/$source.cpp" -o "$out/${source}_libstdcxx.o"
    run "$cxx" "$out/${source}_libstdcxx.o" -pthread -Wl,-z,now \
      -o "$out/${source}_libstdcxx"
  else
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$headers" "$workspace/progress/R54/src/$source.cpp" -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -Wl,-rpath-link,"$libs" -Wl,-rpath-link,"$baseline/lib" \
      -lc++ -lc++abi -o "$out/${source}_libcxx"
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      "$workspace/progress/R54/src/$source.cpp" -Wl,-z,now -o "$out/${source}_libstdcxx"
  fi
done
sha256sum "$out"/*
for binary in "$out"/*; do
  [[ -x $binary ]] || continue
  file "$binary"
  readelf -dW "$binary"
done
