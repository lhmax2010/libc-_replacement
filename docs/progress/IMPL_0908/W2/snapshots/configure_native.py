#!/usr/bin/env python3
"""同配置构建基线/实施运行库；只写调用方指定的 tmp 构建目录。"""
import pathlib, subprocess, sys
source=pathlib.Path(sys.argv[1]).resolve();build=pathlib.Path(sys.argv[2]).resolve();log=sys.argv[3]
compiler='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang'
args=['cmake','-G','Ninja','-S',str(source/'runtimes'),'-B',str(build),
 '-DCMAKE_BUILD_TYPE=RelWithDebInfo','-DCMAKE_C_COMPILER='+compiler,'-DCMAKE_CXX_COMPILER='+compiler+'++',
 '-DCMAKE_ASM_COMPILER='+compiler,'-DCMAKE_C_COMPILER_TARGET=x86_64-tizen-linux-gnu',
 '-DCMAKE_CXX_COMPILER_TARGET=x86_64-tizen-linux-gnu','-DCMAKE_ASM_COMPILER_TARGET=x86_64-tizen-linux-gnu',
 '-DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-tizen-linux-gnu','-DCMAKE_C_FLAGS=','-DCMAKE_CXX_FLAGS=',
 '-U*SUPPORTS_WERROR_FLAG',
 '-DCMAKE_EXE_LINKER_FLAGS=--rtlib=libgcc','-DCMAKE_SHARED_LINKER_FLAGS=--rtlib=libgcc -Wl,-z,now -Wl,--version-script='+str(source/'packaging/libcxx-llvm22.map'),
 '-DCMAKE_INSTALL_PREFIX='+str(build/'install'),'-DLLVM_ENABLE_RUNTIMES=libcxxabi;libcxx',
 '-DLIBCXX_ENABLE_SHARED=ON','-DLIBCXX_ENABLE_STATIC=OFF','-DLIBCXXABI_ENABLE_SHARED=ON','-DLIBCXXABI_ENABLE_STATIC=OFF',
 '-DLIBCXX_CXX_ABI=libcxxabi','-DLIBCXX_ENABLE_STATIC_ABI_LIBRARY=OFF','-DLIBCXX_ENABLE_ABI_LINKER_SCRIPT=ON',
 '-DLIBCXX_USE_COMPILER_RT=OFF','-DLIBCXXABI_USE_COMPILER_RT=OFF','-DLIBCXXABI_USE_LLVM_UNWINDER=OFF',
 '-DLIBCXXABI_ENABLE_STATIC_UNWINDER=OFF','-DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_SHARED_LIBRARY=OFF',
 '-DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=OFF','-DLIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS=ON',
 '-DLLVM_INCLUDE_TESTS=ON','-DLIBCXX_INCLUDE_TESTS=ON','-DLIBCXXABI_INCLUDE_TESTS=ON','-DLIBCXX_INCLUDE_BENCHMARKS=OFF',
 '-DLIBCXX_ENABLE_WERROR=ON','-DLIBCXXABI_ENABLE_WERROR=ON']
raise SystemExit(subprocess.run([sys.executable,'docs/progress/R114/code/record.py',log,'nice','-n','15','ionice','-c','3',*args]).returncode)
