#!/usr/bin/env bash
set -euo pipefail
root=/home/toolchain/development/libc++_replacement
base="$root/docs/progress/R59"
out="$base/INDEX.tsv"
tmp="$base/.INDEX.tsv.tmp"
printf 'path\tsha256\tsize_bytes\n' > "$tmp"
while IFS= read -r file; do
  rel=${file#"$root/"}
  digest=$(sha256sum "$file" | awk '{print $1}')
  size=$(stat -c '%s' "$file")
  printf '%s\t%s\t%s\n' "$rel" "$digest" "$size" >> "$tmp"
done < <(find "$base" -type f ! -name INDEX.tsv ! -name .INDEX.tsv.tmp | sort)
mv "$tmp" "$out"
