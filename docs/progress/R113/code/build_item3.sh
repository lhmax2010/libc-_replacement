#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
build="$root/docs/progress/R113/build/item3"
single="$build/single_old"
dual="$build/dual"
r69=/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64
objects_root="$r69/libcxx/src/CMakeFiles/cxx_shared.dir"
compiler=/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++
headers=/home/toolchain/development/libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1
overlay=/home/toolchain/development/libcxx_runtime_validation/docs/progress/R111/build/item3/removed_overlay
probe="$root/docs/progress/R112/code/item1_oldcaller_cancel.cpp"

mkdir -p "$single" "$dual"

mapfile -t all_objects < <(find "$objects_root" -type f -name '*.o' | sort)
without_cv=()
for object in "${all_objects[@]}"; do
  if [[ "$object" != */condition_variable.cpp.o ]]; then
    without_cv+=("$object")
  fi
done
test "${#all_objects[@]}" -eq 54
test "${#without_cv[@]}" -eq 53

"$compiler" --target=x86_64-tizen-linux-gnu -std=c++23 -O2 -g -fPIC \
  -D_LIBCPP_BUILDING_LIBRARY -D_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER \
  -nostdinc++ -I "$headers" \
  -c "$root/docs/progress/R113/code/item3_dual_condition_variable.cpp" \
  -o "$build/item3_dual_condition_variable.o"

link_common=(
  --target=x86_64-tizen-linux-gnu -fPIC --rtlib=libgcc -shared
  -Wl,-z,defs -Wl,-z,nodelete -Wl,-soname,libc++.so.1
  -nostdlib++ -latomic "$r69/lib/libc++abi.so.1.0" -lpthread -lc -lgcc_s
)

"$compiler" "${link_common[@]}" \
  -Wl,--version-script="$root/docs/progress/R113/code/item3_single_old.map" \
  -o "$single/libc++.so.1.0" "${all_objects[@]}"

"$compiler" "${link_common[@]}" \
  -Wl,--version-script="$root/docs/progress/R113/code/item3_dual.map" \
  -o "$dual/libc++.so.1.0" "${without_cv[@]}" "$build/item3_dual_condition_variable.o"

for directory in "$single" "$dual"; do
  ln -sfn libc++.so.1.0 "$directory/libc++.so.1"
  ln -sfn libc++.so.1 "$directory/libc++.so"
  ln -sfn "$r69/lib/libc++abi.so.1.0" "$directory/libc++abi.so.1"
done

base=(-std=c++20 -O0 -g -fno-inline -fPIC -nostdinc++ -nostdlib++ -I "$headers" -DWORKER_NO_PUTS -DEXPECT_CLEAN=0)
"$compiler" "${base[@]}" -c "$probe" -o "$build/old_quiet.o"
"$compiler" -std=c++20 -O0 -g -fno-inline -fPIC -nostdinc++ -nostdlib++ \
  -I "$overlay/include" -I "$headers" -DWORKER_NO_PUTS -DEXPECT_CLEAN=1 \
  -c "$probe" -o "$build/new_quiet.o"

libs=(-lc++ -lc++abi -latomic -pthread -ldl)
"$compiler" "$build/old_quiet.o" -L "$single" -Wl,-rpath,"$dual:$r69/lib" "${libs[@]}" -o "$build/old_prebound_old"
"$compiler" "$build/old_quiet.o" -L "$dual" -Wl,-rpath,"$dual:$r69/lib" "${libs[@]}" -o "$build/old_relinked_dual"
"$compiler" "$build/new_quiet.o" -L "$dual" -Wl,-rpath,"$dual:$r69/lib" "${libs[@]}" -o "$build/new_linked_dual"
"$compiler" "$build/new_quiet.o" -L "$single" -Wl,-rpath,"$dual:$r69/lib" "${libs[@]}" -o "$build/new_rebuilt_before_dual"

file "$single/libc++.so.1.0" "$dual/libc++.so.1.0" "$build"/*_old "$build"/*_dual
sha256sum "$single/libc++.so.1.0" "$dual/libc++.so.1.0" "$build"/*.o "$build"/old_prebound_old \
  "$build"/old_relinked_dual "$build"/new_linked_dual "$build"/new_rebuilt_before_dual
