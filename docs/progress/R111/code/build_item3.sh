#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
build="$root/docs/progress/R111/build/item3"
overlay="$build/removed_overlay"
compiler=/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++
headers=/home/toolchain/development/libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1
runtime=/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib

mkdir -p "$build"
python3 "$root/docs/progress/R111/code/make_removed_overlay.py" \
  --header "$headers/__condition_variable/condition_variable.h" \
  --source "$root/codes/llvm/libcxx/src/condition_variable.cpp" \
  --output "$overlay"

common=(
  -std=c++20 -O0 -g -fno-inline -fPIC -nostdinc++ -nostdlib++
  -I "$headers"
  -L "$runtime" -Wl,-rpath,"$runtime"
  -lc++ -lc++abi -latomic -pthread
)

"$compiler" -std=c++20 -O0 -g -fno-inline -fPIC -shared -nostdinc++ \
  -I "$overlay/include" -I "$headers" -nostdlib++ \
  "$overlay/condition_variable_removed.cpp" \
  -L "$runtime" -Wl,-rpath,"$runtime" -lc++ -lc++abi -latomic -pthread \
  -Wl,-soname,libcv_removed_r111.so -o "$build/libcv_removed_r111.so"

for source in condition_cancel_probe uncaught_error_probe; do
  "$compiler" "$root/docs/progress/R111/code/${source}.cpp" "${common[@]}" \
    -Wl,-export-dynamic -o "$build/${source}_retained"
  "$compiler" -I "$overlay/include" -I "$headers" \
    "$root/docs/progress/R111/code/${source}.cpp" \
    -std=c++20 -O0 -g -fno-inline -fPIC -nostdinc++ -nostdlib++ \
    -L "$build" -Wl,--no-as-needed -lcv_removed_r111 \
    -L "$runtime" -Wl,-rpath,"$build:$runtime" \
    -lc++ -lc++abi -latomic -pthread -Wl,-export-dynamic \
    -o "$build/${source}_removed"
done

file "$build"/libcv_removed_r111.so "$build"/condition_cancel_probe_* "$build"/uncaught_error_probe_*
sha256sum "$build"/libcv_removed_r111.so "$build"/condition_cancel_probe_* "$build"/uncaught_error_probe_*
