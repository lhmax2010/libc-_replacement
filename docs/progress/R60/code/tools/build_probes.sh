#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
out="$workspace/tmp/R60/probes/$arch"
raw="$workspace/progress/R60/raw/build/probes_$arch"
mkdir -p "$out" "$raw"

if [[ $arch == x86_64 ]]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
elif [[ $arch == armv7l ]]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
else
  printf 'unsupported architecture: %s\n' "$arch" >&2
  exit 2
fi

baseline="$sysroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
headers="$workspace/tmp/R60/build-$arch/include/c++/v1"
libs="$workspace/tmp/R60/build-$arch/lib"

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

build_libcxx() {
  local name=$1 source=$2
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/${name}.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$headers" "$source" -L"$libs" -L"$baseline/lib" -Wl,-z,now -Wl,-rpath-link,"$libs" \
      -Wl,-rpath-link,"$baseline/lib" -lc++ -lc++abi -o "$out/$name"
  else
    run_logged "$raw/${name}_compile.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
      -nostdinc++ -I"$headers" -c "$source" -o "$out/$name.o"
    run_logged "$raw/${name}_link.log" "$cxx" "$out/$name.o" -nostdlib++ -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$out/$name"
  fi
}

build_libstdcxx() {
  local name=$1 source=$2
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/${name}.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread "$source" -Wl,-z,now \
      -o "$out/$name"
  else
    run_logged "$raw/${name}_compile.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -c "$source" \
      -o "$out/$name.o"
    run_logged "$raw/${name}_link.log" "$cxx" "$out/$name.o" -pthread -Wl,-z,now -o "$out/$name"
  fi
}

r59="$workspace/docs/progress/R59/code/src/r59_probe.cpp"
concurrency="$workspace/progress/R60/src/m1_concurrency_probe.cpp"
build_libcxx r60_current "$r59"
build_libstdcxx r60_reference "$r59"
build_libcxx m1_concurrency_current "$concurrency"
build_libstdcxx m1_concurrency_reference "$concurrency"

sha256sum "$out"/* > "$raw/artifact_sha256.txt"
for binary in "$out"/*; do
  [[ -x $binary ]] || continue
  file "$binary"
  readelf -dW "$binary"
done > "$raw/elf_identity.txt"
