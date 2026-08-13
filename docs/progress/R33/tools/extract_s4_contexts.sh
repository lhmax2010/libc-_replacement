#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
input=$workspace/progress/R32/tables/impact_category_locations.tsv
table=$workspace/progress/R33/tables/s4_guard_locations.tsv
contexts=$workspace/progress/R33/results/s4_contexts_30_lines.txt
awk -F '\t' 'BEGIN{OFS="\t"} NR==1 || $5=="forced_unwind_confirmed_rethrow"' "$input" > "$table"
: > "$contexts"
tail -n +2 "$table" | while IFS=$'\t' read -r category source_rpm package_name stage feature file line scope macro_name source_text; do
  source_file="$workspace/tmp/corpus/extracted/$source_rpm/$file"
  start=$((line > 30 ? line - 30 : 1))
  end=$((line + 30))
  {
    printf '===== %s | %s | %s:%s | %s =====\n' "$source_rpm" "$package_name" "$file" "$line" "$scope"
    if test -f "$source_file"; then
      nl -ba "$source_file" | sed -n "${start},${end}p"
    else
      printf 'NOT_FOUND: %s\n' "$source_file"
    fi
  } >> "$contexts"
done
printf 'locations='; tail -n +2 "$table" | wc -l
printf 'source_rpm_identities='; tail -n +2 "$table" | cut -f2 | sort -u | wc -l
printf 'package_names='; tail -n +2 "$table" | cut -f3 | sort -u | wc -l
sha256sum "$input" "$table" "$contexts"
