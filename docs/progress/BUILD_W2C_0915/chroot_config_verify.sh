set -eu
export PATH=/bin:/usr/bin:/sbin:/usr/sbin
cd /home/abuild/rpmbuild/BUILD/llvm-22.1.8/build
grep -E '^CMAKE_C_COMPILER:[A-Z]+=/bin/armv7l-tizen-linux-gnueabi-clang$' CMakeCache.txt
grep -E '^CMAKE_CXX_COMPILER:[A-Z]+=/bin/armv7l-tizen-linux-gnueabi-clang\+\+$' CMakeCache.txt
grep -Fx 'CMAKE_BUILD_TYPE:STRING=MinSizeRel' CMakeCache.txt
grep -Fx 'LLVM_ENABLE_LIBCXX:BOOL=ON' CMakeCache.txt
grep -Fx 'LLVM_TARGETS_TO_BUILD:STRING=ARM;BPF' CMakeCache.txt
grep -Fx 'LLVM_ENABLE_PROJECTS:STRING=clang;lldb;clang-tools-extra;lld;compiler-rt;openmp' CMakeCache.txt
sha256sum CMakeCache.txt build.ninja .ninja_log
ninja -n -j1 > /tmp/w2c-restored-dryrun.txt
tail -n 3 /tmp/w2c-restored-dryrun.txt
printf 'W2C_INNER_EXIT=0\n'
