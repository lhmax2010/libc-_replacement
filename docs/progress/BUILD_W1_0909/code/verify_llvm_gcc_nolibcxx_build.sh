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
grep -q 'Shared library: \[libstdc++.so' "$output_dir/libllvm_dynamic.txt"
if grep -q 'Shared library: \[libc++.so' "$output_dir/libllvm_dynamic.txt"; then
    echo "FATAL: GCC/non-libc++ libLLVM unexpectedly depends on libc++" >&2
    exit 10
fi
if grep -q 'Shared library: \[libc++abi.so' "$output_dir/libllvm_dynamic.txt"; then
    echo "FATAL: GCC/non-libc++ libLLVM unexpectedly depends on libc++abi" >&2
    exit 11
fi

grep -E '^(CMAKE_C_COMPILER:|CMAKE_CXX_COMPILER:|LLVM_ENABLE_LIBCXX:|LLVM_INLINER_MODEL_PATH:|LLVM_MLGO_EMBED_TF_XLA_RUNTIME_OBJECTS:|TENSORFLOW_AOT_PATH:)' \
    "$llvm_build/CMakeCache.txt" >"$output_dir/cmake_selection.txt"
grep -Eq '^CMAKE_CXX_COMPILER:.*(g\+\+|gcc)' "$output_dir/cmake_selection.txt"
if grep -q '^LLVM_ENABLE_LIBCXX:BOOL=ON$' "$output_dir/cmake_selection.txt"; then
    echo "FATAL: LLVM_ENABLE_LIBCXX is ON in GCC/non-libc++ path" >&2
    exit 12
fi

if grep -F -- '-stdlib=libc++' "$llvm_build/CMakeCache.txt" "$llvm_build/build.ninja" \
    >"$output_dir/forbidden_libcxx_flags.txt"; then
    echo "FATAL: -stdlib=libc++ reached GCC/non-libc++ build files" >&2
    exit 13
fi
if grep -F -- '-lc++abi' "$llvm_build/CMakeCache.txt" "$llvm_build/build.ninja" \
    >>"$output_dir/forbidden_libcxx_flags.txt"; then
    echo "FATAL: -lc++abi reached GCC/non-libc++ build files" >&2
    exit 14
fi

llvm-nm -C "$llvm_shared" >"$output_dir/libllvm_symbols.txt"
grep -q '_xla_InlinerSizeModel_llvm_InlinerSizeModel' "$output_dir/libllvm_symbols.txt"
grep -q 'tensorflow::XlaCompiledCpuFunction::LookupArgIndex(std::__cxx11::' \
    "$output_dir/libllvm_symbols.txt"
grep -q 'tensorflow::XlaCompiledCpuFunction::LookupResultIndex(std::__cxx11::' \
    "$output_dir/libllvm_symbols.txt"
if grep -q 'tensorflow::XlaCompiledCpuFunction::LookupArgIndex(std::__1::' \
    "$output_dir/libllvm_symbols.txt"; then
    echo "FATAL: libc++ TensorFlow 2.18 runtime object reached old-asset path" >&2
    exit 15
fi

printf 'LLVM_GCC_NOLIBCXX_BUILD_PASS arch=%s\n' "$arch_label"
