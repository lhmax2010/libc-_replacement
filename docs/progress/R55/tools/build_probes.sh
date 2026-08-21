#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?arch required}
out="$workspace/tmp/R55/probes/$arch"
raw="$workspace/progress/R55/raw/$arch/build"
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
fixed_headers="$workspace/tmp/R55/headers/$arch/include/c++/v1"
full_libs="$workspace/tmp/R54/build-all-$arch/lib"

run_logged() {
  local log=$1
  shift
  {
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } > "$log"
  set +e
  "$@" >> "$log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  return "$rc"
}

for source in ${R55_SOURCES:-async_probe facility_probe}; do
  src="$workspace/progress/R55/src/$source.cpp"
  if [[ $source == facility_probe ]]; then
    src="$workspace/progress/R54/src/facility_probe.cpp"
  fi
  if [[ $arch == x86_64 ]]; then
    run_logged "$raw/${source}_fixed.log" nice -n 15 ionice -c 3 \
      "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$fixed_headers" "$src" -L"$full_libs" -L"$baseline/lib" \
      -Wl,-z,now -Wl,-rpath-link,"$full_libs" -Wl,-rpath-link,"$baseline/lib" \
      -lc++ -lc++abi -o "$out/${source}_fixed_libcxx"
    run_logged "$raw/${source}_baseline.log" nice -n 15 ionice -c 3 \
      "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$baseline/include/c++/v1" "$src" -L"$baseline/lib" \
      -Wl,-z,now -Wl,-rpath-link,"$baseline/lib" -lc++ -lc++abi \
      -o "$out/${source}_baseline_libcxx"
    run_logged "$raw/${source}_libstdcxx.log" nice -n 15 ionice -c 3 \
      "$cxx" -std=c++17 -O0 -g -fexceptions -pthread "$src" -Wl,-z,now \
      -o "$out/${source}_libstdcxx"
  else
    run_logged "$raw/${source}_fixed_compile.log" nice -n 15 ionice -c 3 \
      "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$fixed_headers" -c "$src" -o "$out/${source}_fixed_libcxx.o"
    run_logged "$raw/${source}_fixed_link.log" nice -n 15 ionice -c 3 \
      "$cxx" "$out/${source}_fixed_libcxx.o" -nostdlib++ -L"$full_libs" -L"$baseline/lib" \
      -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
      -o "$out/${source}_fixed_libcxx"
    run_logged "$raw/${source}_baseline_compile.log" nice -n 15 ionice -c 3 \
      "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
      -I"$baseline/include/c++/v1" -c "$src" -o "$out/${source}_baseline_libcxx.o"
    run_logged "$raw/${source}_baseline_link.log" nice -n 15 ionice -c 3 \
      "$cxx" "$out/${source}_baseline_libcxx.o" -nostdlib++ -L"$baseline/lib" \
      -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc \
      -o "$out/${source}_baseline_libcxx"
    run_logged "$raw/${source}_libstdcxx_compile.log" nice -n 15 ionice -c 3 \
      "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -c "$src" \
      -o "$out/${source}_libstdcxx.o"
    run_logged "$raw/${source}_libstdcxx_link.log" nice -n 15 ionice -c 3 \
      "$cxx" "$out/${source}_libstdcxx.o" -pthread -Wl,-z,now \
      -o "$out/${source}_libstdcxx"
  fi
done

sha256sum "$out"/* > "$raw/artifact_sha256.txt"
for binary in "$out"/*_libcxx "$out"/*_libstdcxx; do
  file "$binary"
  readelf -dW "$binary"
done > "$raw/elf_identity.txt"
