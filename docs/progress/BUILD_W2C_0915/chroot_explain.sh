set -eu
export PATH=/bin:/usr/bin:/sbin:/usr/sbin
cd /home/abuild/rpmbuild/BUILD/llvm-22.1.8/build
ninja -d explain -n -j1 lib/Analysis/CMakeFiles/LLVMAnalysis.dir/IVUsers.cpp.o
printf 'W2C_INNER_EXIT=0\n'
