#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"
code=docs/progress/R54/code
review=docs/progress/R54/review_package
test ! -e docs/progress/R54
mkdir -p "$code"/{patches,tests,tools,full_files,config,commands,identity} \
  "$review"/{patches,full_files,comparison,tests/current,matrices/R51,matrices/R52,matrices/R53,matrices/R54,raw,commands}

cp progress/R51/patches/0001-Layer-A-*.patch "$code/patches/"
cp progress/R51/patches/0002-Layer-B-*.patch "$code/patches/"
cp progress/R52/patches/0001-libcxx-preserve-forced-unwind-across-header-catch-al.patch "$code/patches/"
cp progress/R53/patches/0001-libcxx-preserve-forced-unwind-in-library-catch-sites.patch "$code/patches/"
cp progress/R54/src/*.cpp "$code/tests/"
cp progress/R54/tools/*.sh "$code/tools/"
cp tmp/R54/build-all-x86_64/CMakeCache.txt "$code/config/CMakeCache.x86_64.txt"
cp tmp/R54/build-all-armv7l/CMakeCache.txt "$code/config/CMakeCache.armv7l.txt"
cp progress/R54/commands/* "$code/commands/"
cp progress/R54/raw/x86_64/runtime_identity.log "$code/identity/"
cp progress/R54/raw/armv7l/identity_and_noncancel.log "$code/identity/board_identity_initial.log"

for file in \
  libcxxabi/include/cxxabi.h \
  libcxxabi/src/cxa_exception.cpp \
  libcxxabi/src/cxa_personality.cpp \
  libcxx/include/future \
  libcxx/include/istream \
  libcxx/include/string \
  libcxx/include/__ostream/basic_ostream.h \
  libcxx/src/ios.cpp; do
  mkdir -p "$code/full_files/$(dirname "$file")"
  cp "tmp/R54/source-all/$file" "$code/full_files/$file"
done

cp -a "$code/patches/." "$review/patches/"
cp -a "$code/full_files/." "$review/full_files/"
cp -a "$code/tests/." "$review/tests/current/"
cp -a progress/R51/src "$review/tests/R51"
cp -a progress/R52/src "$review/tests/R52"
cp -a progress/R53/src "$review/tests/R53"
cp -a progress/R54/commands/. "$review/commands/"
cp -a progress/R51/raw "$review/raw/R51"
cp -a progress/R52/raw "$review/raw/R52"
cp -a progress/R53/raw "$review/raw/R53"
cp -a progress/R54/raw "$review/raw/R54"
cp progress/R51/tables/*.tsv "$review/matrices/R51/"
cp progress/R52/tables/*.tsv "$review/matrices/R52/"
cp progress/R53/tables/*.tsv "$review/matrices/R53/"
cp progress/R54/tables/*.tsv "$review/matrices/R54/"
cp progress/R54/REPORT.md progress/R54/NOT_AVAILABLE.md progress/R54/RESULT.txt "$review/"

{
  printf 'relative_path\tbaseline_sha256\tcurrent_sha256\n'
  for file in \
    libcxxabi/include/cxxabi.h \
    libcxxabi/src/cxa_exception.cpp \
    libcxxabi/src/cxa_personality.cpp \
    libcxx/include/future \
    libcxx/include/istream \
    libcxx/include/string \
    libcxx/include/__ostream/basic_ostream.h \
    libcxx/src/ios.cpp; do
    before=$(sha256sum "codes/llvm/$file" | awk '{print $1}')
    after=$(sha256sum "tmp/R54/source-all/$file" | awk '{print $1}')
    printf '%s\t%s\t%s\n' "$file" "$before" "$after"
  done
} > "$review/BASELINE_SHA256.tsv"

{
  printf 'UPSTREAM_BASELINE_COMMIT\t%s\n' "$(git -C codes/llvm rev-parse HEAD)"
  printf 'PATCH_ORDER\tLayer-A;Layer-B;Layer-C-header;Layer-C-library\n'
  sha256sum "$review/patches"/*.patch
} > "$review/BASELINE_AND_PATCHES.txt"

{
  printf '=== libstdc++ forced-unwind definitions and guards ===\n'
  rg -n -C 4 '__forced_unwind|found_type == found_cleanup|_Unwind_Resume_or_Rethrow' \
    codes/gcc/libstdc++-v3/libsupc++ codes/gcc/libstdc++-v3/include \
    codes/gcc/libstdc++-v3/src || true
  printf '\n=== patched libc++abi/libc++ corresponding code ===\n'
  rg -n -C 4 '__forced_unwind|ttypeIndex != 0|_Unwind_Resume_or_Rethrow' \
    tmp/R54/source-all/libcxxabi/include tmp/R54/source-all/libcxxabi/src \
    tmp/R54/source-all/libcxx/include tmp/R54/source-all/libcxx/src || true
} > "$review/comparison/libstdcxx_libcxx_side_by_side.txt"
cp progress/R52/tables/header_sites_16.tsv "$review/comparison/"
cp progress/R53/tables/library_sites_34.tsv "$review/comparison/"

cp progress/R54/NOT_AVAILABLE.md "$review/KNOWN_GAPS.md"
printf 'All source, command and output paths are relative to the repository root.\n' > "$review/README.md"

find "$code" -type f -print0 | sort -z | xargs -0 sha256sum | sed "s|  $code/|\t|" > "$code/INDEX.tsv"
find "$review" -type f ! -name INDEX.tsv -print0 | sort -z | xargs -0 sha256sum | sed "s|  $review/|\t|" > "$review/INDEX.tsv"

find docs/progress/R54 -type f -size +5242880 -printf '%s\t%p\n' > progress/R54/EXCLUDED.tsv
