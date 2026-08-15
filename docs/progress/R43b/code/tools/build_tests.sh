#!/bin/bash
set -u

workspace=/home/toolchain/development/libc++_replacement
arch=$1
source_dir="$workspace/progress/R43b/src/upstream"
out="$workspace/progress/R43b/build/$arch"
mkdir -p "$out/libcxx" "$out/libstdcxx"

run() {
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
  nice -n 15 ionice -c 3 "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

sources=(
  forced_unwind1.pass.cpp forced_unwind2.pass.cpp forced_unwind3.pass.cpp
  forced_unwind4.pass.cpp unwind_01.pass.cpp unwind_02.pass.cpp
  unwind_03.pass.cpp unwind_04.pass.cpp unwind_05.pass.cpp unwind_06.pass.cpp
)

if [ "$arch" = x86_64 ]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  build="$workspace/tmp/R42/build-patched-x86_64"
  sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
  for source_name in "${sources[@]}"; do
    name=${source_name%.pass.cpp}
    opt=-O0
    [ "$name" = unwind_06 ] && opt=-Os
    run "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$build/include/c++/v1" -I"$workspace/codes/llvm/libcxxabi/include" \
      "$source_dir/$source_name" -L"$sysroot/usr/lib64" -Wl,-z,now \
      -lc++ -lc++abi -o "$out/libcxx/$name" || true
    if run "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread \
      -I"$workspace/codes/llvm/libcxxabi/include" -c "$source_dir/$source_name" \
      -o "$out/libstdcxx/$name.o"; then
      run "$cxx" "$out/libstdcxx/$name.o" -pthread -Wl,-z,now \
        -o "$out/libstdcxx/$name" || true
    fi
  done
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$build/include/c++/v1" "$workspace/progress/R43b/src/stdlib_cancel_probe.cpp" \
    -L"$sysroot/usr/lib64" -Wl,-z,now -lc++ -lc++abi -o "$out/libcxx/stdlib_cancel_probe"
  if run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
    -c "$workspace/progress/R43b/src/stdlib_cancel_probe.cpp" \
    -o "$out/libstdcxx/stdlib_cancel_probe.o"; then
    run "$cxx" "$out/libstdcxx/stdlib_cancel_probe.o" -pthread -Wl,-z,now \
      -o "$out/libstdcxx/stdlib_cancel_probe" || true
  fi
elif [ "$arch" = armv7l ]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  build="$workspace/tmp/R42/build-patched-armv7l"
  static_libcxx="$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++.a"
  unpatched_abi="$workspace/tmp/R38/build-unpatched-armv7l/lib"
  for source_name in "${sources[@]}"; do
    name=${source_name%.pass.cpp}
    opt=-O0
    [ "$name" = unwind_06 ] && opt=-Os
    if run "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$build/include/c++/v1" -I"$workspace/codes/llvm/libcxxabi/include" \
      -c "$source_dir/$source_name" -o "$out/libcxx/$name.o"; then
      run "$cxx" "$out/libcxx/$name.o" -nostdlib++ "$static_libcxx" \
        -L"$unpatched_abi" -Wl,-z,now -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
        -o "$out/libcxx/$name" || true
    fi
    if run "$cxx" -std=c++14 "$opt" -g -fexceptions -pthread \
      -I"$workspace/codes/llvm/libcxxabi/include" -c "$source_dir/$source_name" \
      -o "$out/libstdcxx/$name.o"; then
      run "$cxx" "$out/libstdcxx/$name.o" -pthread -Wl,-z,now \
        -o "$out/libstdcxx/$name" || true
    fi
  done
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$build/include/c++/v1" \
    -c "$workspace/progress/R43b/src/stdlib_cancel_probe.cpp" \
    -o "$out/libcxx/stdlib_cancel_probe.o"
  run "$cxx" "$out/libcxx/stdlib_cancel_probe.o" -nostdlib++ "$static_libcxx" \
    -L"$unpatched_abi" -Wl,-z,now -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
    -o "$out/libcxx/stdlib_cancel_probe"
  if run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
    -c "$workspace/progress/R43b/src/stdlib_cancel_probe.cpp" \
    -o "$out/libstdcxx/stdlib_cancel_probe.o"; then
    run "$cxx" "$out/libstdcxx/stdlib_cancel_probe.o" -pthread -Wl,-z,now \
      -o "$out/libstdcxx/stdlib_cancel_probe" || true
  fi
else
  printf 'UNSUPPORTED_ARCH=%s\n' "$arch" >&2
  exit 2
fi

for binary in "$out"/libcxx/* "$out"/libstdcxx/*; do
  [ -x "$binary" ] || continue
  file "$binary"
  sha256sum "$binary"
  readelf -dW "$binary"
done
