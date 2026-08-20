#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
root="$workspace/progress/R53/code"
index="$root/INDEX.tsv"
printf 'upload_path\toriginal_path\tsha256\n' > "$index"
while IFS= read -r file; do
  rel=${file#"$root/"}
  case "$rel" in
    libcxx/*) original="tmp/R53/source-layer-c-full/$rel" ;;
    tests/*) original="progress/R53/src/${rel#tests/}" ;;
    tools/*) original="progress/R53/tools/${rel#tools/}" ;;
    patches/*) original="progress/R53/patches/${rel#patches/}" ;;
    evidence/*)
      name=${rel#evidence/}
      if [[ -e "$workspace/progress/R53/raw/$name" ]]; then
        original="progress/R53/raw/$name"
      else
        original="progress/R53/commands/$name"
      fi
      ;;
    *) original=NOT_AVAILABLE ;;
  esac
  hash=$(sha256sum "$file" | awk '{print $1}')
  printf 'docs/progress/R53/code/%s\t%s\t%s\n' "$rel" "$original" "$hash" >> "$index"
done < <(find "$root" -type f ! -name INDEX.tsv | sort)
