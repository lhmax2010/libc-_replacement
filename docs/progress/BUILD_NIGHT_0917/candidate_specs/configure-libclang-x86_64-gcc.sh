set -eu
export PATH=/tmp/night/tools:/bin:/usr/bin:/sbin:/usr/sbin
x86_64-tizen-linux-gnu-g++ --version
cmake -G Ninja -S /tmp/night/source/clang -B /tmp/night/builds/libclang-x86_64-gcc -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_C_COMPILER=x86_64-tizen-linux-gnu-gcc -DCMAKE_CXX_COMPILER=x86_64-tizen-linux-gnu-g++ -DLLVM_DIR=/usr/lib64/cmake/llvm -DLLVM_INCLUDE_TESTS=OFF -DCLANG_INCLUDE_TESTS=OFF -DLLVM_ENABLE_RTTI=ON -DLLVM_LINK_LLVM_DYLIB=ON -DCLANG_LINK_CLANG_DYLIB=ON -DCLANG_ENABLE_ARCMT=OFF -DCLANG_TABLEGEN=/usr/bin/clang-tblgen -DLLVM_TABLEGEN_EXE=/usr/bin/llvm-tblgen -DCMAKE_INSTALL_LIBDIR=lib64
ninja -C /tmp/night/builds/libclang-x86_64-gcc -t targets all | grep -E "libclang(_static)?(:|\.)"
ninja -C /tmp/night/builds/libclang-x86_64-gcc -n -j1 libclang
