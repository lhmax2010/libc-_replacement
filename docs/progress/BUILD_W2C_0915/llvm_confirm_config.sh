set -e
export PATH=/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
#!/bin/sh


  RPM_SOURCE_DIR="/home/abuild/rpmbuild/SOURCES"
  RPM_BUILD_DIR="/home/abuild/rpmbuild/BUILD"
  RPM_OPT_FLAGS="-Os -fstack-protector -Wno-unused-command-line-argument -Wno-error=unused-but-set-variable -Wno-error=unused-command-line-argument  -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -Wformat -Wformat-security -fmessage-length=0 -frecord-gcc-switches -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -Wp,-D__SOFTFP__ -D_FILE_OFFSET_BITS=64 -g"
  RPM_ARCH="arm"
  RPM_OS="linux"
  export RPM_SOURCE_DIR RPM_BUILD_DIR RPM_OPT_FLAGS RPM_ARCH RPM_OS
  RPM_DOC_DIR="/usr/share/doc/packages"
  export RPM_DOC_DIR
  RPM_PACKAGE_NAME="llvm"
  RPM_PACKAGE_VERSION="22.1.8"
  RPM_PACKAGE_RELEASE="115.1"
  export RPM_PACKAGE_NAME RPM_PACKAGE_VERSION RPM_PACKAGE_RELEASE
  LANG=C
  export LANG
  unset CDPATH DISPLAY ||:
  RPM_BUILD_ROOT="/home/abuild/rpmbuild/BUILDROOT/llvm-22.1.8-115.1.arm"
  export RPM_BUILD_ROOT
  
  PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:/usr/lib/pkgconfig:/usr/share/pkgconfig"
  export PKG_CONFIG_PATH
  
  set -x
  umask 022
  cd "/home/abuild/rpmbuild/BUILD"

  CFLAGS="${CFLAGS:--Os -fstack-protector -Wno-unused-command-line-argument -Wno-error=unused-but-set-variable -Wno-error=unused-command-line-argument  -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -Wformat -Wformat-security -fmessage-length=0 -frecord-gcc-switches -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -Wp,-D__SOFTFP__ -D_FILE_OFFSET_BITS=64 -g}" ; export CFLAGS ; 
  CXXFLAGS="${CXXFLAGS:--Os -fstack-protector -Wno-unused-command-line-argument -Wno-error=unused-but-set-variable -Wno-error=unused-command-line-argument  -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -Wformat -Wformat-security -fmessage-length=0 -frecord-gcc-switches -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -Wp,-D__SOFTFP__ -D_FILE_OFFSET_BITS=64 -g}" ; export CXXFLAGS ; 
  LDFLAGS="${LDFLAGS:--Wl,-z,relro,--as-needed,-O1,--hash-style=gnu}" ; export LDFLAGS;

cd 'llvm-22.1.8'
ref=/usr/lib/rpm
mints=0
case $(uname -m) in
    aarch64) mints=20120610;;
    ppc64le) mints=20130610;;
    riscv64) mints=20160911;;
esac
for s in guess sub; do
    for c in $(find -maxdepth 8 -name "config.$s"); do
         grep -q config-patches@ $c || continue
         timestamp=$(sed -n "/^timestamp=/{s///;s/[-'\"]//g;p;q;}" $c | tr -d '')
         test -n "$timestamp" || timestamp=0
         test $timestamp -ge $mints || install -m 755 $ref/config.$s $c
     done
done

export AR=llvm-ar RANLIB=llvm-ranlib;
export CC=armv7l-tizen-linux-gnueabi-clang CXX=armv7l-tizen-linux-gnueabi-clang++;


cp /home/abuild/rpmbuild/SOURCES/llvm.manifest .


export QEMU_RESERVED_VA=0x100000000

# Set up MLGO AOT model paths from bundled verify assets
MLGO_AOT_DIR="$PWD/mlgo_verify_assets"
MLGO_BUILD_DIR="$PWD/build"
MLGO_RUNTIME_OBJECTS="${MLGO_AOT_DIR}/xla_runtime_objects/xla_compiled_cpu_function.cc.o;${MLGO_AOT_DIR}/xla_runtime_objects/cpu_function_runtime.cc.o;${MLGO_AOT_DIR}/xla_runtime_objects/custom_call_status.cc.o;${MLGO_AOT_DIR}/xla_runtime_objects/executable_run_options.cc.o;${MLGO_AOT_DIR}/xla_runtime_objects/runtime_single_threaded_matmul_f32.cc.o"
MLGO_REQUIRED_RUNTIME_OBJECTS="xla_compiled_cpu_function.cc.o cpu_function_runtime.cc.o custom_call_status.cc.o executable_run_options.cc.o runtime_single_threaded_matmul_f32.cc.o"

