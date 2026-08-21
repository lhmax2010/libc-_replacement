#!/usr/bin/env bash
set -euo pipefail
root=/home/toolchain/development/libc++_replacement
base="$root/docs/progress/R59/code"
out="$base/INDEX.tsv"
tmp="$base/.INDEX.tsv.tmp"
printf 'source_task\toriginal_path\tupload_path\tsha256\n' > "$tmp"
while IFS= read -r uploaded; do
  rel=${uploaded#"$root/"}
  case "$rel" in
    docs/progress/R59/code/src/*) original="progress/R59/src/${rel##*/}" ;;
    docs/progress/R59/code/tools/*) original="progress/R59/tools/${rel##*/}" ;;
    docs/progress/R59/code/build/*) original="progress/R59/raw/build/${rel##*/}" ;;
    docs/progress/R59/code/output/matrix/*) original="progress/R59/raw/matrix/${rel##*/}" ;;
    docs/progress/R59/code/output/identity/*) original="progress/R59/raw/identity/${rel##*/}" ;;
    docs/progress/R59/code/output/m2_gdb_trace.log) original="progress/R59/raw/m2_gdb_trace.log" ;;
    docs/progress/R59/code/commands/*) original="progress/R59/commands/${rel##*/}" ;;
    *) original=NOT_AVAILABLE ;;
  esac
  digest=$(sha256sum "$uploaded" | awk '{print $1}')
  printf 'R59\t%s\t%s\t%s\n' "$original" "$rel" "$digest" >> "$tmp"
done < <(find "$base" -type f ! -name INDEX.tsv ! -name .INDEX.tsv.tmp | sort)
mv "$tmp" "$out"
