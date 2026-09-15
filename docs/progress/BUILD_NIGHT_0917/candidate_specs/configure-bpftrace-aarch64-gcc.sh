set -eu
export PATH=/tmp/night/tools/aarch64/python/bin:/tmp/night/tools/aarch64:/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
unset CFLAGS CXXFLAGS LDFLAGS
aarch64-tizen-linux-gnu-g++ --version
cmake -G Ninja -S /tmp/night/bpftrace-source -B /tmp/night/builds/bpftrace-aarch64-gcc -DCMAKE_BUILD_TYPE=Release -DCMAKE_VERBOSE_MAKEFILE=ON -DBUILD_TESTING=OFF -DSTATIC_LINKING=OFF -DCMAKE_SKIP_RPATH:BOOL=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_C_COMPILER=aarch64-tizen-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-tizen-linux-gnu-g++
