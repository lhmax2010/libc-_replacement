set -eu
export PATH=/tmp/night/tools/aarch64/python/bin:/tmp/night/tools/aarch64:/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
ninja -C /tmp/night/builds/libclang-aarch64-gcc -j1 libclang
