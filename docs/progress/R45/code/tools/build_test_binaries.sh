#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
source_dir="$workspace/progress/R45/src/upstream"
out="$workspace/tmp/R45/tests/$arch"
mkdir -p "$out/libcxx" "$out/libstdcxx"

run() {
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
  "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

tests=(forced_unwind1 forced_unwind2 forced_unwind3 forced_unwind4
       unwind_01 unwind_02 unwind_03 unwind_04 unwind_05 unwind_06)

if [[ $arch == x86_64 ]]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  headers="$workspace/tmp/R45/build-x86_64/include/c++/v1"
  abi="$workspace/tmp/R45/runtime/x86_64"
  sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
  baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
  for name in "${tests[@]}"; do
    opt=-O0
    [[ $name == unwind_06 ]] && opt=-Os
    run "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$headers" -I"$workspace/tmp/R45/source-patched/libcxxabi/include" \
      "$source_dir/$name.pass.cpp" -L"$abi" -L"$baseline" -L"$sysroot/usr/lib64" \
      -Wl,-z,now -Wl,-rpath-link,"$baseline" -lc++ -lc++abi -o "$out/libcxx/$name"
    run "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread \
      -I"$workspace/codes/llvm/libcxxabi/include" -c "$source_dir/$name.pass.cpp" \
      -o "$out/libstdcxx/$name.o"
    run "$cxx" "$out/libstdcxx/$name.o" -pthread -Wl,-z,now \
      -o "$out/libstdcxx/$name"
  done
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$headers" "$workspace/progress/R45/src/s1_rethrow_probe.cpp" \
    -L"$abi" -L"$baseline" -L"$sysroot/usr/lib64" -Wl,-z,now \
    -Wl,-rpath-link,"$baseline" -lc++ -lc++abi -o "$out/libcxx/s1_rethrow_probe"
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
    "$workspace/progress/R45/src/s1_rethrow_probe.cpp" -Wl,-z,now \
    -o "$out/libstdcxx/s1_rethrow_probe"
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$headers" "$workspace/progress/R45/src/unpatched_cancel_probe.cpp" \
    -L"$baseline" -L"$sysroot/usr/lib64" -Wl,-z,now -Wl,-rpath-link,"$baseline" \
    -lc++ -lc++abi -o "$out/libcxx/unpatched_cancel_probe"
elif [[ $arch == armv7l ]]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  headers="$workspace/tmp/R45/build-armv7l/include/c++/v1"
  abi="$workspace/tmp/R45/build-armv7l/lib"
  static_libcxx="$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++.a"
  unpatched_abi="$workspace/tmp/R38/build-unpatched-armv7l/lib"
  for name in "${tests[@]}"; do
    opt=-O0
    [[ $name == unwind_06 ]] && opt=-Os
    run "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$headers" -I"$workspace/tmp/R45/source-patched/libcxxabi/include" \
      -c "$source_dir/$name.pass.cpp" -o "$out/libcxx/$name.o"
    run "$cxx" "$out/libcxx/$name.o" -nostdlib++ "$static_libcxx" -L"$abi" \
      -Wl,-z,now -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
      -o "$out/libcxx/$name"
    run "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread \
      -I"$workspace/codes/llvm/libcxxabi/include" -c "$source_dir/$name.pass.cpp" \
      -o "$out/libstdcxx/$name.o"
    run "$cxx" "$out/libstdcxx/$name.o" -pthread -Wl,-z,now \
      -o "$out/libstdcxx/$name"
  done
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$headers" -Dprivate_1=unwinder_cache.reserved1 \
    -c "$workspace/progress/R45/src/s1_rethrow_probe.cpp" -o "$out/libcxx/s1_rethrow_probe.o"
  run "$cxx" "$out/libcxx/s1_rethrow_probe.o" -nostdlib++ "$static_libcxx" -L"$abi" \
    -Wl,-z,now -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
    -o "$out/libcxx/s1_rethrow_probe"
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
    -Dprivate_1=unwinder_cache.reserved1 -c "$workspace/progress/R45/src/s1_rethrow_probe.cpp" \
    -o "$out/libstdcxx/s1_rethrow_probe.o"
  run "$cxx" "$out/libstdcxx/s1_rethrow_probe.o" -pthread -Wl,-z,now \
    -o "$out/libstdcxx/s1_rethrow_probe"
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$headers" -c "$workspace/progress/R45/src/unpatched_cancel_probe.cpp" \
    -o "$out/libcxx/unpatched_cancel_probe.o"
  run "$cxx" "$out/libcxx/unpatched_cancel_probe.o" -nostdlib++ "$static_libcxx" \
    -L"$unpatched_abi" -Wl,-z,now -lc++abi -lpthread -ldl -lm -latomic -lc \
    -lgcc_s -lgcc -o "$out/libcxx/unpatched_cancel_probe"
else
  printf 'ERROR: unsupported architecture: %s\n' "$arch" >&2
  exit 2
fi

for binary in "$out"/libcxx/* "$out"/libstdcxx/*; do
  [[ -x $binary ]] || continue
  file "$binary"
  sha256sum "$binary"
  readelf -dW "$binary"
done
