#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R33/tools/tizen-clang++"
headers="$workspace/tmp/R51/build-product-x86_64/include/c++/v1"
overlay="$workspace/tmp/R52/overlay/include"
abi="$workspace/tmp/R51/runtime/x86_64"
libcxx="$workspace/tmp/R52/rebuilt-libcxx/x86_64"
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
support="$workspace/codes/llvm/libcxx/test/support"
out="$workspace/tmp/R52/upstream-tests/x86_64"
mkdir -p "$out"

tests=(
  "istream_getline|codes/llvm/libcxx/test/std/input.output/iostream.format/input.streams/istream.unformatted/getline_pointer_size.pass.cpp"
  "istream_ws|codes/llvm/libcxx/test/std/input.output/iostream.format/input.streams/istream.manip/ws.pass.cpp"
  "future_get_future|codes/llvm/libcxx/test/std/thread/futures/futures.task/futures.task.members/get_future.pass.cpp"
  "future_operator|codes/llvm/libcxx/test/std/thread/futures/futures.task/futures.task.members/operator.pass.cpp"
  "future_async|codes/llvm/libcxx/test/std/thread/futures/futures.async/async.pass.cpp"
  "string_shrink_to_fit|codes/llvm/libcxx/test/std/strings/basic.string/string.capacity/shrink_to_fit.pass.cpp"
)

overall=0
for item in "${tests[@]}"; do
  name=${item%%|*}
  source=${item#*|}
  printf '\nCELL_BEGIN matrix=selected_upstream arch=x86_64 test=%s source=%s\n' "$name" "$source"
  cmd=("$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++
       -I "$overlay" -I "$headers" -I "$support" "$workspace/$source"
       -L "$abi" -L "$libcxx" -L "$sysroot/usr/lib64" -Wl,-z,now
       -Wl,-rpath-link,"$libcxx" -lc++ -lc++abi -o "$out/$name")
  printf 'COMPILE_COMMAND='; printf '%q ' "${cmd[@]}"; printf '\n'
  nice -n 15 ionice -c 3 "${cmd[@]}"
  compile_rc=$?
  printf 'COMPILE_EXIT_CODE=%d\n' "$compile_rc"
  if ((compile_rc == 0)); then
    sha256sum "$out/$name"
    printf 'RUN_COMMAND='; printf '%q ' "$loader" --library-path "$abi:$libcxx:$sysroot/usr/lib64:$sysroot/lib64" "$out/$name"; printf '\n'
    "$loader" --library-path "$abi:$libcxx:$sysroot/usr/lib64:$sysroot/lib64" "$out/$name"
    run_rc=$?
  else
    run_rc=125
  fi
  printf 'RUN_EXIT_CODE=%d\nCELL_END matrix=selected_upstream arch=x86_64 test=%s\n' "$run_rc" "$name"
  ((compile_rc == 0 && run_rc == 0)) || overall=1
done
exit "$overall"
