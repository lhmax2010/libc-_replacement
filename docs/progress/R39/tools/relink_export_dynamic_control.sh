#!/bin/bash
set -u

workspace=/home/toolchain/development/libc++_replacement
gate_log="$workspace/progress/R39/commands/039_resource_gate_before_export_dynamic_control.log"
compiler="$workspace/progress/R36/tools/armv7l-clang++"
object="$workspace/tmp/R38/probes-armv7l/s1_libcxx_strict.o"
static_libcxx="$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++.a"
abi_dir="$workspace/tmp/R36/build-armv7l/lib"
output="$workspace/tmp/R39/r38_static_export_dynamic"

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

if [ -e "$output" ]; then
  printf 'FAIL_CLOSED_EXISTING_OUTPUT=%s\n' "$output"
  exit 96
fi
export CMAKE_BUILD_PARALLEL_LEVEL=2
export LLVM_PARALLEL_LINK_JOBS=1
printf 'CMAKE_BUILD_PARALLEL_LEVEL=%s\nLLVM_PARALLEL_LINK_JOBS=%s\n' "$CMAKE_BUILD_PARALLEL_LEVEL" "$LLVM_PARALLEL_LINK_JOBS"
printf 'LINK_COMMAND='
printf '%q ' nice -n 15 ionice -c 3 "$compiler" "$object" -nostdlib++ "$static_libcxx" -L"$abi_dir" '-Wl,-rpath,$ORIGIN' -Wl,-z,now -Wl,--export-dynamic -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$output"
printf '\n'
nice -n 15 ionice -c 3 "$compiler" "$object" -nostdlib++ "$static_libcxx" -L"$abi_dir" '-Wl,-rpath,$ORIGIN' -Wl,-z,now -Wl,--export-dynamic -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$output"
link_rc=$?
printf 'LINK_EXIT_CODE=%d\n' "$link_rc"
if [ "$link_rc" -ne 0 ]; then
  exit "$link_rc"
fi
sha256sum "$object" "$static_libcxx" "$output"
file "$output"
readelf -dW "$output"
readelf -Ws "$output" | grep -E '__forced_unwind|_ZT[IS]N10__cxxabiv115__forced_unwindE' || true
readelf --dyn-syms -W "$output" | grep -E '__forced_unwind|_ZT[IS]N10__cxxabiv115__forced_unwindE' || true
