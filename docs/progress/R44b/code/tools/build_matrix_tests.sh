#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
source_file="$workspace/progress/R43b/src/stdlib_cancel_probe.cpp"
out_dir="$workspace/tmp/R44b/tests/$arch"
mkdir -p "$out_dir/libcxx" "$out_dir/libstdcxx"

case "$arch" in
  x86_64)
    cxx="$workspace/progress/R33/tools/tizen-clang++"
    headers="$workspace/tmp/R42/build-patched-x86_64/include/c++/v1"
    baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
    ;;
  armv7l)
    cxx="$workspace/progress/R36/tools/armv7l-clang++"
    headers="$workspace/tmp/R42/build-patched-armv7l/include/c++/v1"
    baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
    ;;
  *)
    printf 'unsupported architecture: %s\n' "$arch" >&2
    exit 2
    ;;
esac

run() {
  printf 'COMMAND='
  printf '%q ' "$@"
  printf '\n'
  nice -n 15 ionice -c 3 "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

if [[ $arch == x86_64 ]]; then
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$headers" "$source_file" -L"$baseline" \
    -Wl,-z,now -Wl,-rpath-link,"$baseline" -lc++ -lc++abi \
    -o "$out_dir/libcxx/stdlib_cancel_probe"
else
  run "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$headers" -c "$source_file" \
    -o "$out_dir/libcxx/stdlib_cancel_probe.o"
  run "$cxx" "$out_dir/libcxx/stdlib_cancel_probe.o" -nostdlib++ \
    -L"$baseline" -Wl,-z,now -Wl,-rpath-link,"$baseline" \
    -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
    -o "$out_dir/libcxx/stdlib_cancel_probe"
fi

printf 'COMMAND=cp %q %q\n' \
  "$workspace/progress/R43b/build/$arch/libstdcxx/stdlib_cancel_probe" \
  "$out_dir/libstdcxx/stdlib_cancel_probe"
cp "$workspace/progress/R43b/build/$arch/libstdcxx/stdlib_cancel_probe" \
  "$out_dir/libstdcxx/stdlib_cancel_probe"
printf 'EXIT_CODE=%d\n' "$?"

for binary in "$out_dir/libcxx/stdlib_cancel_probe" "$out_dir/libstdcxx/stdlib_cancel_probe"; do
  file "$binary"
  sha256sum "$binary"
  readelf -dW "$binary"
done
