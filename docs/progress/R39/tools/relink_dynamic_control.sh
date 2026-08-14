#!/bin/bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
gate_log="$workspace/progress/R39/commands/025_resource_gate_before_relink.log"
extract="$workspace/tmp/R39/dynamic-libcxx"
rpm="$workspace/artifacts/rebuild_2218_c2/armv7l/libc++-22.1.8-1.armv7l.rpm"
object="$workspace/tmp/R38/probes-armv7l/s1_libcxx_strict.o"
abi_dir="$workspace/tmp/R36/build-armv7l/lib"
output="$workspace/tmp/R39/r38_dynamic_test"
compiler="$workspace/progress/R36/tools/armv7l-clang++"

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

if [ -e "$extract" ] || [ -e "$output" ]; then
  printf 'FAIL_CLOSED_EXISTING_OUTPUT=%s OR %s\n' "$extract" "$output"
  exit 96
fi
mkdir -p "$extract"
printf 'EXTRACT_COMMAND=cd %q && rpm2cpio %q | cpio -idmv\n' "$extract" "$rpm"
(cd "$extract" && rpm2cpio "$rpm" | cpio -idmv)
extract_rc=$?
printf 'EXTRACT_EXIT_CODE=%d\n' "$extract_rc"
if [ "$extract_rc" -ne 0 ]; then
  exit "$extract_rc"
fi

export CMAKE_BUILD_PARALLEL_LEVEL=2
export LLVM_PARALLEL_LINK_JOBS=1
printf 'CMAKE_BUILD_PARALLEL_LEVEL=%s\nLLVM_PARALLEL_LINK_JOBS=%s\n' "$CMAKE_BUILD_PARALLEL_LEVEL" "$LLVM_PARALLEL_LINK_JOBS"
printf 'LINK_COMMAND='
printf '%q ' nice -n 15 ionice -c 3 "$compiler" "$object" -nostdlib++ -L"$extract/usr/lib" -L"$abi_dir" '-Wl,-rpath,$ORIGIN' -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$output"
printf '\n'
nice -n 15 ionice -c 3 "$compiler" "$object" -nostdlib++ -L"$extract/usr/lib" -L"$abi_dir" '-Wl,-rpath,$ORIGIN' -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$output"
link_rc=$?
printf 'LINK_EXIT_CODE=%d\n' "$link_rc"
if [ "$link_rc" -ne 0 ]; then
  exit "$link_rc"
fi

sha256sum "$output" "$extract/usr/lib/libc++.so.1.0" "$object"
file "$output" "$extract/usr/lib/libc++.so.1.0"
readelf -dW "$output"
readelf -Ws "$output" | grep -E '__forced_unwind|_ZT[IS]N10__cxxabiv115__forced_unwindE' || true
readelf --dyn-syms -W "$output" | grep -E '__forced_unwind|_ZT[IS]N10__cxxabiv115__forced_unwindE' || true
