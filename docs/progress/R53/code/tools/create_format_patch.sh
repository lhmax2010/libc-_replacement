#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
repo="$workspace/tmp/R53/patch-repo"
baseline="$workspace/tmp/R52/source-layer-c"
modified="$workspace/tmp/R53/source-layer-c-full"
message="$workspace/progress/R53/patches/COMMIT_MESSAGE.txt"
output="$workspace/progress/R53/patches/0001-libcxx-preserve-forced-unwind-in-library-catch-sites.patch"
test ! -e "$repo"
mkdir -p "$repo/libcxx/include/__ostream" "$repo/libcxx/src"
cp "$baseline/libcxx/include/istream" "$repo/libcxx/include/istream"
cp "$baseline/libcxx/include/__ostream/basic_ostream.h" "$repo/libcxx/include/__ostream/basic_ostream.h"
cp "$baseline/libcxx/src/ios.cpp" "$repo/libcxx/src/ios.cpp"
git -C "$repo" init
git -C "$repo" config user.name "He Fangyu"
git -C "$repo" config user.email "fangyu.he@samsung.com"
git -C "$repo" add libcxx
git -C "$repo" commit -m 'baseline for library catch-site patch'
cp "$modified/libcxx/include/istream" "$repo/libcxx/include/istream"
cp "$modified/libcxx/include/__ostream/basic_ostream.h" "$repo/libcxx/include/__ostream/basic_ostream.h"
cp "$modified/libcxx/src/ios.cpp" "$repo/libcxx/src/ios.cpp"
git -C "$repo" add libcxx
git -C "$repo" commit -F "$message"
git -C "$repo" format-patch -1 --stdout > "$output"
git -C "$repo" show --stat --oneline --decorate HEAD
sha256sum "$output"
