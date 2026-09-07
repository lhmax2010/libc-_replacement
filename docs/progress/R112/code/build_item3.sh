#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
build="$root/docs/progress/R112/build/item3"
overlay="$build/overlay/include"
compiler=/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++
headers=/home/toolchain/development/libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1
runtime=/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib
source="$root/docs/progress/R112/code/item3_wbuffer_loss.cpp"
mkdir -p "$build"

python3 "$root/docs/progress/R112/code/make_wbuffer_overlay.py" \
  --header "$root/codes/llvm/libcxx/include/__locale_dir/wbuffer_convert.h" \
  --output "$overlay"

common=(-std=c++20 -O0 -g -fno-inline -nostdinc++ -nostdlib++ -I "$headers" \
  -D_LIBCPP_ENABLE_CXX17_REMOVED_WSTRING_CONVERT \
  -L "$runtime" -Wl,-rpath,"$runtime" -lc++ -lc++abi -latomic -pthread)
"$compiler" "$source" "${common[@]}" -o "$build/retained"
"$compiler" -I "$overlay" "$source" "${common[@]}" -o "$build/removed"
file "$build/retained" "$build/removed"
sha256sum "$build/retained" "$build/removed"
