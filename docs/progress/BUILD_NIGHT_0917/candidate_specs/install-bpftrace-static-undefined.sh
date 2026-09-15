set -eu
export PATH=/tmp/night/tools/aarch64/python/bin:/tmp/night/tools/aarch64:/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
unset CFLAGS CXXFLAGS LDFLAGS
mkdir -p /tmp/night/fallback-undefined/usr/bin
install -m 0755 /tmp/night/original_assets/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace /tmp/night/fallback-undefined/usr/bin/bpftrace-static
sha256sum /tmp/night/original_assets/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace /tmp/night/fallback-undefined/usr/bin/bpftrace-static
