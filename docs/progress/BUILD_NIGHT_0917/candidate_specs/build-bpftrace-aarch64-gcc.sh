set -eu
export PATH=/tmp/night/tools/aarch64/python/bin:/tmp/night/tools/aarch64:/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
unset CFLAGS CXXFLAGS LDFLAGS
ninja -C /tmp/night/builds/bpftrace-aarch64-gcc -j1
