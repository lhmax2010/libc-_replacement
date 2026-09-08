#!/bin/bash
set -euo pipefail

root=${1:?usage: validate_status_update.sh PROJECT_ROOT}
base="$root/docs/progress/BUILD_W3_0908"
tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

for lang in zh en; do
  source_tsv="$base/source/base_packages_${lang}.tsv"
  updated_tsv="$base/updated/base_packages_${lang}.tsv"
  updated_md="$base/updated/base_packages_${lang}.md"

  test "$(wc -l < "$source_tsv")" -eq 255
  test "$(wc -l < "$updated_tsv")" -eq 255
  cut -f1-8 "$source_tsv" > "$tmpdir/source_${lang}_first8.tsv"
  cut -f1-8 "$updated_tsv" > "$tmpdir/updated_${lang}_first8.tsv"
  cmp "$tmpdir/source_${lang}_first8.tsv" "$tmpdir/updated_${lang}_first8.tsv"

  changed=$(paste "$source_tsv" "$updated_tsv" | awk -F '\t' '$9 != $18 {n++} END {print n+0}')
  test "$changed" -eq 3
  markdown_rows=$(awk '/^\| / {n++} END {print n-2}' "$updated_md")
  test "$markdown_rows" -eq 254
  printf '%s rows=254 first_eight_columns=IDENTICAL status_changes=%s markdown_rows=%s\n' \
    "$lang" "$changed" "$markdown_rows"
done

awk -F '\t' 'NR>1 {n[$4]++} END {for (k in n) print k "=" n[k]}' \
  "$base/updated/base_packages_zh.tsv" | sort

sha256sum "$base"/source/* "$base"/updated/*
