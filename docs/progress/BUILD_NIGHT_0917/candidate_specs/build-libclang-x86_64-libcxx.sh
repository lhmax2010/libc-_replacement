set -eu
export PATH=/tmp/night/tools:/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
ninja -C /tmp/night/builds/libclang-x86_64-libcxx -j1 libclang_static
