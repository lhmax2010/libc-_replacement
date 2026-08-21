#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
out="$workspace/tmp/R58/probes/$arch"
raw="$workspace/progress/R58/raw/build/probes_$arch"
mkdir -p "$out" "$raw"

if [[ $arch == x86_64 ]]; then
  cxx="$workspace/progress/R33/tools/tizen-clang++"
  sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
elif [[ $arch == armv7l ]]; then
  cxx="$workspace/progress/R36/tools/armv7l-clang++"
  sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
else
  exit 2
fi
baseline="$sysroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
full_headers="$workspace/tmp/R58/build-full-$arch/include/c++/v1"
full_libs="$workspace/tmp/R58/build-full-$arch/lib"

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
  local name=$1 source=$2 headers=$3 libs=$4
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/${name}.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      -stdlib=libc++ -nostdinc++ -I"$headers" "$source" -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -Wl,-rpath-link,"$libs" -Wl,-rpath-link,"$baseline/lib" \
      -lc++ -lc++abi -o "$out/$name"
  else
    run_logged "$raw/${name}_compile.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      -stdlib=libc++ -nostdinc++ -I"$headers" -c "$source" -o "$out/$name.o"
    run_logged "$raw/${name}_link.log" "$cxx" "$out/$name.o" -nostdlib++ \
      -L"$libs" -L"$baseline/lib" -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm \
      -latomic -lc -lgcc_s -lgcc -o "$out/$name"
  fi
}

build_libstdcxx() {
  local name=$1 source=$2
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/${name}.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      "$source" -Wl,-z,now -o "$out/$name"
  else
    run_logged "$raw/${name}_compile.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      -c "$source" -o "$out/$name.o"
    run_logged "$raw/${name}_link.log" "$cxx" "$out/$name.o" -pthread -Wl,-z,now \
      -o "$out/$name"
  fi
}

facility="$workspace/progress/R54/src/facility_probe.cpp"
concurrency="$workspace/progress/R54/src/concurrency_probe.cpp"
catchall="$workspace/progress/R54/src/catchall_rethrow_probe.cpp"
async_probe="$workspace/progress/R55/src/async_probe.cpp"
noncancel="$workspace/progress/R52/src/noncancel_probe.cpp"

build_libcxx facility_baseline "$facility" "$baseline/include/c++/v1" "$baseline/lib"
build_libcxx facility_full "$facility" "$full_headers" "$full_libs"
build_libstdcxx facility_reference "$facility"
build_libcxx concurrency_full "$concurrency" "$full_headers" "$full_libs"
build_libstdcxx concurrency_reference "$concurrency"
build_libcxx catchall_baseline "$catchall" "$baseline/include/c++/v1" "$baseline/lib"
build_libcxx async_full "$async_probe" "$full_headers" "$full_libs"
build_libcxx async_baseline "$async_probe" "$baseline/include/c++/v1" "$baseline/lib"
build_libstdcxx async_reference "$async_probe"
build_libcxx noncancel_full "$noncancel" "$full_headers" "$full_libs"
build_libcxx noncancel_baseline "$noncancel" "$baseline/include/c++/v1" "$baseline/lib"
build_libstdcxx noncancel_reference "$noncancel"

sha256sum "$out"/* > "$raw/artifact_sha256.txt"
for binary in "$out"/*; do
  [[ -x $binary ]] || continue
  file "$binary"
  readelf -dW "$binary"
done > "$raw/elf_identity.txt"

