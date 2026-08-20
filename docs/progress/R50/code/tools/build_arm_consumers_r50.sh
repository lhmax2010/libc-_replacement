#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R36/tools/armv7l-clang++"
src="$workspace/progress/R50/code/tests/r50_cancel_probe.cpp"
launcher_src="$workspace/progress/R50/code/tests/dso_launcher.c"
out="$workspace/tmp/R50/tests/armv7l"
base="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
headers="$base/include/c++/v1"
libdir="$base/lib"
overlay="$workspace/tmp/R50/overlay/include"
r45_headers="$workspace/tmp/R45/build-armv7l/include/c++/v1"
sysroot="$workspace/tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0"
gxx="$sysroot/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/c++"
mkdir -p "$out"

run() { printf 'COMMAND='; printf '%q ' "$@"; printf '\n'; nice -n 15 ionice -c 3 "$@"; local rc=$?; printf 'EXIT_CODE=%d\n' "$rc"; return "$rc"; }

build_libcxx_dso() {
  local name=$1 include1=$2
  local object="$out/$name.o" dso="$out/lib$name.so"
  run "$cxx" -std=c++17 -O0 -g2 -gdwarf-4 -fexceptions -pthread \
    -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon \
    -mfloat-abi=softfp -mthumb -Wp,-D__SOFTFP__ -fPIC \
    -fvisibility=hidden -fvisibility-inlines-hidden -stdlib=libc++ \
    -nostdinc++ -I"$include1" -I"$r45_headers" -c "$src" -o "$object"
  run "$cxx" -shared "$object" -nostdlib++ -L"$libdir" -Wl,-z,now \
    -Wl,--allow-shlib-undefined -Wl,-rpath-link,"$libdir" \
    -lc++ -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$dso"
}

build_libcxx_dso unpatched "$r45_headers"
build_libcxx_dso patched "$overlay"

run "$cxx" -std=c++17 -O0 -g2 -gdwarf-4 -fexceptions -pthread -fPIC \
  -nostdinc++ -I"$gxx" -I"$gxx/armv7l-tizen-linux-gnueabi" -I"$gxx/backward" \
  -c "$src" -o "$out/libstdcxx.o"
run "$cxx" -shared "$out/libstdcxx.o" -pthread -Wl,-z,now -o "$out/liblibstdcxx.so"
run "$cxx" -x c -std=c11 -O0 -g -nostdinc -nostdlib++ -c "$launcher_src" -o "$out/dso_launcher.o"
run "$cxx" "$out/dso_launcher.o" -nostdlib++ -ldl -lc -lgcc_s -lgcc -o "$out/dso_launcher"

for f in "$out"/libunpatched.so "$out"/libpatched.so "$out"/liblibstdcxx.so "$out"/dso_launcher; do
  file "$f"; sha256sum "$f"; readelf -dW "$f"; readelf --dyn-syms -W "$f" | grep -E 'UND|__forced_unwind' || true
done
