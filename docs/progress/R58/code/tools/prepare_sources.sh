#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
root="$workspace/tmp/R58"
source_seed="$workspace/tmp/R51/source-product-final"
patch_a="$workspace/progress/R51/patches/0001-Layer-A-libc-abi-preserve-foreign-forced-unwind-stat.patch"
patch_b="$workspace/progress/R51/patches/0002-Layer-B-libc-abi-expose-GNU-forced-unwind-marker-typ.patch"
patch_c_header="$workspace/progress/R55/patches/0001-Layer-C-header-preserve-forced-unwind-and-ready-async.patch"
patch_c_library="$workspace/progress/R53/patches/0001-libcxx-preserve-forced-unwind-in-library-catch-sites.patch"

mkdir -p "$root"
for directory in source-base source-full source-bc source-c-only; do
  test ! -e "$root/$directory"
done

cp -a "$source_seed" "$root/source-base"
git apply --check -R --directory="tmp/R58/source-base" "$patch_b"
git apply -R --directory="tmp/R58/source-base" "$patch_b"
git apply --check -R --directory="tmp/R58/source-base" "$patch_a"
git apply -R --directory="tmp/R58/source-base" "$patch_a"

cp -a "$root/source-base" "$root/source-full"
cp -a "$root/source-base" "$root/source-bc"
cp -a "$root/source-base" "$root/source-c-only"

apply_one() {
  local source_dir=$1 patch=$2
  printf 'APPLY_CHECK_BEGIN source=%s patch=%s sha256=' "$source_dir" "$patch"
  sha256sum "$patch" | awk '{print $1}'
  git apply --check --directory="${source_dir#"$workspace/"}" "$patch"
  printf 'APPLY_CHECK_EXIT_CODE=0\n'
  git apply --directory="${source_dir#"$workspace/"}" "$patch"
  printf 'APPLY_EXIT_CODE=0\n'
}

apply_one "$root/source-full" "$patch_a"
apply_one "$root/source-full" "$patch_b"
apply_one "$root/source-full" "$patch_c_header"
apply_one "$root/source-full" "$patch_c_library"

apply_one "$root/source-bc" "$patch_b"
apply_one "$root/source-bc" "$patch_c_header"
apply_one "$root/source-bc" "$patch_c_library"

apply_one "$root/source-c-only" "$patch_c_header"
apply_one "$root/source-c-only" "$patch_c_library"

files=(
  libcxxabi/include/cxxabi.h
  libcxxabi/src/cxa_exception.cpp
  libcxxabi/src/cxa_personality.cpp
  libcxx/include/future
  libcxx/include/istream
  libcxx/include/string
  libcxx/include/__ostream/basic_ostream.h
  libcxx/src/ios.cpp
)

printf 'PATCH_SHA256_BEGIN\n'
sha256sum "$patch_a" "$patch_b" "$patch_c_header" "$patch_c_library"
printf 'PATCH_SHA256_END\n'
printf 'BASELINE_FILE_SHA256_BEGIN\n'
for file in "${files[@]}"; do sha256sum "$root/source-base/$file"; done
printf 'BASELINE_FILE_SHA256_END\n'
printf 'FINAL_FILE_SHA256_BEGIN\n'
for file in "${files[@]}"; do sha256sum "$root/source-full/$file"; done
printf 'FINAL_FILE_SHA256_END\n'

printf 'SOURCE_MARKERS_BEGIN\n'
rg -n '__forced_unwind|_Unwind_Resume_or_Rethrow|__set_exception_ready|__set_value_ready' \
  "$root/source-full/libcxxabi/include/cxxabi.h" \
  "$root/source-full/libcxxabi/src/cxa_exception.cpp" \
  "$root/source-full/libcxxabi/src/cxa_personality.cpp" \
  "$root/source-full/libcxx/include/future" \
  "$root/source-full/libcxx/include/istream" \
  "$root/source-full/libcxx/include/string" \
  "$root/source-full/libcxx/include/__ostream/basic_ostream.h" \
  "$root/source-full/libcxx/src/ios.cpp"
printf 'SOURCE_MARKERS_END\n'

expected_future=80a837cdda19397af933d28f1ed611e9bf27504df2b17fd645f685bef9ac6a9d
expected_istream=4111af1754590455948aaa3e14a90a742445e852485322771bb923c80281bd7c
expected_string=a9d0adfef9ba6f86cb168b64b1500ad8fc4a6b97e4c54aef5e51175733bb6935
expected_ostream=e9dbe41f142f1bbf2925546cd2d7dbe73c1b1863ae41f7f94387d51c7a18ded6
expected_ios=6734c4d2fbe5b9708c318a083e41f74633010a17a850c411dacd126e0b6964a4
for pair in \
  "libcxx/include/future:$expected_future" \
  "libcxx/include/istream:$expected_istream" \
  "libcxx/include/string:$expected_string" \
  "libcxx/include/__ostream/basic_ostream.h:$expected_ostream" \
  "libcxx/src/ios.cpp:$expected_ios"; do
  file=${pair%%:*}
  expected=${pair#*:}
  actual=$(sha256sum "$root/source-full/$file" | awk '{print $1}')
  printf 'EXPECTED_SHA file=%s expected=%s actual=%s match=%s\n' \
    "$file" "$expected" "$actual" "$([[ $actual == "$expected" ]] && echo yes || echo no)"
  [[ $actual == "$expected" ]]
done

