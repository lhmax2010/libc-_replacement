#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
source_file="$workspace/progress/R52/src/header_batch_probe.cpp"
launcher_source="$workspace/progress/R52/src/dso_launcher.c"
out_dir="$workspace/tmp/R52/tests/$arch"
overlay_include="$workspace/tmp/R52/overlay/include"

case "$arch" in
  x86_64)
    cxx="$workspace/progress/R33/tools/tizen-clang++"
    base_headers="$workspace/tmp/R51/build-product-x86_64/include/c++/v1"
    abi_dir="$workspace/tmp/R51/runtime/x86_64"
    baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
    ;;
  armv7l)
    cxx="$workspace/progress/R36/tools/armv7l-clang++"
    base_headers="$workspace/tmp/R51/build-product-armv7l/include/c++/v1"
    abi_dir="$workspace/tmp/R51/build-product-armv7l/lib"
    buildroot="$workspace/tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0"
    gxx_headers="$buildroot/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/c++"
    baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
    ;;
  *)
    printf 'unsupported architecture: %s\n' "$arch" >&2
    exit 2
    ;;
esac

mkdir -p "$out_dir"

run() {
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
  set +e
  nice -n 15 ionice -c 3 "$@"
  rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

compile_libcxx() {
  local variant=$1
  local first_include=$2
  local object="$out_dir/$variant.o"
  if [[ "$arch" == x86_64 ]]; then
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I "$first_include" -I "$base_headers" "$source_file" \
      -L "$abi_dir" -L "$baseline" -Wl,-z,now \
      -Wl,-rpath-link,"$abi_dir" -Wl,-rpath-link,"$baseline" \
      -lc++ -lc++abi -o "$out_dir/$variant"
  else
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I "$first_include" -I "$base_headers" -fPIC \
      -fvisibility=hidden -c "$source_file" -o "$object"
    run "$cxx" -shared "$object" -nostdlib++ -L "$baseline" -Wl,-z,now \
      -Wl,--allow-shlib-undefined -Wl,-rpath-link,"$baseline" \
      -lc++ -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
      -o "$out_dir/lib$variant.so"
  fi
}

compile_libcxx unpatched_headers "$base_headers"
compile_libcxx patched_headers "$overlay_include"

if [[ "$arch" == x86_64 ]]; then
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread "$source_file" \
    -o "$out_dir/libstdcxx"
  outputs=("$out_dir/unpatched_headers" "$out_dir/patched_headers" "$out_dir/libstdcxx")
else
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -fPIC -fvisibility=hidden \
    -nostdinc++ -I "$gxx_headers" -I "$gxx_headers/armv7l-tizen-linux-gnueabi" \
    -I "$gxx_headers/backward" -c "$source_file" -o "$out_dir/libstdcxx.o"
  run "$cxx" -shared "$out_dir/libstdcxx.o" -pthread -Wl,-z,now \
    -o "$out_dir/liblibstdcxx.so"
  run "$cxx" -x c -std=c11 -O0 -g -nostdlib++ -c \
    "$launcher_source" -o "$out_dir/dso_launcher.o"
  run "$cxx" "$out_dir/dso_launcher.o" -nostdlib++ -ldl -lc -lgcc_s -lgcc \
    -o "$out_dir/dso_launcher"
  outputs=("$out_dir/libunpatched_headers.so" "$out_dir/libpatched_headers.so" \
           "$out_dir/liblibstdcxx.so" "$out_dir/dso_launcher")
fi

for output in "${outputs[@]}"; do
  file "$output"
  sha256sum "$output"
  readelf -dW "$output"
  readelf --dyn-syms -W "$output" | grep -E '__forced_unwind|r52_run|UND' || true
done
