#!/bin/bash
set -euo pipefail

output=progress/R38/tables/material_index.tsv
printf 'relative_path\tsize_bytes\tsha256\n' > "$output"
find progress/R38/code progress/R38/tools -type f ! -path "$output" -print0 |
  sort -z |
  while IFS= read -r -d '' file; do
    relative=${file#progress/R38/}
    size=$(stat -c %s "$file")
    sha=$(sha256sum "$file" | cut -d' ' -f1)
    printf '%s\t%s\t%s\n' "$relative" "$size" "$sha"
  done >> "$output"
