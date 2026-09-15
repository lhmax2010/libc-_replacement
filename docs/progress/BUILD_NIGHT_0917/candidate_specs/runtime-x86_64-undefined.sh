set -eu
export PATH=/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
export CFLAGS='-Os -fstack-protector -Wno-unused-command-line-argument -Wno-error=unused-but-set-variable -Wno-error=unused-command-line-argument -momit-leaf-frame-pointer  -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -Wformat -Wformat-security -fmessage-length=0 -frecord-gcc-switches -fdiagnostics-color=never -m64 -march=nehalem -msse4.2 -mfpmath=sse -fasynchronous-unwind-tables -fno-omit-frame-pointer -g'
export CXXFLAGS='-Os -fstack-protector -Wno-unused-command-line-argument -Wno-error=unused-but-set-variable -Wno-error=unused-command-line-argument -momit-leaf-frame-pointer  -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -Wformat -Wformat-security -fmessage-length=0 -frecord-gcc-switches -fdiagnostics-color=never -m64 -march=nehalem -msse4.2 -mfpmath=sse -fasynchronous-unwind-tables -fno-omit-frame-pointer -g'
mkdir -p /tmp/night/builds/runtime-x86_64-undefined
cd /tmp/night/builds/runtime-x86_64-undefined
x86_64-tizen-linux-gnu-clang++ --version
cmake -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=x86_64-tizen-linux-gnu-clang \
  -DCMAKE_CXX_COMPILER=x86_64-tizen-linux-gnu-clang++ \
  -DCMAKE_ASM_COMPILER=x86_64-tizen-linux-gnu-clang \
  -DCMAKE_C_COMPILER_TARGET=x86_64-tizen-linux-gnu \
  -DCMAKE_CXX_COMPILER_TARGET=x86_64-tizen-linux-gnu \
  -DCMAKE_ASM_COMPILER_TARGET=x86_64-tizen-linux-gnu \
  -DCMAKE_C_FLAGS="$CFLAGS --rtlib=libgcc" \
  -DCMAKE_CXX_FLAGS="$CXXFLAGS --rtlib=libgcc" \
  -DCMAKE_ASM_FLAGS="$CFLAGS" \
  -DCMAKE_SHARED_LINKER_FLAGS="-Wl,--version-script=/tmp/night/source/packaging/libcxx-llvm22.map -Wl,-z,now --rtlib=libgcc" \
  -DCMAKE_EXE_LINKER_FLAGS="--rtlib=libgcc" \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DCMAKE_INSTALL_LIBDIR=lib64 \
  -DLIBCXX_INSTALL_LIBRARY_DIR=lib64 \
  -DLIBCXXABI_INSTALL_LIBRARY_DIR=lib64 \
  -DLLVM_ENABLE_RUNTIMES="libcxxabi;libcxx" \
  -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-tizen-linux-gnu \
  -DLLVM_INCLUDE_TESTS=OFF \
  -DLIBCXX_ENABLE_SHARED=ON \
  -DLIBCXX_ENABLE_TIZEN_CONDVAR_VERSIONS=ON \
  -DLIBCXX_ENABLE_STATIC=OFF \
  -DLIBCXX_INSTALL_SHARED_LIBRARY=ON \
  -DLIBCXX_INSTALL_STATIC_LIBRARY=OFF \
  -DLIBCXX_CXX_ABI=libcxxabi \
  -DLIBCXX_ENABLE_STATIC_ABI_LIBRARY=OFF \
  -DLIBCXX_ENABLE_ABI_LINKER_SCRIPT=ON \
  -DLIBCXX_USE_COMPILER_RT=OFF \
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
  -DLIBCXX_INCLUDE_TESTS=OFF \
  -DLIBCXXABI_INCLUDE_TESTS=OFF \
  -DLIBCXX_INCLUDE_BENCHMARKS=OFF \
  /tmp/night/source/runtimes

cmake --build . --target cxxabi cxx cxx_experimental -- -j1
find lib -maxdepth 2 -type f -name "libc++*" -print
