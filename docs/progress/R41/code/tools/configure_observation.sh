#!/bin/bash
set -u

if [ "$#" -ne 2 ]; then
  printf 'usage: %s ARCH BUILD_DIR\n' "$0" >&2
  exit 2
fi
arch=$1
build_dir=$2
workspace=/home/toolchain/development/libc++_replacement
source_dir="$workspace/tmp/R41/source-observation"

case "$arch" in
  x86_64)
    c_compiler="$workspace/progress/R33/tools/tizen-clang"
    cxx_compiler="$workspace/progress/R33/tools/tizen-clang++"
    target=x86_64-tizen-linux-gnu
    system_args=()
    arch_flags='--rtlib=libgcc'
    ;;
  armv7l)
    c_compiler="$workspace/progress/R36/tools/armv7l-clang"
    cxx_compiler="$workspace/progress/R36/tools/armv7l-clang++"
    target=armv7l-tizen-linux-gnueabi
    system_args=(-DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=armv7l -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY)
    arch_flags='--rtlib=libgcc -march=armv7-a -mfpu=neon -mfloat-abi=softfp -mthumb'
    ;;
  *) printf 'unsupported arch: %s\n' "$arch" >&2; exit 3 ;;
esac

test ! -e "$build_dir" || { printf 'BUILD_DIR_ALREADY_EXISTS=%s\n' "$build_dir"; exit 4; }
args=(-G Ninja -S "$source_dir/runtimes" -B "$build_dir" "${system_args[@]}"
  -DCMAKE_BUILD_TYPE=RelWithDebInfo
  -DCMAKE_C_COMPILER="$c_compiler" -DCMAKE_CXX_COMPILER="$cxx_compiler" -DCMAKE_ASM_COMPILER="$c_compiler"
  -DCMAKE_C_COMPILER_TARGET="$target" -DCMAKE_CXX_COMPILER_TARGET="$target" -DCMAKE_ASM_COMPILER_TARGET="$target"
  -DLLVM_DEFAULT_TARGET_TRIPLE="$target" '-DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi' -DLLVM_INCLUDE_TESTS=OFF
  -DLIBCXX_ENABLE_SHARED=OFF -DLIBCXX_ENABLE_STATIC=OFF -DLIBCXX_INSTALL_SHARED_LIBRARY=OFF -DLIBCXX_INSTALL_STATIC_LIBRARY=OFF
  -DLIBCXX_INCLUDE_TESTS=OFF -DLIBCXX_INCLUDE_BENCHMARKS=OFF
  -DLIBCXXABI_ENABLE_SHARED=ON -DLIBCXXABI_ENABLE_STATIC=OFF -DLIBCXXABI_INSTALL_SHARED_LIBRARY=ON
  -DLIBCXXABI_INSTALL_STATIC_LIBRARY=OFF -DLIBCXXABI_USE_COMPILER_RT=OFF -DLIBCXXABI_USE_LLVM_UNWINDER=OFF
  -DLIBCXXABI_ENABLE_STATIC_UNWINDER=OFF -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_SHARED_LIBRARY=OFF
  -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=OFF -DLIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS=ON
  -DLIBCXXABI_INCLUDE_TESTS=OFF -DCMAKE_C_FLAGS="$arch_flags" -DCMAKE_CXX_FLAGS="$arch_flags"
  -DCMAKE_SHARED_LINKER_FLAGS=--rtlib=libgcc -DCMAKE_EXE_LINKER_FLAGS=--rtlib=libgcc)
printf 'ARCH=%s\nSOURCE_DIR=%s\nBUILD_DIR=%s\nCMAKE_BUILD_PARALLEL_LEVEL=2\nLLVM_PARALLEL_LINK_JOBS=1\n' "$arch" "$source_dir" "$build_dir"
printf 'CONFIGURE_COMMAND='; printf ' %q' env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1 nice -n 15 ionice -c 3 cmake "${args[@]}"; printf '\n'
env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1 nice -n 15 ionice -c 3 cmake "${args[@]}"

