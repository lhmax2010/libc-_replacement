set -eu
export PATH=/tmp/night/deps/x86_64/usr/bin:/tmp/night/tools:/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
ninja -C /tmp/night/builds/bpftrace-x86_64-static -j1 bpftrace
