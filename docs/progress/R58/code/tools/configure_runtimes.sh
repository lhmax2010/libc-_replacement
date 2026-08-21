#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
source_dir=${2:?source directory required}
build_dir=${3:?build directory required}

common=(
  -G Ninja -S "$source_dir/runtimes" -B "$build_dir"
  -DCMAKE_BUILD_TYPE=RelWithDebInfo
  '-DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi'
  -DLLVM_INCLUDE_TESTS=OFF
  -DLIBCXX_ENABLE_STATIC=OFF
  -DLIBCXX_INSTALL_STATIC_LIBRARY=OFF
  -DLIBCXX_INCLUDE_TESTS=OFF
  -DLIBCXX_INCLUDE_BENCHMARKS=OFF
  -DLIBCXX_ENABLE_SHARED=ON
  -DLIBCXX_INSTALL_SHARED_LIBRARY=ON
  -DLIBCXXABI_ENABLE_SHARED=ON
  -DLIBCXXABI_ENABLE_STATIC=OFF
  -DLIBCXXABI_INSTALL_SHARED_LIBRARY=ON
  -DLIBCXXABI_INSTALL_STATIC_LIBRARY=OFF
  -DLIBCXXABI_USE_COMPILER_RT=OFF
  -DLIBCXXABI_USE_LLVM_UNWINDER=OFF
  -DLIBCXXABI_ENABLE_STATIC_UNWINDER=OFF
  -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_SHARED_LIBRARY=OFF
  -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=OFF
  -DLIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS=ON
  -DLIBCXXABI_INCLUDE_TESTS=OFF
)

case "$arch" in
  x86_64)
    cc="$workspace/progress/R33/tools/tizen-clang"
    cxx="$workspace/progress/R33/tools/tizen-clang++"
    target=x86_64-tizen-linux-gnu
    arch_flags='--rtlib=libgcc'
    extra=()
    ;;
  armv7l)
    cc="$workspace/progress/R36/tools/armv7l-clang"
    cxx="$workspace/progress/R36/tools/armv7l-clang++"
    target=armv7l-tizen-linux-gnueabi
    arch_flags='--rtlib=libgcc -march=armv7-a -mfpu=neon -mfloat-abi=softfp -mthumb'
    extra=(-DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=armv7l -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY)
    ;;
  *) printf 'ERROR: unsupported architecture: %s\n' "$arch" >&2; exit 2 ;;
esac

cmd=(env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1
  nice -n 15 ionice -c 3 cmake "${common[@]}" "${extra[@]}"
  -DCMAKE_C_COMPILER="$cc" -DCMAKE_CXX_COMPILER="$cxx" -DCMAKE_ASM_COMPILER="$cc"
  -DCMAKE_C_COMPILER_TARGET="$target" -DCMAKE_CXX_COMPILER_TARGET="$target"
  -DCMAKE_ASM_COMPILER_TARGET="$target" -DLLVM_DEFAULT_TARGET_TRIPLE="$target"
  -DCMAKE_C_FLAGS="$arch_flags" -DCMAKE_CXX_FLAGS="$arch_flags"
  -DCMAKE_SHARED_LINKER_FLAGS=--rtlib=libgcc -DCMAKE_EXE_LINKER_FLAGS=--rtlib=libgcc)
printf 'CONFIGURE_COMMAND='; printf '%q ' "${cmd[@]}"; printf '\n'
"${cmd[@]}"
rc=$?
printf 'CONFIGURE_EXIT_CODE=%d\n' "$rc"
exit "$rc"

