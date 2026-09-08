#!/bin/bash
set -euo pipefail

if [ "$#" -ne 4 ]; then
    echo "usage: $0 ASSET_TAR ARCH_LABEL EXPECTED_MACHINE OUTPUT_DIR" >&2
    exit 2
fi

asset_tar=$1
arch_label=$2
expected_machine=$3
output_dir=$4
extract_dir="$output_dir/extracted"
mkdir -p "$extract_dir"

tar tzf "$asset_tar" >"$output_dir/archive_members.txt"
required=(
    InlinerSizeModel.h
    InlinerSizeModel.o
    RegAllocEvictModel.h
    RegAllocEvictModel.o
    xla_runtime_objects/xla_compiled_cpu_function.cc.o
    xla_runtime_objects/cpu_function_runtime.cc.o
    xla_runtime_objects/custom_call_status.cc.o
    xla_runtime_objects/executable_run_options.cc.o
    xla_runtime_objects/runtime_single_threaded_matmul_f32.cc.o
)

for member in "${required[@]}"; do
    grep -qx "./$member" "$output_dir/archive_members.txt"
    tar xzf "$asset_tar" -C "$extract_dir" "./$member"
done

: >"$output_dir/object_headers.txt"
objects=(
    InlinerSizeModel.o
    RegAllocEvictModel.o
    xla_runtime_objects/xla_compiled_cpu_function.cc.o
    xla_runtime_objects/cpu_function_runtime.cc.o
    xla_runtime_objects/custom_call_status.cc.o
    xla_runtime_objects/executable_run_options.cc.o
    xla_runtime_objects/runtime_single_threaded_matmul_f32.cc.o
)
for object in "${objects[@]}"; do
    printf '=== %s ===\n' "$object" >>"$output_dir/object_headers.txt"
    readelf -h "$extract_dir/$object" >>"$output_dir/object_headers.txt"
    readelf -h "$extract_dir/$object" | grep -q "Machine:.*$expected_machine"
done

llvm-nm -C -A "${objects[@]/#/$extract_dir/}" \
    >"$output_dir/demangled_symbols.txt"
std_1_count=$(grep -c 'std::__1' "$output_dir/demangled_symbols.txt" || true)
std_cxx11_count=$(grep -c 'std::__cxx11' "$output_dir/demangled_symbols.txt" || true)
printf 'arch=%s\nstd___1_lines=%s\nstd___cxx11_lines=%s\n' \
    "$arch_label" "$std_1_count" "$std_cxx11_count" \
    >"$output_dir/summary.txt"
test "$std_1_count" -gt 0
test "$std_cxx11_count" -eq 0
printf 'ASSET_ARCHIVE_PASS arch=%s\n' "$arch_label"
