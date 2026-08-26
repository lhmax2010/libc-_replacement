#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
out="$workspace/tmp/R69/probes/$arch"
raw="$workspace/progress/R69/raw/build-probes-$arch"
mkdir -p "$out" "$raw"

case "$arch" in
  x86_64)
    cxx="$workspace/progress/R33/tools/tizen-clang++"
    sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
    ;;
  armv7l)
    cxx="$workspace/progress/R36/tools/armv7l-clang++"
    sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
    ;;
  *) printf 'ERROR: unsupported architecture: %s\n' "$arch" >&2; exit 2 ;;
esac

baseline="$sysroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
fixed_headers="$workspace/tmp/R69/build-$arch/include/c++/v1"
fixed_libs="$workspace/tmp/R69/build-$arch/lib"

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
  local name=$1 source=$2 headers=$3 libs=$4
  local extra=()
  if [[ $arch == armv7l && $name == regression_fixed ]]; then
    extra=(-Dprivate_1=unwinder_cache.reserved1)
  fi
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/$name.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      "${extra[@]}" -stdlib=libc++ -nostdinc++ -I"$headers" "$source" -L"$libs" -L"$baseline/lib" \
      -Wl,-z,now -Wl,-rpath-link,"$libs" -Wl,-rpath-link,"$baseline/lib" \
      -lc++ -lc++abi -o "$out/$name"
  else
    run_logged "$raw/${name}_compile.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      "${extra[@]}" -stdlib=libc++ -nostdinc++ -I"$headers" -c "$source" -o "$out/$name.o"
    run_logged "$raw/${name}_link.log" "$cxx" "$out/$name.o" -nostdlib++ \
      -L"$libs" -L"$baseline/lib" -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm \
      -latomic -lc -lgcc_s -lgcc -o "$out/$name"
  fi
}

build_reference() {
  local name=$1 source=$2
  local extra=()
  if [[ $arch == armv7l && $name == regression_reference ]]; then
    extra=(-Dprivate_1=unwinder_cache.reserved1)
  fi
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/$name.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      "${extra[@]}" "$source" -Wl,-z,now -o "$out/$name"
  else
    run_logged "$raw/${name}_compile.log" "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
      "${extra[@]}" -c "$source" -o "$out/$name.o"
    run_logged "$raw/${name}_link.log" "$cxx" "$out/$name.o" -pthread -Wl,-z,now -o "$out/$name"
  fi
}

source_root="$workspace/progress/R69/src/existing"
r61="$source_root/r61_probe.cpp"
m2_lock="$source_root/m2_lock_outside_probe.cpp"
facility="$source_root/facility_probe.cpp"
concurrency="$source_root/concurrency_probe.cpp"
normal="$source_root/normal_paths_probe.cpp"
regression="$source_root/s1_rethrow_probe.cpp"
reference_return="$source_root/m1_async_reference_compile.cpp"
m1_concurrency="$source_root/m1_concurrency_probe.cpp"
async_probe="$source_root/async_probe.cpp"
new_cases="$workspace/progress/R69/src/r69_new_cases.cpp"

build_libcxx r61_fixed "$r61" "$fixed_headers" "$fixed_libs"
build_reference r61_reference "$r61"
build_libcxx m2_lock_fixed "$m2_lock" "$fixed_headers" "$fixed_libs"
build_reference m2_lock_reference "$m2_lock"
build_libcxx async_reference_return_fixed "$reference_return" "$fixed_headers" "$fixed_libs"
build_reference async_reference_return_reference "$reference_return"
build_libcxx m1_concurrency_fixed "$m1_concurrency" "$fixed_headers" "$fixed_libs"
build_reference m1_concurrency_reference "$m1_concurrency"
build_libcxx async_baseline "$async_probe" "$baseline/include/c++/v1" "$baseline/lib"
build_libcxx async_fixed "$async_probe" "$fixed_headers" "$fixed_libs"
build_reference async_reference "$async_probe"
build_libcxx r69_new_baseline "$new_cases" "$baseline/include/c++/v1" "$baseline/lib"
build_libcxx r69_new_fixed "$new_cases" "$fixed_headers" "$fixed_libs"
build_reference r69_new_reference "$new_cases"
build_libcxx facility_baseline "$facility" "$baseline/include/c++/v1" "$baseline/lib"
build_libcxx facility_fixed "$facility" "$fixed_headers" "$fixed_libs"
build_reference facility_reference "$facility"
build_libcxx concurrency_fixed "$concurrency" "$fixed_headers" "$fixed_libs"
build_reference concurrency_reference "$concurrency"
build_libcxx normal_baseline "$normal" "$baseline/include/c++/v1" "$baseline/lib"
build_libcxx normal_fixed "$normal" "$fixed_headers" "$fixed_libs"
build_reference normal_reference "$normal"
build_libcxx regression_fixed "$regression" "$fixed_headers" "$fixed_libs"
build_reference regression_reference "$regression"

sha256sum "$out"/* > "$raw/artifact_sha256.txt"
for binary in "$out"/*; do
  [[ -x $binary ]] || continue
  file "$binary"
  readelf -dW "$binary"
done > "$raw/elf_identity.txt"
