set -eu
export PATH=/tmp/night/tools/aarch64/python/bin:/tmp/night/tools/aarch64:/bin:/usr/bin:/sbin:/usr/sbin
aarch64-tizen-linux-gnu-g++ --version
cmake -G Ninja -S /tmp/night/source/clang -B /tmp/night/builds/libclang-aarch64-gcc -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_C_COMPILER=aarch64-tizen-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-tizen-linux-gnu-g++ -DLLVM_DIR=/usr/lib64/cmake/llvm -DLLVM_INCLUDE_TESTS=OFF -DCLANG_INCLUDE_TESTS=OFF -DLLVM_ENABLE_RTTI=ON -DLLVM_LINK_LLVM_DYLIB=ON -DCLANG_LINK_CLANG_DYLIB=ON -DCLANG_ENABLE_ARCMT=OFF -DCLANG_TABLEGEN=/usr/bin/clang-tblgen -DLLVM_TABLEGEN_EXE=/usr/bin/llvm-tblgen -DCMAKE_INSTALL_LIBDIR=lib64 -DPython3_EXECUTABLE=/tmp/night/tools/aarch64/python/bin/python3
ninja -C /tmp/night/builds/libclang-aarch64-gcc -t targets all | grep -E "libclang(_static)?(:|\.)"
ninja -C /tmp/night/builds/libclang-aarch64-gcc -n -j1 libclang
