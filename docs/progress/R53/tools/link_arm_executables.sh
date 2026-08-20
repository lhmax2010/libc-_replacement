#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R36/tools/armv7l-clang++"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
abi_dir="$workspace/tmp/R51/build-product-armv7l/lib"
out="$workspace/tmp/R53/tests/armv7l-dso"
run() { printf 'COMMAND='; printf '%q ' "$@"; printf '\n'; nice -n 15 ionice -c 3 "$@"; printf 'EXIT_CODE=0\n'; }
run "$cxx" -x c -std=c11 -O0 -g -nostdlib++ -c "$workspace/progress/R53/src/direct_launcher.c" -o "$out/direct_launcher.o"
for name in cout failbit istream; do
  for variant in stale rebuilt; do
    run "$cxx" "$out/direct_launcher.o" "$out/${name}_${variant}.o" -nostdlib++ -L "$abi_dir" -L "$baseline" -Wl,-z,now -Wl,--allow-shlib-undefined \
      -Wl,-rpath-link,"$abi_dir" -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$out/${name}_${variant}_exe"
  done
  run "$cxx" "$out/direct_launcher.o" "$out/${name}_libstdcxx.o" -pthread -Wl,-z,now -o "$out/${name}_libstdcxx_exe"
done
sha256sum "$out"/*_exe
