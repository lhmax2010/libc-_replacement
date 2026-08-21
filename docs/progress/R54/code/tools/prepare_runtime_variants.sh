#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
arch=${1:?arch required}
root="$workspace/tmp/R54/runtime/$arch"
mkdir -p "$root"
if [[ $arch == x86_64 ]]; then
  baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
  a_abi="$workspace/tmp/R51/build-layer-a-x86_64/lib/libc++abi.so.1.0"
  b_abi="$workspace/tmp/R51/build-layer-b-x86_64/lib/libc++abi.so.1.0"
elif [[ $arch == armv7l ]]; then
  baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
  a_abi="$workspace/tmp/R54/build-a-armv7l/lib/libc++abi.so.1.0"
  b_abi="$workspace/tmp/R54/build-b-armv7l/lib/libc++abi.so.1.0"
else
  exit 2
fi
full="$workspace/tmp/R54/build-all-$arch/lib"
make_variant() {
  local name=$1 cxx=$2 abi=$3 dir="$root/$1"
  mkdir -p "$dir"
  ln -sfn "$cxx" "$dir/libc++.so.1"
  ln -sfn "$abi" "$dir/libc++abi.so.1"
  printf '%s\t%s\t%s\n' "$name" "$cxx" "$abi"
}
make_variant baseline "$baseline/libc++.so.1.0" "$baseline/libc++abi.so.1.0"
make_variant a "$baseline/libc++.so.1.0" "$a_abi"
make_variant ab "$baseline/libc++.so.1.0" "$full/libc++abi.so.1.0"
make_variant abc "$full/libc++.so.1.0" "$full/libc++abi.so.1.0"
make_variant bc "$full/libc++.so.1.0" "$b_abi"
find "$root" -type l -printf '%p -> %l\n' | sort
sha256sum "$baseline/libc++.so.1.0" "$baseline/libc++abi.so.1.0" \
  "$a_abi" "$b_abi" "$full/libc++.so.1.0" "$full/libc++abi.so.1.0"
