#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
cxx="$workspace/progress/R33/tools/tizen-clang++"
sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
baseline="$sysroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
full="$workspace/tmp/R58/build-full-x86_64"
source="$workspace/progress/R59/src/r59_probe.cpp"
out="$workspace/tmp/R59/bin"
raw="$workspace/progress/R59/raw/build"
mkdir -p "$out" "$raw"

run() {
  local name=$1; shift
  printf 'COMMAND=' > "$raw/$name.log"; printf '%q ' "$@" >> "$raw/$name.log"; printf '\n' >> "$raw/$name.log"
  set +e
  nice -n 15 ionice -c 3 "$@" >> "$raw/$name.log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" >> "$raw/$name.log"
  return "$rc"
}

common=(-std=c++17 -O0 -g -fexceptions -pthread -Wl,-z,now)
run current_compile "$cxx" "${common[@]}" -stdlib=libc++ -nostdinc++ -I"$full/include/c++/v1" "$source" \
  -L"$full/lib" -L"$baseline/lib" -Wl,-rpath-link,"$full/lib" -Wl,-rpath-link,"$baseline/lib" \
  -lc++ -lc++abi -o "$out/current"
run baseline_compile "$cxx" "${common[@]}" -stdlib=libc++ -nostdinc++ -I"$baseline/include/c++/v1" "$source" \
  -L"$baseline/lib" -Wl,-rpath-link,"$baseline/lib" -lc++ -lc++abi -o "$out/baseline"
run reference_compile "$cxx" "${common[@]}" "$source" -o "$out/reference"
sha256sum "$out"/* > "$raw/artifact_sha256.txt"
for f in "$out"/*; do file "$f"; readelf -dW "$f"; done > "$raw/elf_identity.txt"
