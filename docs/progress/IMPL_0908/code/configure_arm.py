#!/usr/bin/env python3
"""ARM 完整构建；QEMU 仅运行交叉编译工具，不用它执行探针。"""
import pathlib,subprocess,sys
src=pathlib.Path(sys.argv[1]).resolve();build=pathlib.Path(sys.argv[2]).resolve();log=sys.argv[3]
cc='/home/toolchain/development/libc++_replacement/progress/R36/tools/armv7l-clang'
flags='-march=armv7-a -mfpu=neon -mfloat-abi=softfp -mthumb'
args=['cmake','-G','Ninja','-S',str(src/'runtimes'),'-B',str(build),
 '-DCMAKE_BUILD_TYPE=RelWithDebInfo','-DCMAKE_SYSTEM_NAME=Linux','-DCMAKE_SYSTEM_PROCESSOR=armv7l',
 '-DCMAKE_C_COMPILER='+cc,'-DCMAKE_CXX_COMPILER='+cc+'++','-DCMAKE_ASM_COMPILER='+cc,
 '-DCMAKE_C_COMPILER_TARGET=armv7l-tizen-linux-gnueabi','-DCMAKE_CXX_COMPILER_TARGET=armv7l-tizen-linux-gnueabi',
 '-DCMAKE_ASM_COMPILER_TARGET=armv7l-tizen-linux-gnueabi','-DLLVM_DEFAULT_TARGET_TRIPLE=armv7l-tizen-linux-gnueabi',
 '-DCMAKE_AR=/usr/bin/llvm-ar','-DCMAKE_RANLIB=/usr/bin/llvm-ranlib','-DCMAKE_NM=/usr/bin/llvm-nm',
 '-DCMAKE_C_FLAGS='+flags,'-DCMAKE_CXX_FLAGS='+flags,'-DCMAKE_EXE_LINKER_FLAGS=--rtlib=libgcc',
 '-DCMAKE_SHARED_LINKER_FLAGS=--rtlib=libgcc -Wl,-z,now -Wl,--version-script='+str(src/'packaging/libcxx-llvm22.map'),
 '-DCMAKE_INSTALL_PREFIX='+str(build/'install'),'-DLLVM_ENABLE_RUNTIMES=libcxxabi;libcxx',
 '-DLIBCXX_ENABLE_SHARED=ON','-DLIBCXX_ENABLE_STATIC=OFF','-DLIBCXXABI_ENABLE_SHARED=ON','-DLIBCXXABI_ENABLE_STATIC=OFF',
 '-DLIBCXX_CXX_ABI=libcxxabi','-DLIBCXX_ENABLE_STATIC_ABI_LIBRARY=OFF','-DLIBCXX_ENABLE_ABI_LINKER_SCRIPT=ON',
 '-DLIBCXX_USE_COMPILER_RT=OFF','-DLIBCXXABI_USE_COMPILER_RT=OFF','-DLIBCXXABI_USE_LLVM_UNWINDER=OFF',
 '-DLIBCXXABI_ENABLE_STATIC_UNWINDER=OFF','-DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_SHARED_LIBRARY=OFF',
 '-DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=OFF','-DLIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS=ON',
 '-DLLVM_INCLUDE_TESTS=ON','-DLIBCXX_INCLUDE_TESTS=ON','-DLIBCXXABI_INCLUDE_TESTS=ON','-DLIBCXX_INCLUDE_BENCHMARKS=OFF',
 '-DLIBCXX_ENABLE_WERROR=ON','-DLIBCXXABI_ENABLE_WERROR=ON']
if src.name=='source':args+=['-DLIBCXX_ENABLE_TIZEN_CONDVAR_VERSIONS=ON']
raise SystemExit(subprocess.run([sys.executable,'docs/progress/R114/code/record.py',log,'nice','-n','15','ionice','-c','3',*args]).returncode)
