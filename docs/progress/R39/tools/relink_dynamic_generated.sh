#!/bin/bash
set -u

workspace=/home/toolchain/development/libc++_replacement
gate_log="$workspace/progress/R39/commands/031_resource_gate_before_generated_dynamic_relink_v2.log"
compiler="$workspace/progress/R36/tools/armv7l-clang++"
static_libcxx="$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++.a"
abi_dir="$workspace/tmp/R36/build-armv7l/lib"
object="$workspace/tmp/R38/probes-armv7l/s1_libcxx_strict.o"
out_dir="$workspace/tmp/R39/generated-libcxx-v2"
shared="$out_dir/libc++.so.1"
binary="$workspace/tmp/R39/r38_dynamic_test_v2"

printf 'RESOURCE_GATE_COMMAND='
printf '%q ' "$workspace/progress/R39/tools/run_logged.sh" "$gate_log" bash "$workspace/progress/R39/tools/resource_gate.sh"
printf '\n'
"$workspace/progress/R39/tools/run_logged.sh" "$gate_log" bash "$workspace/progress/R39/tools/resource_gate.sh"
gate_rc=$?
printf 'RESOURCE_GATE_EXPLICIT_RC=%d\n' "$gate_rc"
if [ "$gate_rc" -ne 0 ]; then
  printf 'RED_STOP_RESOURCE\n'
  exit "$gate_rc"
fi

if [ -e "$out_dir" ] || [ -e "$binary" ]; then
  printf 'FAIL_CLOSED_EXISTING_OUTPUT=%s OR %s\n' "$out_dir" "$binary"
  exit 96
fi
mkdir -p "$out_dir"
export CMAKE_BUILD_PARALLEL_LEVEL=2
export LLVM_PARALLEL_LINK_JOBS=1
printf 'CMAKE_BUILD_PARALLEL_LEVEL=%s\nLLVM_PARALLEL_LINK_JOBS=%s\n' "$CMAKE_BUILD_PARALLEL_LEVEL" "$LLVM_PARALLEL_LINK_JOBS"

printf 'SHARED_LIBCXX_LINK_COMMAND='
printf '%q ' nice -n 15 ionice -c 3 "$compiler" -shared -nostdlib++ -Wl,-soname,libc++.so.1 -Wl,--whole-archive "$static_libcxx" -Wl,--no-whole-archive -L"$abi_dir" -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$shared"
printf '\n'
nice -n 15 ionice -c 3 "$compiler" -shared -nostdlib++ -Wl,-soname,libc++.so.1 -Wl,--whole-archive "$static_libcxx" -Wl,--no-whole-archive -L"$abi_dir" -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$shared"
shared_rc=$?
printf 'SHARED_LIBCXX_LINK_EXIT_CODE=%d\n' "$shared_rc"
if [ "$shared_rc" -ne 0 ]; then
  exit "$shared_rc"
fi
ln -s libc++.so.1 "$out_dir/libc++.so"
symlink_rc=$?
printf 'LIBCXX_LINKER_SYMLINK_EXIT_CODE=%d\n' "$symlink_rc"
if [ "$symlink_rc" -ne 0 ]; then
  exit "$symlink_rc"
fi

printf 'TEST_LINK_COMMAND='
printf '%q ' nice -n 15 ionice -c 3 "$compiler" "$object" -nostdlib++ -L"$out_dir" -L"$abi_dir" '-Wl,-rpath,$ORIGIN' -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$binary"
printf '\n'
nice -n 15 ionice -c 3 "$compiler" "$object" -nostdlib++ -L"$out_dir" -L"$abi_dir" '-Wl,-rpath,$ORIGIN' -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$binary"
test_rc=$?
printf 'TEST_LINK_EXIT_CODE=%d\n' "$test_rc"
if [ "$test_rc" -ne 0 ]; then
  exit "$test_rc"
fi

sha256sum "$static_libcxx" "$shared" "$object" "$binary"
file "$shared" "$binary"
for file in "$shared" "$binary"; do
  printf '=== ELF=%s ===\n' "$file"
  readelf -dW "$file"
  readelf -Ws "$file" | grep -E '__forced_unwind|_ZT[IS]N10__cxxabiv115__forced_unwindE' || true
  readelf --dyn-syms -W "$file" | grep -E '__forced_unwind|_ZT[IS]N10__cxxabiv115__forced_unwindE' || true
done
