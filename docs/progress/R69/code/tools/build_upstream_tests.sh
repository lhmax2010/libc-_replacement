#!/usr/bin/env bash
set -u -o pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
out="$workspace/tmp/R69/upstream/$arch"
raw="$workspace/progress/R69/raw/upstream-build-$arch"
mkdir -p "$out/libcxxabi" "$out/libstdcxx" "$out/libcxx" "$raw"

if [[ $arch == x86_64 ]]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
else
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
fi
baseline="$sysroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
headers="$workspace/tmp/R69/build-$arch/include/c++/v1"
libs="$workspace/tmp/R69/build-$arch/lib"
support="$workspace/tmp/R69/replay/libcxx/test/support"

run_logged() {
  local log=$1
  shift
  printf 'COMMAND=' > "$log"; printf '%q ' "$@" >> "$log"; printf '\n' >> "$log"
  set +e
  nice -n 15 ionice -c 3 "$@" >> "$log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  return "$rc"
}

compile_libcxx() {
  local name=$1 source=$2 standard=${3:-c++17}
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/libcxx_$name.log" "$cxx" -std="$standard" -O0 -g -fexceptions -pthread \
      -stdlib=libc++ -nostdinc++ -I"$headers" -I"$support" "$source" -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -Wl,-rpath-link,"$libs" -Wl,-rpath-link,"$baseline/lib" -lc++ -lc++abi -o "$out/libcxx/$name"
  else
    run_logged "$raw/libcxx_${name}_compile.log" "$cxx" -std="$standard" -O0 -g -fexceptions -pthread \
      -stdlib=libc++ -nostdinc++ -I"$headers" -I"$support" -c "$source" -o "$out/libcxx/$name.o" || return $?
    run_logged "$raw/libcxx_${name}_link.log" "$cxx" "$out/libcxx/$name.o" -nostdlib++ -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$out/libcxx/$name"
  fi
}

compile_abi() {
  local name=$1 source=$2 opt=-O0
  [[ $name == unwind_06 ]] && opt=-Os
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/abi_$name.log" "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$headers" -I"$workspace/tmp/R69/replay/libcxxabi/include" "$source" \
      -L"$libs" -L"$baseline/lib" -Wl,-z,now -Wl,-rpath-link,"$libs" -Wl,-rpath-link,"$baseline/lib" \
      -lc++ -lc++abi -o "$out/libcxxabi/$name"
    run_logged "$raw/reference_$name.log" "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread \
      -I"$workspace/tmp/R69/replay/libcxxabi/include" "$source" -Wl,-z,now -o "$out/libstdcxx/$name"
  else
    run_logged "$raw/abi_${name}_compile.log" "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$headers" -I"$workspace/tmp/R69/replay/libcxxabi/include" -c "$source" -o "$out/libcxxabi/$name.o" || return $?
    run_logged "$raw/abi_${name}_link.log" "$cxx" "$out/libcxxabi/$name.o" -nostdlib++ -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$out/libcxxabi/$name"
    run_logged "$raw/reference_${name}_compile.log" "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread \
      -I"$workspace/tmp/R69/replay/libcxxabi/include" -c "$source" -o "$out/libstdcxx/$name.o" || return $?
    run_logged "$raw/reference_${name}_link.log" "$cxx" "$out/libstdcxx/$name.o" -pthread -Wl,-z,now -o "$out/libstdcxx/$name"
  fi
}

overall=0
for name in forced_unwind1 forced_unwind2 forced_unwind3 forced_unwind4 unwind_01 unwind_02 unwind_03 unwind_04 unwind_05 unwind_06; do
  compile_abi "$name" "$workspace/progress/R69/src/upstream/$name.pass.cpp" || overall=1
done

tests=(
  'future_async|libcxx/test/std/thread/futures/futures.async/async.pass.cpp'
  'future_async_race|libcxx/test/std/thread/futures/futures.async/async_race.pass.cpp'
  'future_async_race_38682|libcxx/test/std/thread/futures/futures.async/async_race.38682.pass.cpp'
  'future_wait_on_destruct|libcxx/test/std/thread/futures/futures.async/wait_on_destruct.pass.cpp'
  'future_thread_create_failure|libcxx/test/std/thread/futures/futures.async/thread_create_failure.pass.cpp'
  'istream_getline|libcxx/test/std/input.output/iostream.format/input.streams/istream.unformatted/getline_pointer_size.pass.cpp'
  'istream_read|libcxx/test/std/input.output/iostream.format/input.streams/istream.unformatted/read.pass.cpp'
  'istream_ws|libcxx/test/std/input.output/iostream.format/input.streams/istream.manip/ws.pass.cpp'
  'future_get_future|libcxx/test/std/thread/futures/futures.task/futures.task.members/get_future.pass.cpp'
  'future_task_operator|libcxx/test/std/thread/futures/futures.task/futures.task.members/operator.pass.cpp'
  'string_shrink_to_fit|libcxx/test/std/strings/basic.string/string.capacity/shrink_to_fit.pass.cpp'
)
for item in "${tests[@]}"; do
  name=${item%%|*}; path=${item#*|}
  compile_libcxx "$name" "$workspace/tmp/R69/replay/$path" || overall=1
done

find "$out" -type f -perm -0100 -print0 | sort -z | xargs -0 sha256sum > "$raw/artifact_sha256.txt"
exit "$overall"
