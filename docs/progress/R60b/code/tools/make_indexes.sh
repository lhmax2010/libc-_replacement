#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
for root in "$workspace/docs/progress/R60" "$workspace/docs/progress/R60b"; do
  output="$root/INDEX.tsv"
  tmp="$output.tmp"
  printf 'path\tsha256\tsize_bytes\n' > "$tmp"
  while IFS= read -r -d '' file; do
    relative=${file#"$root/"}
    hash=$(sha256sum "$file" | awk '{print $1}')
    size=$(stat -c %s "$file")
    printf '%s\t%s\t%s\n' "$relative" "$hash" "$size" >> "$tmp"
  done < <(find "$root" -type f ! -name INDEX.tsv ! -name 'INDEX.tsv.tmp' -print0 | sort -z)
  mv "$tmp" "$output"
done
