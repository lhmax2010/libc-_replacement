#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
source_root="$workspace/progress/R55"
dest="$workspace/docs/progress/R55"

mkdir -p "$dest"
cp -a "$source_root"/. "$dest"/
mkdir -p "$dest/code/modified/libcxx/include" "$dest/code/tests" \
  "$dest/code/config" "$dest/code/toolchains" "$dest/code/patches"
cp "$workspace/tmp/R55/source-fixed/libcxx/include/future" "$dest/code/modified/libcxx/include/future"
cp "$workspace/tmp/R55/source-fixed/libcxx/include/istream" "$dest/code/modified/libcxx/include/istream"
cp "$workspace/tmp/R55/source-fixed/libcxx/include/string" "$dest/code/modified/libcxx/include/string"
cp "$source_root/patches/"* "$dest/code/patches/"
cp "$source_root/src/"*.cpp "$dest/code/tests/"
cp "$workspace/progress/R54/src/facility_probe.cpp" "$dest/code/tests/r54_facility_probe.cpp"
cp "$workspace/progress/R54/src/concurrency_probe.cpp" "$dest/code/tests/r54_concurrency_probe.cpp"
cp "$workspace/progress/R51/code/config/CMakeCache.x86_64.txt" "$dest/code/config/"
cp "$workspace/progress/R51/code/config/CMakeCache.armv7l.txt" "$dest/code/config/"
cp "$workspace/progress/R33/tools/tizen-clang++" "$dest/code/toolchains/"
cp "$workspace/progress/R36/tools/armv7l-clang++" "$dest/code/toolchains/"

index="$dest/code/INDEX.tsv"
printf 'path\tsha256\tsize_bytes\n' > "$index"
while IFS= read -r file; do
  rel=${file#"$dest/code/"}
  sha=$(sha256sum "$file" | awk '{print $1}')
  size=$(stat -c '%s' "$file")
  printf '%s\t%s\t%s\n' "$rel" "$sha" "$size" >> "$index"
done < <(find "$dest/code" -type f ! -name INDEX.tsv | sort)

printf 'path\tsha256\tsize_bytes\n' > "$dest/INDEX.tsv"
while IFS= read -r file; do
  rel=${file#"$dest/"}
  sha=$(sha256sum "$file" | awk '{print $1}')
  size=$(stat -c '%s' "$file")
  printf '%s\t%s\t%s\n' "$rel" "$sha" "$size" >> "$dest/INDEX.tsv"
done < <(find "$dest" -type f ! -path "$dest/INDEX.tsv" | sort)

printf 'path\tsize_bytes\tsha256\treason\n' > "$dest/EXCLUDED.tsv"
if find "$dest" -type f -size +5M -print -quit | grep -q .; then
  printf 'ERROR: curated file exceeds 5 MiB\n' >&2
  exit 80
fi
