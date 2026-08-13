#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
src=$workspace/tmp/R33/llvm-exp
build=$workspace/tmp/R33/build-s1-r3
test ! -e "$build"
export CMAKE_BUILD_PARALLEL_LEVEL=4
export LLVM_PARALLEL_LINK_JOBS=1
nice -n 15 ionice -c 3 cmake -G Ninja \
  -S "$src/runtimes" -B "$build" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_C_COMPILER="$workspace/progress/R33/tools/tizen-clang" \
  -DCMAKE_CXX_COMPILER="$workspace/progress/R33/tools/tizen-clang++" \
  -DCMAKE_ASM_COMPILER="$workspace/progress/R33/tools/tizen-clang" \
  -DCMAKE_C_COMPILER_TARGET=x86_64-tizen-linux-gnu \
  -DCMAKE_CXX_COMPILER_TARGET=x86_64-tizen-linux-gnu \
  -DCMAKE_ASM_COMPILER_TARGET=x86_64-tizen-linux-gnu \
  -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-tizen-linux-gnu \
  -DLLVM_ENABLE_RUNTIMES='libcxx;libcxxabi' \
  -DLLVM_INCLUDE_TESTS=OFF \
  -DLIBCXXABI_ENABLE_SHARED=ON \
  -DLIBCXXABI_ENABLE_STATIC=OFF \
  -DLIBCXXABI_INSTALL_SHARED_LIBRARY=ON \
  -DLIBCXXABI_INSTALL_STATIC_LIBRARY=OFF \
  -DLIBCXXABI_USE_COMPILER_RT=OFF \
  -DLIBCXXABI_USE_LLVM_UNWINDER=OFF \
  -DLIBCXXABI_ENABLE_STATIC_UNWINDER=OFF \
  -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_SHARED_LIBRARY=OFF \
  -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=OFF \
  -DLIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS=ON \
  -DLIBCXXABI_INCLUDE_TESTS=OFF \
  -DCMAKE_C_FLAGS=--rtlib=libgcc \
  -DCMAKE_CXX_FLAGS=--rtlib=libgcc \
  -DCMAKE_SHARED_LINKER_FLAGS=--rtlib=libgcc \
  -DCMAKE_EXE_LINKER_FLAGS=--rtlib=libgcc
