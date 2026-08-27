#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
upload_root="$workspace/docs/progress/R68"
index="$upload_root/INDEX.tsv"
temporary=$(mktemp)
trap 'rm -f "$temporary"' EXIT

printf 'source\tupload_path\tsize_bytes\tsha256\n' >"$temporary"

while IFS= read -r -d '' path; do
  relative=${path#"$upload_root/"}
  if [[ "$relative" == "INDEX.tsv" ]]; then
    continue
  fi
  if [[ "$relative" == code/* ]]; then
    source_path="progress/R68/tools/${relative#code/}"
  else
    source_path="progress/R68/$relative"
  fi
  size=$(stat -c '%s' "$path")
  digest=$(sha256sum "$path")
  digest=${digest%% *}
  printf '%s\t%s\t%s\t%s\n' \
    "$source_path" "docs/progress/R68/$relative" "$size" "$digest" \
    >>"$temporary"
done < <(find "$upload_root" -type f -print0 | sort -z)

mv "$temporary" "$index"
trap - EXIT
