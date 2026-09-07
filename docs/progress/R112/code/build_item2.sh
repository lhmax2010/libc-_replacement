#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
build="$root/docs/progress/R112/build/item2"
overlay="$root/docs/progress/R112/build/item1/removed_overlay/include"
compiler=/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++
headers=/home/toolchain/development/libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1
runtime=/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib
tu="$root/docs/progress/R112/code/item2_comdat_tu.cpp"
main="$root/docs/progress/R112/code/item2_comdat_main.cpp"
mkdir -p "$build"

common=(-std=c++20 -O0 -g -fno-inline -fPIC -nostdinc++ -nostdlib++ -I "$headers")
"$compiler" "${common[@]}" -DENTRY_NAME=old_entry -c "$tu" -o "$build/old_tu.o"
"$compiler" -I "$overlay" "${common[@]}" -DENTRY_NAME=new_entry -c "$tu" -o "$build/new_tu.o"
"$compiler" "${common[@]}" -c "$main" -o "$build/main.o"

link=(-L "$runtime" -Wl,-rpath,"$runtime" -lc++ -lc++abi -latomic -pthread)
"$compiler" "$build/main.o" "$build/old_tu.o" "$build/new_tu.o" "${link[@]}" \
  -Wl,-Map,"$build/old_first.map" -o "$build/old_first"
"$compiler" "$build/main.o" "$build/new_tu.o" "$build/old_tu.o" "${link[@]}" \
  -Wl,-Map,"$build/new_first.map" -o "$build/new_first"

ar crs "$build/mixed_old_new.a" "$build/old_tu.o" "$build/new_tu.o"
"$compiler" "$build/main.o" -Wl,--whole-archive "$build/mixed_old_new.a" -Wl,--no-whole-archive \
  "${link[@]}" -Wl,-Map,"$build/archive_old_first.map" -o "$build/archive_old_first"

file "$build"/*.o "$build"/old_first "$build"/new_first "$build"/archive_old_first "$build"/mixed_old_new.a
sha256sum "$build"/*.o "$build"/old_first "$build"/new_first "$build"/archive_old_first "$build"/mixed_old_new.a
