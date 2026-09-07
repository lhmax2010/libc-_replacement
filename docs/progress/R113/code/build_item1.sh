#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
build="$root/docs/progress/R113/build/item1"
overlay="$build/removed_overlay"
compiler=/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++
headers=/home/toolchain/development/libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1
runtime=/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib

mkdir -p "$build"
python3 "$root/docs/progress/R113/code/make_removed_overlay.py" \
  --header "$headers/__condition_variable/condition_variable.h" \
  --source "$root/codes/llvm/libcxx/src/condition_variable.cpp" \
  --output "$overlay"

common=(
  -std=c++20 -O0 -g -fno-inline -fPIC -nostdinc++ -nostdlib++
  -I "$overlay/include" -I "$headers"
)

"$compiler" -std=c++20 -O0 -g -fno-inline -fPIC -shared -nostdinc++ \
  -I "$overlay/include" -I "$headers" -nostdlib++ \
  "$overlay/condition_variable_removed.cpp" \
  "$root/codes/llvm/libcxx/src/mutex.cpp" \
  "$root/codes/llvm/libcxx/src/shared_mutex.cpp" \
  "$root/codes/llvm/libcxx/src/future.cpp" \
  "$root/codes/llvm/libcxx/src/system_error.cpp" \
  -L "$runtime" -Wl,-rpath,"$runtime" -lc++ -lc++abi -latomic -pthread \
  -Wl,-soname,libfacility_removed_r113.so -o "$build/libfacility_removed_r113.so"

"$compiler" "$root/docs/progress/R113/code/item1_facility_cancel.cpp" "${common[@]}" \
  -L "$build" -Wl,--no-as-needed -lfacility_removed_r113 \
  -L "$runtime" -Wl,-rpath,"$build:$runtime" \
  -lc++ -lc++abi -latomic -pthread -Wl,-export-dynamic \
  -o "$build/item1_facility_cancel"

file "$build/libfacility_removed_r113.so" "$build/item1_facility_cancel"
sha256sum "$build/libfacility_removed_r113.so" "$build/item1_facility_cancel"
