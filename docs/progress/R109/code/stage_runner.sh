#!/bin/bash
set -euo pipefail
root=$(cd "$(dirname "$0")/../../../.." && pwd)
cd "$root"
log=docs/progress/R109/code/run_logged.sh
tables=docs/progress/R109/tables
build=docs/progress/R109/build
replacement=/home/toolchain/development/libc++_replacement

quiet_log() {
  "$log" "$@" >/dev/null 2>&1
}

phase=${1:-}
case "$phase" in
  w1_scan)
    quiet_log 003 snapshot_w1_scan_scripts sha256sum \
      docs/progress/R109/code/run_logged.sh \
      docs/progress/R109/code/stage_runner.sh \
      docs/progress/R109/code/w1_scan_callers.py \
      docs/progress/R109/code/w1_caller_control.cpp \
      docs/progress/R109/code/capture_lines.py
    quiet_log 004 scan_w1_callers nice -n 15 ionice -c 3 python3 \
      docs/progress/R109/code/w1_scan_callers.py \
      --facility-hits docs/progress/R72/raw/facility_usage_hits.tsv \
      --source-root "$replacement/tmp/corpus/extracted" \
      --control-root docs/progress/R109/code \
      --output "$tables/w1_caller_candidates.tsv"
    quiet_log 005 capture_w1_implementation_evidence python3 \
      docs/progress/R109/code/capture_lines.py \
      --output "$tables/w1_implementation_evidence.tsv" \
      "codes/llvm/libcxx/src/condition_variable.cpp:29-63" \
      "$replacement/tmp/R72/glibc-2.40/nptl/pthread_cond_wait.c:350-370,379-425,600-610,630-682"
    printf 'phase=1 scan=ready\n'
    ;;
  w1_behavior)
    baseline="$replacement/tmp/R68/build-baseline-x86_64"
    patched="$replacement/tmp/R69/build-x86_64"
    base_headers="$baseline/libcxx/test-suite-install/include/c++/v1"
    compiler="$replacement/progress/R33/tools/tizen-clang++"
    overlay="$build/w1/removed_overlay"
    mkdir -p "$overlay"
    quiet_log 006r snapshot_w1_behavior_scripts_retry sha256sum \
      docs/progress/R109/code/w1_behavior_probe.cpp \
      docs/progress/R109/code/w1_inject_wait_error.c \
      docs/progress/R109/code/w1_run_matrix.py \
      docs/progress/R109/code/w1_make_removed_overlay.py \
      docs/progress/R109/code/stage_runner.sh
    quiet_log 007r make_w1_removed_overlay_retry python3 \
      docs/progress/R109/code/w1_make_removed_overlay.py \
      --header "$base_headers/__condition_variable/condition_variable.h" \
      --source codes/llvm/libcxx/src/condition_variable.cpp \
      --output "$overlay"
    quiet_log 008r build_w1_injector nice -n 15 ionice -c 3 gcc -shared -fPIC \
      docs/progress/R109/code/w1_inject_wait_error.c -o "$build/w1/libinject_wait_error.so"
    quiet_log 009r build_w1_removed_implementation nice -n 15 ionice -c 3 "$compiler" \
      -std=c++20 -O2 -fPIC -shared -nostdinc++ -I "$overlay/include" -I "$base_headers" \
      -nostdlib++ "$overlay/condition_variable_removed.cpp" -L "$baseline/lib" \
      -Wl,-rpath,"$baseline/lib" -lc++ -lc++abi -latomic -pthread \
      -Wl,-soname,libcv_removed.so -o "$build/w1/libcv_removed.so"
    common=(-std=c++20 -O2 -g -nostdinc++ -nostdlib++)
    quiet_log 010r build_w1_retained nice -n 15 ionice -c 3 "$compiler" "${common[@]}" \
      -I "$base_headers" docs/progress/R109/code/w1_behavior_probe.cpp \
      -L "$baseline/lib" -Wl,-rpath,"$baseline/lib" -lc++ -lc++abi -latomic -pthread \
      -o "$build/w1/probe_retained"
    quiet_log 011r build_w1_removed nice -n 15 ionice -c 3 "$compiler" "${common[@]}" \
      -I "$overlay/include" -I "$base_headers" docs/progress/R109/code/w1_behavior_probe.cpp \
      -L "$build/w1" -Wl,--no-as-needed -lcv_removed -L "$baseline/lib" \
      -Wl,-rpath,"$build/w1:$baseline/lib" -lc++ -lc++abi -latomic -pthread \
      -o "$build/w1/probe_removed"
    quiet_log 012r build_w1_libstdcxx nice -n 15 ionice -c 3 g++ -std=c++20 -O2 -g \
      docs/progress/R109/code/w1_behavior_probe.cpp -pthread -o "$build/w1/probe_libstdcxx"
    quiet_log 013r hash_w1_runtime_inputs sha256sum \
      "$baseline/lib/libc++.so.1.0" "$baseline/lib/libc++abi.so.1.0" \
      "$patched/lib/libc++.so.1.0" "$patched/lib/libc++abi.so.1.0" \
      "$build/w1/libcv_removed.so" "$build/w1/libinject_wait_error.so"
    quiet_log 014r identify_w1_retained env LD_LIBRARY_PATH="$baseline/lib" \
      LD_TRACE_LOADED_OBJECTS=1 "$build/w1/probe_retained"
    quiet_log 015r identify_w1_removed env LD_LIBRARY_PATH="$build/w1:$baseline/lib" \
      LD_TRACE_LOADED_OBJECTS=1 "$build/w1/probe_removed"
    quiet_log 016r identify_w1_oldcaller env \
      LD_PRELOAD="$build/w1/libcv_removed.so" LD_LIBRARY_PATH="$baseline/lib" \
      LD_TRACE_LOADED_OBJECTS=1 "$build/w1/probe_retained"
    quiet_log 017r identify_w1_libstdcxx env LD_TRACE_LOADED_OBJECTS=1 "$build/w1/probe_libstdcxx"
    quiet_log 018r run_w1_behavior_matrix nice -n 15 ionice -c 3 python3 \
      docs/progress/R109/code/w1_run_matrix.py \
      --cell "retained,$build/w1/probe_retained,$baseline/lib,NONE" \
      --cell "removed,$build/w1/probe_removed,$build/w1:$baseline/lib,NONE" \
      --cell "oldcaller_removedimpl,$build/w1/probe_retained,$baseline/lib,$build/w1/libcv_removed.so" \
      --cell "libstdcxx,$build/w1/probe_libstdcxx,SYSTEM,NONE" \
      --injector "$build/w1/libinject_wait_error.so" \
      --repeat 5 --output "$tables/w1_behavior_matrix.tsv" \
      --summary "$tables/w1_behavior_summary.tsv"
    printf 'phase=1 behavior=ready\n'
    ;;
  w1_binary)
    quiet_log 019 snapshot_w1_binary_audit sha256sum \
      docs/progress/R109/code/w1_binary_callers.py docs/progress/R109/code/stage_runner.sh
    quiet_log 020 scan_w1_binary_callers nice -n 15 ionice -c 3 python3 \
      docs/progress/R109/code/w1_binary_callers.py \
      --shards "$replacement/tmp/R11/elf_scan_shards" \
      --extracted "$replacement/tmp/R11/extracted" \
      --extra "$build/w1/probe_retained" \
      --output "$tables/w1_binary_callers.tsv" \
      --summary "$tables/w1_binary_callers_summary.tsv"
    printf 'phase=1 binary=ready\n'
    ;;
  *)
    printf 'phase=unknown result=error\n'
    exit 2
    ;;
esac
