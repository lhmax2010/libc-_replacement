#!/usr/bin/env bash
# R47:构建取消实验的消费者二进制(x86_64)。
# 变体:unpatched_O0 / unpatched_O2(基线头文件)、patched_O0(补丁 overlay 头文件)、libstdcxx(对照)。
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R33/tools/tizen-clang++"
src="$workspace/tmp/R47/probe/r47_cancel_probe.cpp"
out="$workspace/tmp/R47/tests/x86_64"
buildroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
gbs_inc="$buildroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/include/c++/v1"
gbs_lib="$buildroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
overlay="$workspace/tmp/R47/overlay/include"
r45_abi_inc="$workspace/tmp/R45/source-patched/libcxxabi/include"
mkdir -p "$out"

run() {
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
  nice -n 15 ionice -c 3 "$@"
  local rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

# 共同选项说明(逐项):
#   -std=c++17          与 R46 消费者一致
#   -O0 / -O2           两档优化(生产口径为 -O2)
#   -g -fexceptions -pthread
#   -stdlib=libc++ -nostdinc++ -I…   使用申报的头文件树,无其他隐式 C++ 头
#   -L"$gbs_lib" -lc++ -lc++abi      链接期解析用 GBS 构建库(运行期由 loader --library-path 决定实际加载)
#   -Wl,-z,now          立即绑定(与平台一致)
#   未使用:-fvisibility=*、--export-dynamic、-rpath(运行期路径由 loader 显式给出)
run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
  -I"$gbs_inc" "$src" -L"$gbs_lib" -Wl,-z,now -lc++ -lc++abi \
  -o "$out/consumer_unpatched_O0"

run "$cxx" -std=c++17 -O2 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
  -I"$gbs_inc" "$src" -L"$gbs_lib" -Wl,-z,now -lc++ -lc++abi \
  -o "$out/consumer_unpatched_O2"

# 补丁头文件消费者:overlay(补丁 istream)在前,R45 libcxxabi 头次之(cxxabi.h 的 __forced_unwind),基线头最后。
run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
  -I"$overlay" -I"$r45_abi_inc" -I"$gbs_inc" "$src" -L"$gbs_lib" -Wl,-z,now -lc++ -lc++abi \
  -o "$out/consumer_patched_O0"

# libstdc++ 对照:不加 -stdlib/-nostdinc++,包装器默认走 GCC/libstdc++(与 R43b x86_64 同法)。
run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread "$src" -Wl,-z,now \
  -o "$out/consumer_libstdcxx"

sha256sum "$src" "$out"/consumer_*
