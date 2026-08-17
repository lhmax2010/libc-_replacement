#!/usr/bin/env bash
set -e
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
source_file="$workspace/progress/R46/src/stdlib_header_site_probe.cpp"
out_dir="$workspace/tmp/R46/tests/$arch"
patched_headers="$workspace/tmp/R46/overlay/include"
r45_abi_headers_x86="$workspace/tmp/R45/build-x86_64/include/c++/v1"
r45_abi_headers_arm="$workspace/tmp/R45/build-armv7l/include/c++/v1"

case "$arch" in
  x86_64)
    cxx="$workspace/progress/R33/tools/tizen-clang++"
    headers="$r45_abi_headers_x86"
    abi_dir="$workspace/tmp/R45/runtime/x86_64"
    baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
    ;;
  armv7l)
    cxx="$workspace/progress/R36/tools/armv7l-clang++"
    headers="$r45_abi_headers_arm"
    abi_dir="$workspace/tmp/R45/build-armv7l/lib"
    compiler_sysroot="$workspace/tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0"
    gxx_headers="$compiler_sysroot/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/c++"
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
  nice -n 15 ionice -c 3 "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

compile_libcxx() {
  local variant=$1
  local include_root=$2
  local object="$out_dir/$variant.o"
  if [[ $arch == x86_64 ]]; then
    local binary="$out_dir/$variant"
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$include_root" -I"$headers" "$source_file" \
      -L"$abi_dir" -L"$baseline" -Wl,-z,now \
      -Wl,-rpath-link,"$abi_dir" -Wl,-rpath-link,"$baseline" \
      -lc++ -lc++abi -o "$binary"
  else
    local binary="$out_dir/lib$variant.so"
    run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$include_root" -I"$headers" -fPIC -c "$source_file" -o "$object"
    run "$cxx" -shared "$object" -nostdlib++ -L"$baseline" -Wl,-z,now \
      -Wl,--allow-shlib-undefined -Wl,-rpath-link,"$baseline" \
      -lc++ -lpthread -ldl -lm \
      -latomic -lc -lgcc_s -lgcc -o "$binary"
  fi
}

compile_libcxx unpatched_headers "$headers"
compile_libcxx patched_headers "$patched_headers"

if [[ $arch == x86_64 ]]; then
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread "$source_file" \
    -o "$out_dir/libstdcxx"
  outputs=("$out_dir/unpatched_headers" "$out_dir/patched_headers" "$out_dir/libstdcxx")
else
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -fPIC -nostdinc++ \
    -I"$gxx_headers" -I"$gxx_headers/armv7l-tizen-linux-gnueabi" \
    -I"$gxx_headers/backward" -c "$source_file" -o "$out_dir/libstdcxx.o"
  run "$cxx" -shared "$out_dir/libstdcxx.o" -pthread -Wl,-z,now \
    -o "$out_dir/liblibstdcxx.so"
  run "$cxx" -x c -std=c11 -O0 -g -nostdinc -nostdlib++ -c \
    "$workspace/progress/R46/src/dso_launcher.c" -o "$out_dir/dso_launcher.o"
  run "$cxx" "$out_dir/dso_launcher.o" -nostdlib++ -ldl -lc -lgcc_s -lgcc \
    -o "$out_dir/dso_launcher"
  outputs=("$out_dir/libunpatched_headers.so" "$out_dir/libpatched_headers.so" \
           "$out_dir/liblibstdcxx.so" "$out_dir/dso_launcher")
fi

for binary in "${outputs[@]}"; do
  file "$binary"
  sha256sum "$binary"
  readelf -dW "$binary"
  readelf --dyn-syms -W "$binary" | grep -E '__forced_unwind|UND' || true
done
