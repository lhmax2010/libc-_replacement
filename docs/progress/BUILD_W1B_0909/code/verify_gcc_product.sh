#!/bin/bash
set -euo pipefail

if [ "$#" -ne 5 ]; then
    echo "usage: $0 BUILD_ROOT RPM_DIR SRPM ARCH OUTPUT_DIR" >&2
    exit 2
fi

build_root=$1
rpm_dir=$2
srpm=$3
arch=$4
output_dir=$5
mkdir -p "$output_dir"

llvm_source="$build_root/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
llvm_build="$llvm_source/build"
llvm_shared=
for candidate in "$llvm_build/lib/libLLVM.so.22.1" "$llvm_build/lib64/libLLVM.so.22.1"; do
    if [ -f "$candidate" ]; then
        llvm_shared=$candidate
        break
    fi
done
test -n "$llvm_shared"

grep -E '^(CMAKE_C_COMPILER:|CMAKE_CXX_COMPILER:|LLVM_ENABLE_LIBCXX:)' \
    "$llvm_build/CMakeCache.txt" >"$output_dir/compiler_selection.txt"
grep -Eq '^CMAKE_C_COMPILER:.*gcc' "$output_dir/compiler_selection.txt"
grep -Eq '^CMAKE_CXX_COMPILER:.*g\+\+' "$output_dir/compiler_selection.txt"
if grep -q '^LLVM_ENABLE_LIBCXX:BOOL=ON$' "$output_dir/compiler_selection.txt"; then
    echo "FATAL: LLVM_ENABLE_LIBCXX is ON" >&2
    exit 10
fi

: >"$output_dir/forbidden_build_content.txt"
for pattern in '-stdlib=libc++' '-lc++abi'; do
    if grep -F -- "$pattern" "$llvm_build/CMakeCache.txt" "$llvm_build/build.ninja" \
        >>"$output_dir/forbidden_build_content.txt"; then
        echo "FATAL: forbidden option reached generated build files: $pattern" >&2
        exit 11
    fi
done

rpm -qp --requires "$srpm" | LC_ALL=C sort >"$output_dir/srpm_build_requires.txt"
if grep -E '(^|[^[:alnum:]_])libc\+\+(-devel|abi-devel)?([^[:alnum:]_]|$)' \
    "$output_dir/srpm_build_requires.txt" >"$output_dir/forbidden_build_requires.txt"; then
    echo "FATAL: libc++ BuildRequires reached source RPM" >&2
    exit 12
fi

readelf -d "$llvm_shared" >"$output_dir/libllvm_dynamic.txt"
grep -q 'Shared library: \[libstdc++.so' "$output_dir/libllvm_dynamic.txt"
if grep -Eq 'Shared library: \[libc\+\+|Shared library: \[libc\+\+abi' \
    "$output_dir/libllvm_dynamic.txt"; then
    echo "FATAL: GCC product depends on libc++" >&2
    exit 13
fi

find "$rpm_dir" -maxdepth 1 -type f -name "*.$arch.rpm" -printf '%f\n' \
    | LC_ALL=C sort >"$output_dir/rpm_files.txt"
test -s "$output_dir/rpm_files.txt"
printf 'FORBIDDEN_OPTION_MATCHES=0\n'
printf 'FORBIDDEN_BUILDREQUIRES_MATCHES=0\n'
printf 'GCC_PRODUCT_VERIFY=PASS arch=%s\n' "$arch"
