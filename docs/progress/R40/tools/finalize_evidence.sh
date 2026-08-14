#!/bin/bash
set -euo pipefail

repo=/home/toolchain/development/libc++_replacement
root="$repo/progress/R40"
code="$root/code"

rm -rf "$code"
mkdir -p \
  "$code/implementation/platform_baseline/libcxxabi/include" \
  "$code/implementation/platform_baseline/libcxxabi/src" \
  "$code/implementation/r34b_input/libcxxabi/include" \
  "$code/implementation/r34b_input/libcxxabi/src" \
  "$code/implementation/diagnostic/current/libcxxabi/include" \
  "$code/implementation/diagnostic/current/libcxxabi/src" \
  "$code/implementation/strong/current/libcxxabi/include" \
  "$code/implementation/strong/current/libcxxabi/src" \
  "$code/implementation/visibility/current/libcxxabi/include" \
  "$code/implementation/visibility/current/libcxxabi/src" \
  "$code/tests" "$code/tools" "$code/toolchains" "$code/config" \
  "$code/reference/libgcc" "$code/evidence/commands" "$code/patches"

cp "$repo/codes/llvm/libcxxabi/include/cxxabi.h" \
  "$code/implementation/platform_baseline/libcxxabi/include/"
cp "$repo/codes/llvm/libcxxabi/src/cxa_exception.cpp" \
  "$repo/codes/llvm/libcxxabi/src/cxa_personality.cpp" \
  "$code/implementation/platform_baseline/libcxxabi/src/"

cp "$repo/tmp/R34B/llvm-exp/libcxxabi/include/cxxabi.h" \
  "$code/implementation/r34b_input/libcxxabi/include/"
cp "$repo/tmp/R34B/llvm-exp/libcxxabi/src/cxa_exception.cpp" \
  "$repo/tmp/R34B/llvm-exp/libcxxabi/src/cxa_personality.cpp" \
  "$code/implementation/r34b_input/libcxxabi/src/"

for variant in diagnostic strong visibility; do
  cp "$repo/tmp/R40/source-$variant/libcxxabi/include/cxxabi.h" \
    "$code/implementation/$variant/current/libcxxabi/include/"
  cp "$repo/tmp/R40/source-$variant/libcxxabi/src/cxa_exception.cpp" \
    "$repo/tmp/R40/source-$variant/libcxxabi/src/cxa_personality.cpp" \
    "$code/implementation/$variant/current/libcxxabi/src/"
done

cp "$repo/progress/R34B/src/s1_rethrow_probe.cpp" "$code/tests/"
cp "$repo/progress/R40/tools/"*.sh "$code/tools/"
cp "$repo/progress/R33/tools/tizen-clang" \
  "$repo/progress/R33/tools/tizen-clang++" \
  "$repo/progress/R36/tools/armv7l-clang" \
  "$repo/progress/R36/tools/armv7l-clang++" "$code/toolchains/"
cp "$repo/codes/gcc/libgcc/unwind-arm-common.inc" "$code/reference/libgcc/"
cp "$repo/packaging/libcxx-llvm22.map" "$code/reference/"
cp "$repo/progress/R40/patches/"*.diff "$code/patches/"
cp "$repo/progress/R40/commands/"*.log "$code/evidence/commands/"

for variant in diagnostic strong visibility; do
  for arch in x86_64 armv7l; do
    mkdir -p "$code/config/${variant}-${arch}"
    cp "$repo/tmp/R40/build-${variant}-${arch}/CMakeCache.txt" \
      "$code/config/${variant}-${arch}/"
  done
done

index="$code/INDEX.tsv"
printf 'upload_path\tsource\tsha256\n' > "$index"
while IFS= read -r file; do
  rel=${file#"$code/"}
  case "$rel" in
    implementation/platform_baseline/*)
      source="codes/llvm/${rel#implementation/platform_baseline/}" ;;
    implementation/r34b_input/*)
      source="tmp/R34B/llvm-exp/${rel#implementation/r34b_input/}" ;;
    implementation/diagnostic/current/*)
      source="tmp/R40/source-diagnostic/${rel#implementation/diagnostic/current/}" ;;
    implementation/strong/current/*)
      source="tmp/R40/source-strong/${rel#implementation/strong/current/}" ;;
    implementation/visibility/current/*)
      source="tmp/R40/source-visibility/${rel#implementation/visibility/current/}" ;;
    tests/*) source="progress/R34B/src/${rel#tests/}" ;;
    tools/*) source="progress/R40/tools/${rel#tools/}" ;;
    toolchains/tizen-*) source="progress/R33/tools/${rel#toolchains/}" ;;
    toolchains/armv7l-*) source="progress/R36/tools/${rel#toolchains/}" ;;
    reference/libgcc/*) source="codes/gcc/libgcc/${rel#reference/libgcc/}" ;;
    reference/libcxx-llvm22.map) source="packaging/libcxx-llvm22.map" ;;
    patches/*) source="progress/R40/patches/${rel#patches/}" ;;
    evidence/commands/*) source="progress/R40/commands/${rel#evidence/commands/}" ;;
    config/*)
      pair=${rel#config/}; pair=${pair%%/*}; variant=${pair%-*}; arch=${pair##*-}
      source="tmp/R40/build-${variant}-${arch}/${rel##*/}" ;;
    *) source=NOT_AVAILABLE ;;
  esac
  printf '%s\t%s\t%s\n' "docs/progress/R40/code/$rel" "$source" \
    "$(sha256sum "$file" | awk '{print $1}')" >> "$index"
done < <(find "$code" -type f ! -name INDEX.tsv | sort)

find "$root" -type f ! -path "$root/MANIFEST.sha256" -printf '%P\0' \
  | sort -z \
  | while IFS= read -r -d '' rel; do sha256sum "$root/$rel"; done \
  | sed "s#  $root/#  ./#" > "$root/MANIFEST.sha256"

docs="$repo/docs/progress/R40"
rm -rf "$docs"
mkdir -p "$(dirname "$docs")"
cp -a "$root" "$docs"

printf 'CURATED_FILE_COUNT=%s\n' "$(find "$docs" -type f | wc -l)"
printf 'CODE_FILE_COUNT=%s\n' "$(find "$docs/code" -type f | wc -l)"
printf 'MAX_FILE_BYTES=%s\n' "$(find "$docs" -type f -printf '%s\n' | sort -nr | head -n1)"
printf 'FILES_OVER_5M=%s\n' "$(find "$docs" -type f -size +5M | wc -l)"

while IFS= read -r line; do
  hash=${line%%  *}
  rel=${line#*  ./}
  printf '%s  %s\n' "$hash" "$docs/$rel"
done < "$docs/MANIFEST.sha256" | sha256sum -c -

cmp "$root/R40_report.md" "$docs/R40_report.md"
cmp "$root/code/INDEX.tsv" "$docs/code/INDEX.tsv"
printf 'FINALIZE_EVIDENCE=PASS\n'
