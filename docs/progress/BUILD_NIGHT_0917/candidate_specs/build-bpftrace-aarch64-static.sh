set -eu
export PATH=/tmp/night/deps/aarch64/usr/bin:/tmp/night/tools/aarch64/python/bin:/tmp/night/tools/aarch64:/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
ninja -C /tmp/night/builds/bpftrace-aarch64-static -j1 bpftrace
