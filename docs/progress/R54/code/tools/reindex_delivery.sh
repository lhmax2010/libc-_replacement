#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"
root=docs/progress/R54
mkdir -p "$root/tables"
cp progress/R54/REPORT.md progress/R54/NOT_AVAILABLE.md progress/R54/RESULT.txt "$root/"
cp progress/R54/tables/*.tsv "$root/tables/"
cp "$root/review_package/BASELINE_SHA256.tsv" "$root/code/BASELINE_SHA256.tsv"
cp "$root/review_package/BASELINE_AND_PATCHES.txt" "$root/code/BASELINE_AND_PATCHES.txt"

make_index() {
  local base=$1 output=$2 path rel task source sha
  printf 'sha256\tupload_path\tsource_task\tsource_path\n' > "$output"
  while IFS= read -r -d '' path; do
    [[ $path == "$output" ]] && continue
    rel=${path#"$base/"}
    task=R54
    source="progress/R54/$rel"
    case "$rel" in
      patches/0001-Layer-A-*|patches/0002-Layer-B-*) task=R51; source="progress/R51/patches/${rel#patches/}" ;;
      patches/*header*) task=R52; source="progress/R52/patches/${rel#patches/}" ;;
      patches/*library*) task=R53; source="progress/R53/patches/${rel#patches/}" ;;
      tests/R51/*) task=R51; source="progress/R51/src/${rel#tests/R51/}" ;;
      tests/R52/*) task=R52; source="progress/R52/src/${rel#tests/R52/}" ;;
      tests/R53/*) task=R53; source="progress/R53/src/${rel#tests/R53/}" ;;
      tests/current/*|tests/*) task=R54; source="progress/R54/src/${rel##*/}" ;;
      raw/R51/*) task=R51; source="progress/R51/raw/${rel#raw/R51/}" ;;
      raw/R52/*) task=R52; source="progress/R52/raw/${rel#raw/R52/}" ;;
      raw/R53/*) task=R53; source="progress/R53/raw/${rel#raw/R53/}" ;;
      raw/R54/*) task=R54; source="progress/R54/raw/${rel#raw/R54/}" ;;
      matrices/R51/*) task=R51; source="progress/R51/tables/${rel#matrices/R51/}" ;;
      matrices/R52/*) task=R52; source="progress/R52/tables/${rel#matrices/R52/}" ;;
      matrices/R53/*) task=R53; source="progress/R53/tables/${rel#matrices/R53/}" ;;
      matrices/R54/*) task=R54; source="progress/R54/tables/${rel#matrices/R54/}" ;;
      full_files/*) task=R54; source="tmp/R54/source-all/${rel#full_files/}" ;;
      config/CMakeCache.x86_64.txt) source="tmp/R54/build-all-x86_64/CMakeCache.txt" ;;
      config/CMakeCache.armv7l.txt) source="tmp/R54/build-all-armv7l/CMakeCache.txt" ;;
      commands/*) source="progress/R54/commands/${rel#commands/}" ;;
      tools/*) source="progress/R54/tools/${rel#tools/}" ;;
      identity/*) source="progress/R54/raw/runtime-identity-curation" ;;
      comparison/*) source="R51/R52/R53/R54 referenced source capture" ;;
      BASELINE*) source="generated from codes/llvm and four patch files" ;;
    esac
    sha=$(sha256sum "$path" | awk '{print $1}')
    printf '%s\t%s\t%s\t%s\n' "$sha" "$path" "$task" "$source" >> "$output"
  done < <(find "$base" -type f ! -name INDEX.tsv -print0 | sort -z)
}
make_index "$root/code" "$root/code/INDEX.tsv"
make_index "$root/review_package" "$root/review_package/INDEX.tsv"
find "$root" -type f -size +5242880 -printf '%s\t%p\n' > progress/R54/EXCLUDED.tsv
