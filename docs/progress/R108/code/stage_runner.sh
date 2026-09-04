#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
cd "$root"
log=docs/progress/R108/code/run_logged.sh
tables=docs/progress/R108/tables
build=docs/progress/R108/build
replacement=/home/toolchain/development/libc++_replacement

quiet_log() {
  "$log" "$@" >/dev/null 2>&1
}

phase=${1:-}
case "$phase" in
  w1)
    compgen -G 'docs/progress/R108/raw/012_*.exitcode' >/dev/null
    compgen -G 'docs/progress/R108/raw/015_*.exitcode' >/dev/null
    quiet_log 016 classify_w1_source python3 docs/progress/R108/code/w1_classify_source.py \
      --input "$tables/w1_filtered_source_hits.raw" \
      --output "$tables/w1_filtered_source_classification.tsv"
    quiet_log 017 hash_w1_prior_evidence sha256sum \
      docs/progress/R87/REPORT.md \
      docs/progress/R87/tables/static_archive_candidates.tsv \
      docs/progress/R90/REPORT.md \
      docs/progress/R92/REPORT.md \
      "$replacement/progress/R11/tables/source_download_status.tsv" \
      "$replacement/progress/R11/tables/binary_download_status.tsv" \
      "$replacement/progress/R11/tables/elf_inventory.tsv"
    printf 'phase=1 result=ready\n'
    ;;
  w2)
    mkdir -p "$build/w2"
    quiet_log 040 snapshot_w2_final_scripts sha256sum \
      docs/progress/R108/code/run_logged.sh \
      docs/progress/R108/code/w2_inventory.py \
      docs/progress/R108/code/w2_rollup.py \
      docs/progress/R108/code/stage_runner.sh
    if [ "${2:-}" != "read_only" ]; then
      quiet_log 041 resource_gate_w2_final tools/resource_gate.sh --level medium
    fi
    quiet_log 042 inventory_w2_final nice -n 15 ionice -c 3 python3 \
      docs/progress/R108/code/w2_inventory.py \
      --static-paths docs/progress/R87/tables/static_archive_paths.txt \
      --base-workspace /home/toolchain/development \
      --shards "$replacement/tmp/R11/elf_scan_shards" \
      --source-root "$replacement/tmp/corpus/extracted" \
      --output "$tables/w2_inventory.tsv" \
      --summary "$tables/w2_inventory_summary.tsv"
    quiet_log 043 rollup_w2_final python3 docs/progress/R108/code/w2_rollup.py \
      --input "$tables/w2_inventory.tsv" \
      --output "$tables/w2_package_rollup.tsv"
    quiet_log 044 hash_w2_prior_evidence sha256sum \
      docs/progress/R66/REPORT.md \
      docs/progress/R66/tables/vendor_rebuildability.tsv \
      docs/progress/R87/tables/static_archive_paths.txt \
      "$replacement/progress/R11/tables/elf_inventory.tsv"
    printf 'phase=2 result=ready\n'
    ;;
  w3)
    mkdir -p "$build/w3"
    quiet_log 047 snapshot_w3_scripts sha256sum \
      docs/progress/R108/code/w3_atomic_wait_probe.cpp \
      docs/progress/R108/code/w3_capture_evidence.py \
      docs/progress/R108/code/w3_repeat.sh \
      docs/progress/R108/code/stage_runner.sh
    quiet_log 048 resource_gate_w3 tools/resource_gate.sh --level medium
    runtime="$replacement/tmp/R69/build-x86_64/lib"
    headers="$replacement/tmp/R68/build-patched-x86_64/libcxx/test-suite-install/include/c++/v1"
    compiler="$replacement/progress/R33/tools/tizen-clang++"
    patched="$build/w3/atomic_wait_patched"
    baseline="$build/w3/atomic_wait_baseline"
    quiet_log 049 build_w3_patched nice -n 15 ionice -c 3 "$compiler" \
      -std=c++20 -O0 -g -fno-inline -nostdinc++ -I "$headers" -nostdlib++ \
      docs/progress/R108/code/w3_atomic_wait_probe.cpp -L "$runtime" \
      -Wl,-rpath,"$runtime" -lc++ -lc++abi -latomic -pthread -o "$patched"
    quiet_log 050 identify_w3_patched env LD_LIBRARY_PATH="$runtime" \
      LD_TRACE_LOADED_OBJECTS=1 "$patched"
    quiet_log 051 symbol_w3_patched bash -c \
      'nm -C -- "$1" | rg "atomic<int>::wait|__cxx_atomic_wait"' _ "$patched"
    quiet_log 052 run_w3_patched env LD_LIBRARY_PATH="$runtime" \
      bash docs/progress/R108/code/w3_repeat.sh "$patched"
    quiet_log 053 build_w3_baseline nice -n 15 ionice -c 3 g++ \
      -std=c++20 -O0 -g -fno-inline docs/progress/R108/code/w3_atomic_wait_probe.cpp \
      -pthread -o "$baseline"
    quiet_log 054 identify_w3_baseline env LD_TRACE_LOADED_OBJECTS=1 "$baseline"
    quiet_log 055 symbol_w3_baseline bash -c \
      'nm -C -- "$1" | rg "atomic<int>::wait|__atomic_wait"' _ "$baseline"
    quiet_log 056 run_w3_baseline bash docs/progress/R108/code/w3_repeat.sh "$baseline"
    lwe="$replacement/tmp/corpus/extracted/lightweight-web-engine-1.4.0-1.src.rpm/unpacked/lightweight-web-engine-1.4.0.tar.gz/lightweight-web-engine-1.4.0/src/core/modules/worker"
    omp="$replacement/tmp/corpus/extracted/llvm-22.1.8-18.1.src.rpm/unpacked/llvm-22.1.8.tar.gz/llvm-22.1.8/openmp/runtime/src"
    quiet_log 057 capture_w3_source_evidence python3 \
      docs/progress/R108/code/w3_capture_evidence.py \
      --output "$tables/w3_source_evidence.tsv" \
      "$lwe/WorkerThread.cpp:116-138,178-194" \
      "$lwe/WorkerHost.cpp:41-61" \
      "$omp/z_Linux_util.cpp:442-458,527-600,610-650,719-732" \
      "$omp/kmp_runtime.cpp:5969-6050" \
      "$headers/atomic:95-110" \
      "$headers/__atomic/atomic.h:110-128"
    quiet_log 058 hash_w3_runtimes sha256sum \
      "$runtime/libc++.so.1.0" "$runtime/libc++abi.so.1.0" \
      "$headers/atomic"
    printf 'phase=3 result=ready\n'
    ;;
  w2_link)
    llvm_root="$replacement/tmp/corpus/extracted/llvm-22.1.8-18.1.src.rpm"
    quiet_log 045r snapshot_w2_linkage_helper_retry sha256sum \
      docs/progress/R108/code/w2_linked_object_evidence.py
    quiet_log 046r capture_w2_linkage_retry python3 \
      docs/progress/R108/code/w2_linked_object_evidence.py \
      --root "$llvm_root" --output "$tables/w2_linked_object_evidence.tsv"
    printf 'phase=2 linkage=ready\n'
    ;;
  w2_static)
    quiet_log 046s snapshot_w2_static_retry sha256sum \
      docs/progress/R108/code/w2_inventory.py docs/progress/R108/code/stage_runner.sh
    quiet_log 046t inventory_w2_static_retry nice -n 15 ionice -c 3 python3 \
      docs/progress/R108/code/w2_inventory.py \
      --static-paths docs/progress/R87/tables/static_archive_paths.txt \
      --base-workspace "$replacement" \
      --shards "$replacement/tmp/R11/elf_scan_shards" \
      --source-root "$replacement/tmp/corpus/extracted" \
      --output "$tables/w2_static_inventory.tsv" \
      --summary "$tables/w2_static_summary.tsv" --skip-source
    printf 'phase=2 static=ready\n'
    ;;
  w3_resume)
    mkdir -p "$build/w3"
    runtime="$replacement/tmp/R69/build-x86_64/lib"
    headers="$replacement/tmp/R68/build-patched-x86_64/libcxx/test-suite-install/include/c++/v1"
    patched="$build/w3/atomic_wait_patched"
    baseline="$build/w3/atomic_wait_baseline"
    quiet_log 051r snapshot_w3_symbol_helper sha256sum \
      docs/progress/R108/code/w3_symbol_check.sh docs/progress/R108/code/stage_runner.sh
    quiet_log 051s symbol_w3_patched_retry bash docs/progress/R108/code/w3_symbol_check.sh \
      "$patched" "$tables/w3_patched_symbols.txt"
    quiet_log 052 run_w3_patched env LD_LIBRARY_PATH="$runtime" \
      bash docs/progress/R108/code/w3_repeat.sh "$patched"
    quiet_log 053 build_w3_baseline nice -n 15 ionice -c 3 g++ \
      -std=c++20 -O0 -g -fno-inline docs/progress/R108/code/w3_atomic_wait_probe.cpp \
      -pthread -o "$baseline"
    quiet_log 054 identify_w3_baseline env LD_TRACE_LOADED_OBJECTS=1 "$baseline"
    quiet_log 055 symbol_w3_baseline bash docs/progress/R108/code/w3_symbol_check.sh \
      "$baseline" "$tables/w3_baseline_symbols.txt"
    quiet_log 056 run_w3_baseline bash docs/progress/R108/code/w3_repeat.sh "$baseline"
    lwe="$replacement/tmp/corpus/extracted/lightweight-web-engine-1.4.0-1.src.rpm/unpacked/lightweight-web-engine-1.4.0.tar.gz/lightweight-web-engine-1.4.0/src/core/modules/worker"
    omp="$replacement/tmp/corpus/extracted/llvm-22.1.8-18.1.src.rpm/unpacked/llvm-22.1.8.tar.gz/llvm-22.1.8/openmp/runtime/src"
    quiet_log 057 capture_w3_source_evidence python3 \
      docs/progress/R108/code/w3_capture_evidence.py \
      --output "$tables/w3_source_evidence.tsv" \
      "$lwe/WorkerThread.cpp:116-138,178-194" \
      "$lwe/WorkerHost.cpp:41-61" \
      "$omp/z_Linux_util.cpp:442-458,527-600,610-650,719-732" \
      "$omp/kmp_runtime.cpp:5969-6050" \
      "$headers/atomic:95-110" \
      "$headers/__atomic/atomic.h:110-128"
    quiet_log 058 hash_w3_runtimes sha256sum \
      "$runtime/libc++.so.1.0" "$runtime/libc++abi.so.1.0" "$headers/atomic"
    printf 'phase=3 result=ready\n'
    ;;
  final)
    quiet_log 059r snapshot_all_scripts_retry sha256sum docs/progress/R108/code/*.*
    quiet_log 060r validate_artifacts_retry python3 docs/progress/R108/code/validate_artifacts.py
    printf 'phase=final result=ready\n'
    ;;
  *)
    printf 'phase=unknown result=error\n'
    exit 2
    ;;
esac