mkdir -p build
cd build
cmake \
    -G Ninja \
    -DTIZEN=1 \
    -DCMAKE_C_COMPILER=/bin/armv7l-tizen-linux-gnueabi-clang \
    -DCMAKE_CXX_COMPILER=/bin/armv7l-tizen-linux-gnueabi-clang++ \
    -DLLVM_HOST_TRIPLE=armv7l-tizen-linux-gnueabi \
    -DLLVM_DEFAULT_TARGET_TRIPLE=armv7l-tizen-linux-gnueabi \
    -DLLVM_TARGET_TRIPLE_ENV=armv7l-tizen-linux-gnueabi \
    -DCMAKE_ASM_FLAGS="$CFLAGS" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DLLVM_ENABLE_LIBCXX=ON \
    -DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,--no-as-needed -lc++abi -Wl,--as-needed" \
    -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,--no-as-needed -lc++abi -Wl,--as-needed" \
    -DLLVM_USE_LINKER=lld \
    -DLLVM_ENABLE_ASSERTIONS=No \
    -DLLVM_ENABLE_RTTI=ON \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DLLVM_TARGETS_TO_BUILD='ARM;BPF' \
    -DLLVM_TARGET_ARCH="ARM" \
    -DCLANG_ENABLE_ARCMT=OFF \
    -DLLVM_BUILD_LLVM_DYLIB=ON \
    -DCLANG_BUILD_CLANG_DYLIB=ON \
    -DLLVM_LINK_LLVM_DYLIB=ON \
    -DCLANG_LINK_CLANG_DYLIB=ON \
    -DLLVM_ENABLE_PROJECTS="clang;lldb;clang-tools-extra;lld;compiler-rt;openmp" \
    -DLLVM_ENABLE_PER_TARGET_RUNTIME_DIR=OFF \
    -DLLVM_BUILD_EXAMPLES=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_BUILD_TESTS=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_ENABLE_DOXYGEN=OFF \
    -DLLVM_BUILD_DOCS=OFF \
    -DLLVM_INCLUDE_DOCS=OFF \
    -DLLVM_OPTIMIZED_TABLEGEN=ON \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DLLVM_LIBDIR_SUFFIX=`echo lib | sed s/lib//g` \
    -DCLANG_RESOURCE_DIR="../lib/clang/22" \
    -DLLVM_BINUTILS_INCDIR=/usr/include \
    -DLLVM_PARALLEL_COMPILE_JOBS=6 \
    -DLLVM_PARALLEL_LINK_JOBS=2 \
    -DTENSORFLOW_AOT_PATH="${MLGO_AOT_DIR}/mlgo_sysroot" \
    -DLLVM_MLGO_EXPORT_TF_XLA_RUNTIME=OFF \
    -DLLVM_MLGO_EMBED_TF_XLA_RUNTIME_OBJECTS="${MLGO_RUNTIME_OBJECTS}" \
    -DLLVM_OVERRIDE_MODEL_HEADER_INLINERSIZEMODEL="${MLGO_AOT_DIR}/InlinerSizeModel.h" \
    -DLLVM_OVERRIDE_MODEL_OBJECT_INLINERSIZEMODEL="${MLGO_AOT_DIR}/InlinerSizeModel.o" \
    -DLLVM_OVERRIDE_MODEL_HEADER_REGALLOCEVICTMODEL="${MLGO_AOT_DIR}/RegAllocEvictModel.h" \
    -DLLVM_OVERRIDE_MODEL_OBJECT_REGALLOCEVICTMODEL="${MLGO_AOT_DIR}/RegAllocEvictModel.o" \
    ../llvm


# Full build using ninja (already in build/ directory after cd build)

printf 'FIXED_PATH=%s\n' "$PATH"
grep -E '^CMAKE_CXX_COMPILER:[A-Z]+=/bin/armv7l-tizen-linux-gnueabi-clang\+\+$' CMakeCache.txt
/bin/armv7l-tizen-linux-gnueabi-clang++ --version
grep -F 'set(CMAKE_CXX_COMPILER_ID "Clang")' CMakeFiles/*/CMakeCXXCompiler.cmake
grep -F 'set(CMAKE_CXX_COMPILER_VERSION "22.1.8")' CMakeFiles/*/CMakeCXXCompiler.cmake
grep -Fx 'LLVM_ENABLE_LIBCXX:BOOL=ON' CMakeCache.txt
grep -Fx 'CMAKE_BUILD_TYPE:STRING=MinSizeRel' CMakeCache.txt
ninja -t commands lib/Analysis/CMakeFiles/LLVMAnalysis.dir/IVUsers.cpp.o > /tmp/w2c-compiler-commands.txt
awk '/ -c .*IVUsers.cpp/ {print; found=1} END {if (!found) exit 1}' /tmp/w2c-compiler-commands.txt
awk '/ -c .*IVUsers.cpp/ && /-stdlib=libc\+\+/ {found=1} END {if (!found) exit 1}' /tmp/w2c-compiler-commands.txt
printf 'COMPILER_AND_LIBCXX_CONFIG_CHECK=PASS\n'
printf 'W2C_INNER_EXIT=0\n'
