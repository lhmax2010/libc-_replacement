#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R36/tools/armv7l-clang++"
sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
baseline="$sysroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
headers="$workspace/tmp/W2/build-armv7l/include/c++/v1"
libs="$workspace/tmp/W2/build-armv7l/lib"
out="$workspace/tmp/W2/probes/armv7l"
raw="$workspace/progress/W2/raw/build_probes"
mkdir -p "$out" "$raw"

run_logged() {
  local log=$1
  shift
  printf 'COMMAND=' > "$log"
  printf '%q ' "$@" >> "$log"
  printf '\n' >> "$log"
  set +e
  nice -n 15 ionice -c 3 "$@" >> "$log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  return "$rc"
}

build_libcxx() {
  local name=$1 source=$2
  run_logged "$raw/${name}_compile.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ \
    -nostdinc++ -I"$headers" -c "$source" -o "$out/$name.o"
  run_logged "$raw/${name}_link.log" "$cxx" "$out/$name.o" -nostdlib++ -L"$libs" -L"$baseline/lib" \
    -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$out/$name"
}

build_libstdcxx() {
  local name=$1 source=$2
  run_logged "$raw/${name}_compile.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -c "$source" -o "$out/$name.o"
  run_logged "$raw/${name}_link.log" "$cxx" "$out/$name.o" -pthread -Wl,-z,now -o "$out/$name"
}

build_pair() {
  local stem=$1 source=$2
  build_libcxx "${stem}_fixed" "$source"
  build_libstdcxx "${stem}_reference" "$source"
}

build_pair r61 "$workspace/progress/W1/src/r61_probe.cpp"
build_pair m2 "$workspace/progress/W1/src/r61_m2_matrix.cpp"
build_pair m1_concurrency "$workspace/progress/R60/src/m1_concurrency_probe.cpp"
build_pair facility "$workspace/progress/R54/src/facility_probe.cpp"
build_pair async "$workspace/progress/R55/src/async_probe.cpp"
build_pair noncancel "$workspace/progress/R52/src/noncancel_probe.cpp"
build_pair normal "$workspace/progress/R58/src/normal_paths_probe.cpp"

sha256sum "$out"/* > "$raw/artifact_sha256.txt"
for binary in "$out"/*; do
  [[ -x $binary ]] || continue
  file "$binary"
  readelf -dW "$binary"
done > "$raw/elf_identity.txt"
