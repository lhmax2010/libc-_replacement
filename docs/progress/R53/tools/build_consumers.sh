#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
out="$workspace/tmp/R53/tests/$arch"
mkdir -p "$out"
case "$arch" in
  x86_64)
    cxx="$workspace/progress/R33/tools/tizen-clang++"
    buildroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
    product_abi="$workspace/tmp/R51/runtime/x86_64/libc++abi.so.1.0"
    opt=-O2
    ;;
  armv7l)
    cxx="$workspace/progress/R36/tools/armv7l-clang++"
    buildroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
    product_abi="$workspace/tmp/R51/build-product-armv7l/lib/libc++abi.so.1.0"
    opt=-O0
    ;;
  *) exit 2 ;;
esac
root="$buildroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
inc="$root/build/include/c++/v1"
lib="$root/build/lib"
overlay="$workspace/tmp/R53/overlay/include"
abi_inc="$workspace/tmp/R51/source-product-final/libcxxabi/include"
link_dir="$workspace/tmp/R53/link/$arch"
mkdir -p "$link_dir"
ln -sfn "$workspace/tmp/R53/build/$arch/libc++.so.1.0" "$link_dir/libc++.so"
ln -sfn "$product_abi" "$link_dir/libc++abi.so"

run() { printf 'COMMAND='; printf '%q ' "$@"; printf '\n'; nice -n 15 ionice -c 3 "$@"; printf 'EXIT_CODE=0\n'; }
for item in \
  cout:$workspace/docs/progress/R44b/code/tests/stdlib_cancel_probe.cpp \
  failbit:$workspace/progress/R46/src/failbit_control_probe.cpp \
  istream:$workspace/docs/progress/R47/code/tests/r47_cancel_probe.cpp; do
  name=${item%%:*}; src=${item#*:}
  run "$cxx" -std=c++17 "$opt" -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
    -I"$inc" "$src" -L"$lib" -Wl,-z,now -lc++ -lc++abi -o "$out/${name}_stale"
  run "$cxx" -std=c++17 "$opt" -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
    -I"$overlay" -I"$abi_inc" -I"$inc" "$src" -L"$link_dir" -L"$lib" -Wl,-z,now -lc++ -lc++abi -o "$out/${name}_rebuilt"
  run "$cxx" -std=c++17 "$opt" -g -fexceptions -pthread "$src" -Wl,-z,now -o "$out/${name}_libstdcxx"
done
sha256sum "$out"/*
