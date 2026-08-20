#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R36/tools/armv7l-clang++"
base_headers="$workspace/tmp/R51/build-product-armv7l/include/c++/v1"
overlay="$workspace/tmp/R53/overlay/include"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
gxx_root="$workspace/tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/c++"
out="$workspace/tmp/R53/tests/armv7l-dso"
mkdir -p "$out"
run() { printf 'COMMAND='; printf '%q ' "$@"; printf '\n'; nice -n 15 ionice -c 3 "$@"; printf 'EXIT_CODE=0\n'; }

for name in cout failbit istream; do
  src="$workspace/progress/R53/src/${name}_probe_dso.cpp"
  for variant in stale rebuilt; do
    first=$base_headers; [[ $variant == rebuilt ]] && first=$overlay
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I "$first" -I "$base_headers" -fPIC -fvisibility=hidden -c "$src" -o "$out/${name}_${variant}.o"
    run "$cxx" -shared "$out/${name}_${variant}.o" -nostdlib++ -L "$baseline" -Wl,-z,now \
      -Wl,--allow-shlib-undefined -Wl,-rpath-link,"$baseline" -lc++ -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
      -o "$out/lib${name}_${variant}.so"
  done
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -fPIC -fvisibility=hidden -nostdinc++ \
    -I "$gxx_root" -I "$gxx_root/armv7l-tizen-linux-gnueabi" -I "$gxx_root/backward" \
    -c "$src" -o "$out/${name}_libstdcxx.o"
  run "$cxx" -shared "$out/${name}_libstdcxx.o" -pthread -Wl,-z,now -o "$out/lib${name}_libstdcxx.so"
done
run "$cxx" -x c -std=c11 -O0 -g -nostdlib++ -c "$workspace/progress/R53/src/dso_launcher.c" -o "$out/dso_launcher.o"
run "$cxx" "$out/dso_launcher.o" -nostdlib++ -ldl -lc -lgcc_s -lgcc -o "$out/dso_launcher"
sha256sum "$out"/*.so "$out/dso_launcher"
