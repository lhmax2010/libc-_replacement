#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
root="$workspace/tmp/R54"
patch_a="$workspace/progress/R51/patches/0001-Layer-A-libc-abi-preserve-foreign-forced-unwind-stat.patch"
patch_b="$workspace/progress/R51/patches/0002-Layer-B-libc-abi-expose-GNU-forced-unwind-marker-typ.patch"
patch_c_header="$workspace/progress/R52/patches/0001-libcxx-preserve-forced-unwind-across-header-catch-al.patch"
patch_c_library="$workspace/progress/R53/patches/0001-libcxx-preserve-forced-unwind-in-library-catch-sites.patch"

mkdir -p "$root"
test ! -e "$root/source-all"
test ! -e "$root/source-bc"
test ! -e "$root/source-c-only"

cp -a "$workspace/tmp/R51/source-product-final" "$root/source-all"
git apply --check -R --directory="tmp/R54/source-all" "$patch_b"
git apply -R --directory="tmp/R54/source-all" "$patch_b"
git apply --check -R --directory="tmp/R54/source-all" "$patch_a"
git apply -R --directory="tmp/R54/source-all" "$patch_a"

printf 'BASELINE_SHA256_BEGIN\n'
sha256sum \
  "$root/source-all/libcxxabi/include/cxxabi.h" \
  "$root/source-all/libcxxabi/src/cxa_exception.cpp" \
  "$root/source-all/libcxxabi/src/cxa_personality.cpp" \
  "$root/source-all/libcxx/include/future" \
  "$root/source-all/libcxx/include/istream" \
  "$root/source-all/libcxx/include/string" \
  "$root/source-all/libcxx/include/__ostream/basic_ostream.h" \
  "$root/source-all/libcxx/src/ios.cpp"
printf 'BASELINE_SHA256_END\n'

for patch in "$patch_a" "$patch_b" "$patch_c_header" "$patch_c_library"; do
  printf 'APPLY_CHECK patch=%s sha256=' "$patch"
  sha256sum "$patch" | awk '{print $1}'
  git apply --check --directory="tmp/R54/source-all" "$patch"
  printf 'APPLY_CHECK_EXIT_CODE=0\n'
  git apply --directory="tmp/R54/source-all" "$patch"
  printf 'APPLY_EXIT_CODE=0\n'
done

cp -a "$workspace/tmp/R51/source-layer-b-only" "$root/source-bc"
for patch in "$patch_c_header" "$patch_c_library"; do
  git apply --check --directory="tmp/R54/source-bc" "$patch"
  git apply --directory="tmp/R54/source-bc" "$patch"
  printf 'BC_APPLY patch=%s EXIT_CODE=0\n' "$patch"
done

cp -a "$workspace/tmp/R51/source-product-final" "$root/source-c-only"
git apply -R --directory="tmp/R54/source-c-only" "$patch_b"
git apply -R --directory="tmp/R54/source-c-only" "$patch_a"
for patch in "$patch_c_header" "$patch_c_library"; do
  git apply --check --directory="tmp/R54/source-c-only" "$patch"
  git apply --directory="tmp/R54/source-c-only" "$patch"
  printf 'C_ONLY_APPLY patch=%s EXIT_CODE=0\n' "$patch"
done

printf 'FINAL_SHA256_BEGIN\n'
sha256sum \
  "$root/source-all/libcxxabi/include/cxxabi.h" \
  "$root/source-all/libcxxabi/src/cxa_exception.cpp" \
  "$root/source-all/libcxxabi/src/cxa_personality.cpp" \
  "$root/source-all/libcxx/include/future" \
  "$root/source-all/libcxx/include/istream" \
  "$root/source-all/libcxx/include/string" \
  "$root/source-all/libcxx/include/__ostream/basic_ostream.h" \
  "$root/source-all/libcxx/src/ios.cpp"
printf 'FINAL_SHA256_END\n'
