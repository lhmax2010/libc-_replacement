#!/bin/bash
set -u

arch=$1
build_dir=$2
trace=$3
source_dir=/home/toolchain/development/libc++_replacement/tmp/R42/source-patched
common=(
  -G Ninja -S "$source_dir/runtimes" -B "$build_dir"
  -DCMAKE_BUILD_TYPE=RelWithDebInfo
  '-DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi'
  -DLLVM_INCLUDE_TESTS=OFF
  -DLIBCXX_ENABLE_SHARED=OFF
  -DLIBCXX_ENABLE_STATIC=OFF
  -DLIBCXX_INSTALL_SHARED_LIBRARY=OFF
  -DLIBCXX_INSTALL_STATIC_LIBRARY=OFF
  -DLIBCXX_INCLUDE_TESTS=OFF
  -DLIBCXX_INCLUDE_BENCHMARKS=OFF
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

trace_flag=
if [ "$trace" = on ]; then
  trace_flag=' -DLIBCXXABI_R42_TRACE=1'
elif [ "$trace" != off ]; then
  printf 'invalid trace mode: %s\n' "$trace" >&2
  exit 2
fi

if [ "$arch" = x86_64 ]; then
  cc=/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang
  cxx=/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++
  target=x86_64-tizen-linux-gnu
  arch_flags="--rtlib=libgcc${trace_flag}"
  extra=()
elif [ "$arch" = armv7l ]; then
  cc=/home/toolchain/development/libc++_replacement/progress/R36/tools/armv7l-clang
  cxx=/home/toolchain/development/libc++_replacement/progress/R36/tools/armv7l-clang++
  target=armv7l-tizen-linux-gnueabi
  arch_flags="--rtlib=libgcc -march=armv7-a -mfpu=neon -mfloat-abi=softfp -mthumb${trace_flag}"
  extra=(-DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=armv7l -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY)
else
  printf 'unsupported arch: %s\n' "$arch" >&2
  exit 2
fi

cmd=(env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1
  nice -n 15 ionice -c 3 cmake "${common[@]}" "${extra[@]}"
  -DCMAKE_C_COMPILER="$cc" -DCMAKE_CXX_COMPILER="$cxx" -DCMAKE_ASM_COMPILER="$cc"
  -DCMAKE_C_COMPILER_TARGET="$target" -DCMAKE_CXX_COMPILER_TARGET="$target"
  -DCMAKE_ASM_COMPILER_TARGET="$target" -DLLVM_DEFAULT_TARGET_TRIPLE="$target"
  -DCMAKE_C_FLAGS="$arch_flags" -DCMAKE_CXX_FLAGS="$arch_flags"
  -DCMAKE_SHARED_LINKER_FLAGS=--rtlib=libgcc -DCMAKE_EXE_LINKER_FLAGS=--rtlib=libgcc)
printf 'CONFIGURE_COMMAND='
printf '%q ' "${cmd[@]}"
printf '\n'
"${cmd[@]}"
rc=$?
printf 'CONFIGURE_EXIT_CODE=%d\n' "$rc"
exit "$rc"
