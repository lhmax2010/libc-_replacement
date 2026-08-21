#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
out="$workspace/tmp/R58/c-only/$arch"
overlay="$workspace/tmp/R58/c-only-include/$arch"
mkdir -p "$out" "$overlay"
if [[ $arch == x86_64 ]]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
elif [[ $arch == armv7l ]]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
else
  exit 2
fi
cp -a "$workspace/tmp/R58/build-full-$arch/include/c++/v1/." "$overlay/"
cp "$workspace/tmp/R58/source-c-only/libcxxabi/include/cxxabi.h" "$overlay/cxxabi.h"
cp "$workspace/tmp/R58/source-c-only/libcxx/include/future" "$overlay/future"
cp "$workspace/tmp/R58/source-c-only/libcxx/include/istream" "$overlay/istream"
cp "$workspace/tmp/R58/source-c-only/libcxx/include/string" "$overlay/string"
cp "$workspace/tmp/R58/source-c-only/libcxx/include/__ostream/basic_ostream.h" \
  "$overlay/__ostream/basic_ostream.h"
cmd=(nice -n 15 ionice -c 3 "$cxx" -std=c++17 -O0 -g -fexceptions -pthread
  -stdlib=libc++ -nostdinc++ -I"$overlay" -c
  "$workspace/progress/R54/src/facility_probe.cpp" -o "$out/facility_c_only.o")
printf 'COMMAND='; printf '%q ' "${cmd[@]}"; printf '\n'
"${cmd[@]}"
rc=$?
printf 'EXIT_CODE=%d\n' "$rc"
exit 0
