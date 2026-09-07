#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
build="$root/docs/progress/R112/build/item1"
overlay="$build/removed_overlay"
compiler=/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++
headers=/home/toolchain/development/libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1
runtime=/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib
source="$root/docs/progress/R112/code/item1_oldcaller_cancel.cpp"

mkdir -p "$build"
python3 "$root/docs/progress/R112/code/make_removed_overlay.py" \
  --header "$headers/__condition_variable/condition_variable.h" \
  --source "$root/codes/llvm/libcxx/src/condition_variable.cpp" \
  --output "$overlay"

"$compiler" -std=c++20 -O0 -g -fno-inline -fPIC -shared -nostdinc++ \
  -I "$overlay/include" -I "$headers" -nostdlib++ \
  "$overlay/condition_variable_removed.cpp" \
  -L "$runtime" -Wl,-rpath,"$runtime" -lc++ -lc++abi -latomic -pthread \
  -Wl,-soname,libcv_removed_r112.so -o "$build/libcv_removed_r112.so"

base=(-std=c++20 -O0 -g -fno-inline -fPIC -nostdinc++ -nostdlib++ -I "$headers")
libs=(-L "$runtime" -Wl,-rpath,"$runtime" -lc++ -lc++abi -latomic -pthread -ldl)
removed=(-L "$build" -Wl,--no-as-needed -lcv_removed_r112 -L "$runtime" -Wl,-rpath,"$build:$runtime" -lc++ -lc++abi -latomic -pthread -ldl)

"$compiler" "${base[@]}" -DEXPECT_CLEAN=-1 -c "$source" -o "$build/oldcaller.o"
"$compiler" "${base[@]}" -DWORKER_NO_PUTS -DEXPECT_CLEAN=-1 -c "$source" -o "$build/oldcaller_quiet.o"
"$compiler" "${base[@]}" -fno-exceptions -DEXPECT_CLEAN=-1 -c "$source" -o "$build/fno_oldcaller.o"
"$compiler" "${base[@]}" -fno-exceptions -DWORKER_NO_PUTS -DEXPECT_CLEAN=-1 -c "$source" -o "$build/fno_oldcaller_quiet.o"
"$compiler" -I "$overlay/include" "${base[@]}" -DEXPECT_CLEAN=1 -c "$source" -o "$build/newcaller.o"
"$compiler" -I "$overlay/include" "${base[@]}" -DWORKER_NO_PUTS -DEXPECT_CLEAN=1 -c "$source" -o "$build/newcaller_quiet.o"

"$compiler" "$build/oldcaller.o" "${libs[@]}" -o "$build/oldcaller_retained"
"$compiler" "$build/oldcaller.o" "${removed[@]}" -o "$build/oldcaller_removed"
"$compiler" "$build/oldcaller_quiet.o" "${removed[@]}" -o "$build/oldcaller_quiet_removed"
"$compiler" "$build/fno_oldcaller.o" "${removed[@]}" -o "$build/fno_oldcaller_removed"
"$compiler" "$build/fno_oldcaller_quiet.o" "${removed[@]}" -o "$build/fno_oldcaller_quiet_removed"
"$compiler" "$build/newcaller.o" "${removed[@]}" -o "$build/newcaller_removed"
"$compiler" "$build/newcaller_quiet.o" "${removed[@]}" -o "$build/newcaller_quiet_removed"

file "$build"/*.o "$build"/libcv_removed_r112.so "$build"/oldcaller_retained "$build"/*caller_removed
sha256sum "$build"/*.o "$build"/libcv_removed_r112.so "$build"/oldcaller_retained "$build"/*caller_removed
