#!/usr/bin/env bash
set -u -o pipefail
workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R33/tools/tizen-clang++"
headers="$workspace/tmp/R58/build-full-x86_64/include/c++/v1"
libs="$workspace/tmp/R58/build-full-x86_64/lib"
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
support="$workspace/codes/llvm/libcxx/test/support"
out="$workspace/tmp/R58/selected-libcxx-x86_64"
raw="$workspace/progress/R58/raw/x86_64/libcxx_selected"
mkdir -p "$out" "$raw"
tests=(
  'istream_getline|codes/llvm/libcxx/test/std/input.output/iostream.format/input.streams/istream.unformatted/getline_pointer_size.pass.cpp'
  'istream_read|codes/llvm/libcxx/test/std/input.output/iostream.format/input.streams/istream.unformatted/read.pass.cpp'
  'istream_ws|codes/llvm/libcxx/test/std/input.output/iostream.format/input.streams/istream.manip/ws.pass.cpp'
  'future_get_future|codes/llvm/libcxx/test/std/thread/futures/futures.task/futures.task.members/get_future.pass.cpp'
  'future_operator|codes/llvm/libcxx/test/std/thread/futures/futures.task/futures.task.members/operator.pass.cpp'
  'future_async|codes/llvm/libcxx/test/std/thread/futures/futures.async/async.pass.cpp'
  'string_shrink_to_fit|codes/llvm/libcxx/test/std/strings/basic.string/string.capacity/shrink_to_fit.pass.cpp'
)
overall=0
printf 'test\tcompile_exit\trun_exit\n'
for item in "${tests[@]}"; do
  name=${item%%|*}; source=${item#*|}; log="$raw/$name.log"
  cmd=("$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ -I "$headers" -I "$support" "$workspace/$source" -L "$libs" -L "$sysroot/usr/lib64" -Wl,-z,now -Wl,-rpath-link,"$libs" -lc++ -lc++abi -o "$out/$name")
  printf 'COMPILE_COMMAND=' > "$log"; printf '%q ' "${cmd[@]}" >> "$log"; printf '\n' >> "$log"
  nice -n 15 ionice -c 3 "${cmd[@]}" >> "$log" 2>&1; crc=$?
  printf 'COMPILE_EXIT_CODE=%d\n' "$crc" >> "$log"
  rrc=125
  if ((crc == 0)); then
    sha256sum "$out/$name" >> "$log"
    printf 'RUN_COMMAND=' >> "$log"; printf '%q ' "$loader" --library-path "$libs:$sysroot/usr/lib64:$sysroot/lib64" "$out/$name" >> "$log"; printf '\n' >> "$log"
    "$loader" --library-path "$libs:$sysroot/usr/lib64:$sysroot/lib64" "$out/$name" >> "$log" 2>&1; rrc=$?
  fi
  printf 'RUN_EXIT_CODE=%d\n' "$rrc" >> "$log"
  printf '%s\t%d\t%d\n' "$name" "$crc" "$rrc"
  ((crc == 0 && rrc == 0)) || overall=1
done
exit "$overall"
