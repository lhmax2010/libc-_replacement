#!/usr/bin/env bash
set -u
set -o pipefail

repo_root=${1:?repository root required}
output_dir=${2:?output directory required}
mkdir -p "$output_dir"

files=(
  "$repo_root/codes/llvm/libcxx/include/__ostream/basic_ostream.h"
  "$repo_root/codes/llvm/libcxx/include/__ostream/print.h"
  "$repo_root/codes/llvm/libcxx/include/__ostream/put_character_sequence.h"
  "$repo_root/codes/llvm/libcxx/include/future"
  "$repo_root/codes/llvm/libcxx/include/iomanip"
  "$repo_root/codes/llvm/libcxx/include/istream"
  "$repo_root/codes/llvm/libcxx/include/string"
)

all_table="$output_dir/libcxx_catch_points_50.tsv"
aggregated_table="$output_dir/aggregated_16.tsv"
unaggregated_table="$output_dir/unaggregated_34.tsv"

printf 'file\tcatch_line\tcatch_source\tclassification\thelper\thelper_line\tbody_excerpt\n' >"$all_table"

rg -n 'catch[[:space:]]*\([[:space:]]*\.\.\.[[:space:]]*\)' "${files[@]}" |
  while IFS=: read -r path line source; do
    end_line=$((line + 5))
    body=$(sed -n "${line},${end_line}p" "$path" | tr '\n' ' ' | sed 's/[[:space:]]\+/ /g; s/[[:space:]]$//')
    helper_hit=$(sed -n "$((line + 1)),$((line + 3))p" "$path" |
      rg -n '__set_(badbit|failbit)_and_consider_rethrow' | head -1 || true)
    if [[ -n $helper_hit ]]; then
      helper_relative_line=${helper_hit%%:*}
      helper_source=${helper_hit#*:}
      helper_line=$((line + helper_relative_line))
      helper=$(sed 's/^[[:space:]]*//; s/[[:space:]]*$//' <<<"$helper_source")
      classification='AGGREGATED_OUT_OF_LINE'
    else
      helper='-'
      helper_line='-'
      classification='HEADER_LOCAL'
    fi
    relative_path=${path#"$repo_root/"}
    clean_source=$(sed 's/^[[:space:]]*//; s/[[:space:]]*$//' <<<"$source")
    printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
      "$relative_path" "$line" "$clean_source" "$classification" "$helper" "$helper_line" "$body"
  done >>"$all_table"

{
  head -1 "$all_table"
  awk -F '\t' 'NR > 1 && $4 == "AGGREGATED_OUT_OF_LINE"' "$all_table"
} >"$aggregated_table"

{
  head -1 "$all_table"
  awk -F '\t' 'NR > 1 && $4 == "HEADER_LOCAL"' "$all_table"
} >"$unaggregated_table"
