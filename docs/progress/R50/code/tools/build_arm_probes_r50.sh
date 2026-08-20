#!/usr/bin/env bash
# R50:按 R47 同一探针与判定口径构建 armv7l 消费者目标文件。
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R36/tools/armv7l-clang++"
src17="$workspace/progress/R50/code/tests/site_symbol_probe.cpp"
src20="$workspace/progress/R50/code/tests/site_symbol_probe_cxx20.cpp"
out="$workspace/tmp/R50/probes/armv7l"
buildroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
headers="$buildroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/include/c++/v1"
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

# 平台 armv7l 生产构建选项来源:
# C2 buildroot 的 CMakeCache.txt 与 build.ninja。架构采用 armv7-a/
# cortex-a8/NEON/softfp/Thumb；libc++ TU 使用 hidden 默认可见性与
# hidden inline。R47 的 -fPIC/-fexceptions/-pthread/nostdinc++ 口径保留。
common=(
  -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2
  -fexceptions -Wformat -Wformat-security -fmessage-length=0
  -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon
  -mfloat-abi=softfp -mthumb -Wp,-D__SOFTFP__ -D_FILE_OFFSET_BITS=64
  --rtlib=libgcc -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
  -pthread -stdlib=libc++ -nostdinc++ -I"$headers"
)

run "$cxx" -std=c++17 -O0 "${common[@]}" -c "$src17" -o "$out/probe_cxx17_O0.o"
run "$cxx" -std=c++17 -O2 "${common[@]}" -c "$src17" -o "$out/probe_cxx17_O2.o"
run "$cxx" -std=c++20 -O0 "${common[@]}" -c "$src20" -o "$out/probe_cxx20_O0.o"
run "$cxx" -std=c++20 -O2 "${common[@]}" -c "$src20" -o "$out/probe_cxx20_O2.o"

sha256sum "$src17" "$src20" "$out"/*.o
