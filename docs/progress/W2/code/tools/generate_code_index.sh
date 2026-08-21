#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
root="$workspace/docs/progress/W2/code"
output="$root/INDEX.tsv"

source_for() {
  case "$1" in
    tests/r61_probe.cpp) echo progress/W1/src/r61_probe.cpp ;;
    tests/r61_m2_matrix.cpp) echo progress/W1/src/r61_m2_matrix.cpp ;;
    tests/m1_concurrency_probe.cpp) echo progress/R60/src/m1_concurrency_probe.cpp ;;
    tests/facility_probe.cpp) echo progress/R54/src/facility_probe.cpp ;;
    tests/async_probe.cpp) echo progress/R55/src/async_probe.cpp ;;
    tests/noncancel_probe.cpp) echo progress/R52/src/noncancel_probe.cpp ;;
    tests/normal_paths_probe.cpp) echo progress/R58/src/normal_paths_probe.cpp ;;
    patches/0001-Layer-A-*) echo progress/R51/patches/0001-Layer-A-libc-abi-preserve-foreign-forced-unwind-stat.patch ;;
    patches/0002-Layer-B-*) echo progress/R51/patches/0002-Layer-B-libc-abi-expose-GNU-forced-unwind-marker-typ.patch ;;
    patches/0001-Layer-C-*) echo progress/W1/patches/0001-Layer-C-header-preserve-forced-unwind-and-async-publication.patch ;;
    patches/0001-libcxx-*) echo progress/R53/patches/0001-libcxx-preserve-forced-unwind-in-library-catch-sites.patch ;;
    current/*) echo "tmp/W1/patch-repo/${1#current/}" ;;
    config/CMakeCache.txt) echo tmp/W2/build-armv7l/CMakeCache.txt ;;
    tools/*) echo "progress/W2/$1" ;;
    *) echo NOT_AVAILABLE ;;
  esac
}

printf 'upload_path\tsource\tsha256\n' > "$output"
while IFS= read -r file; do
  relative=${file#"$root"/}
  sha=$(sha256sum "$file" | awk '{print $1}')
  printf '%s\t%s\t%s\n' "$relative" "$(source_for "$relative")" "$sha" >> "$output"
done < <(find "$root" -type f ! -name INDEX.tsv -print | sort)
