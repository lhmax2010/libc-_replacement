#!/bin/bash
set -euo pipefail

if [ "$#" -ne 3 ]; then
    echo "usage: $0 BUILD_ROOT ARCH_LABEL OUTPUT_DIR" >&2
    exit 2
fi

build_root=$1
arch_label=$2
output_dir=$3
llvm_source="$build_root/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
llvm_build="$llvm_source/build"
llvm_shared=
for candidate in "$llvm_build/lib/libLLVM.so.22.1" "$llvm_build/lib64/libLLVM.so.22.1"; do
    if [ -f "$candidate" ]; then
        llvm_shared=$candidate
        break
    fi
done
mkdir -p "$output_dir"

test -n "$llvm_shared"
file "$llvm_shared" >"$output_dir/libllvm_file.txt"
readelf -d "$llvm_shared" >"$output_dir/libllvm_dynamic.txt"
grep -q 'Shared library: \[libc++.so.1\]' "$output_dir/libllvm_dynamic.txt"
if grep -q 'Shared library: \[libstdc++.so' "$output_dir/libllvm_dynamic.txt"; then
    echo "FATAL: libLLVM unexpectedly depends on libstdc++" >&2
    exit 10
fi

llvm-nm -C "$llvm_shared" >"$output_dir/libllvm_symbols.txt"
grep -q '_xla_InlinerSizeModel_llvm_InlinerSizeModel' \
    "$output_dir/libllvm_symbols.txt"
grep -q 'tensorflow::XlaCompiledCpuFunction::LookupArgIndex(std::__1::' \
    "$output_dir/libllvm_symbols.txt"
grep -q 'tensorflow::XlaCompiledCpuFunction::LookupResultIndex(std::__1::' \
    "$output_dir/libllvm_symbols.txt"
if grep -q 'std::__cxx11' "$output_dir/libllvm_symbols.txt"; then
    echo "FATAL: libLLVM contains std::__cxx11 symbols" >&2
    exit 11
fi

source_headers="$llvm_source/libcxx/include"
cxxabi_header="$llvm_source/libcxxabi/include/cxxabi.h"
: >"$output_dir/header_guards.txt"
grep -n -C 2 '__forced_unwind' "$source_headers/__ostream/basic_ostream.h" \
    >>"$output_dir/header_guards.txt"
grep -n -C 2 '__forced_unwind' "$source_headers/future" \
    >>"$output_dir/header_guards.txt"
grep -n -C 2 '__forced_unwind' "$source_headers/istream" \
    >>"$output_dir/header_guards.txt"
grep -n -C 2 '__forced_unwind' "$source_headers/string" \
    >>"$output_dir/header_guards.txt"
grep -n -E -C 2 'class .*__forced_unwind' "$cxxabi_header" \
    >>"$output_dir/header_guards.txt"

grep -E '^(CMAKE_CXX_COMPILER:|LLVM_ENABLE_LIBCXX:|LLVM_INLINER_MODEL_PATH:|LLVM_MLGO_EMBED_TF_XLA_RUNTIME_OBJECTS:|TENSORFLOW_AOT_PATH:)' \
    "$llvm_build/CMakeCache.txt" >"$output_dir/cmake_selection.txt"

printf 'LLVM_LIBCXX_BUILD_PASS arch=%s\n' "$arch_label"
