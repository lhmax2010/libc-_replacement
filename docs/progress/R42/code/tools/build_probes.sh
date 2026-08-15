#!/bin/bash
set -eu
workspace=/home/toolchain/development/libc++_replacement
arch=$1
out="$workspace/tmp/R42/probes/$arch"
src="$workspace/progress/R42/src/s1_rethrow_probe.cpp"
control_src="$workspace/progress/R42/src/unpatched_cancel_probe.cpp"
mkdir -p "$out"

run() {
  printf 'COMMAND='; printf '%q ' "$@"; printf '\n'
  nice -n 15 ionice -c 3 "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

if [ "$arch" = x86_64 ]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  build="$workspace/tmp/R42/build-patched-x86_64"
  runtime="$workspace/tmp/R42/runtime/patched/x86_64"
  sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
  run "$cxx" -O0 -g -pthread -stdlib=libc++ -nostdinc++ -I"$build/include/c++/v1" "$src" \
    -L"$runtime" -L"$sysroot/usr/lib64" -Wl,-rpath,'$ORIGIN/../runtime/patched/x86_64' \
    -Wl,-z,now -lc++ -lc++abi -o "$out/s1_libcxx"
  run "$cxx" -O0 -g -pthread "$src" -Wl,-z,now -o "$out/s1_libstdcxx"
  run "$cxx" -O0 -g -pthread -stdlib=libc++ -nostdinc++ -I"$build/include/c++/v1" "$control_src" \
    -L"$sysroot/usr/lib64" -Wl,-z,now -lc++ -lc++abi -o "$out/unpatched_libcxx"
elif [ "$arch" = armv7l ]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  build="$workspace/tmp/R42/build-patched-armv7l"
  static_libcxx="$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++.a"
  common=(-O0 -g -fexceptions -pthread)
  run "$cxx" "${common[@]}" -stdlib=libc++ -nostdinc++ -I"$build/include/c++/v1" \
    -Dprivate_1=unwinder_cache.reserved1 -c "$src" -o "$out/s1_libcxx.o"
  run "$cxx" "$out/s1_libcxx.o" -nostdlib++ "$static_libcxx" -L"$build/lib" \
    -Wl,-z,now -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$out/s1_libcxx"
  run "$cxx" "${common[@]}" -Dprivate_1=unwinder_cache.reserved1 \
    -c "$src" -o "$out/s1_libstdcxx.o"
  run "$cxx" "$out/s1_libstdcxx.o" -pthread -Wl,-z,now -o "$out/s1_libstdcxx"
  run "$cxx" "${common[@]}" -stdlib=libc++ -nostdinc++ -I"$build/include/c++/v1" \
    -c "$control_src" -o "$out/unpatched_libcxx.o"
  run "$cxx" "$out/unpatched_libcxx.o" -nostdlib++ "$static_libcxx" \
    -L"$workspace/tmp/R38/build-unpatched-armv7l/lib" -Wl,-z,now \
    -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$out/unpatched_libcxx"
else
  printf 'unsupported arch: %s\n' "$arch" >&2
  exit 2
fi

for binary in "$out/s1_libcxx" "$out/s1_libstdcxx" "$out/unpatched_libcxx"; do
  file "$binary"
  readelf -dW "$binary"
  readelf --dyn-syms -W "$binary" | grep -E '__forced_unwind|__gxx_personality|__cxa_rethrow|_Unwind_|Num:' || true
  sha256sum "$binary"
done
