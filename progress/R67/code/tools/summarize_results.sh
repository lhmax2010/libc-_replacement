#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
tables="$workspace/progress/R67/tables"
mkdir -p "$tables"

normalize() {
  local arch=$1 file=$2
  awk -F '\t' -v arch="$arch" 'BEGIN {OFS="\t"} NR > 1 {
    if (NF == 3) print arch, $1, $2, "host", $3;
    else print arch, $1, $2, $3, $4;
  }' "$file"
}

{
  printf 'architecture\tgroup\tcase\tvariant\texit_code\n'
  normalize x86_64 "$workspace/progress/R67/raw/x86_64/056_x86_matrix_final.tsv"
  normalize armv7l "$workspace/progress/R67/raw/armv7l/061_board_matrix_final.tsv"
} > "$tables/all_matrix_cells.tsv"

awk -F '\t' 'BEGIN {OFS="\t"; print "architecture","group","variant","exit_code","count"}
  NR > 1 {variant=$4; if ($3 ~ /^fixed_/) variant="fixed"; else if ($3 ~ /^reference_/) variant="reference"; else if ($3 ~ /^baseline_/) variant="baseline"; n[$1 FS $2 FS variant FS $5]++}
  END {for (k in n) print k, n[k]}' "$tables/all_matrix_cells.tsv" | sort > "$tables/group_exit_counts.tsv.tmp"
{
  printf 'architecture\tgroup\tvariant\texit_code\tcount\n'
  tail -n +2 "$tables/group_exit_counts.tsv.tmp"
} > "$tables/group_exit_counts.tsv"

awk -F '\t' 'BEGIN {OFS="\t"; print "architecture","group","case","variant","exit_code"}
  NR > 1 && ($2 == "directed" || $2 == "m1_reference" || $2 == "facility" || $2 == "regression" || $2 == "normal" || $2 == "async_noncancel") {print}' \
  "$tables/all_matrix_cells.tsv" > "$tables/directed_facility_regression.tsv"

awk -F '\t' 'BEGIN {OFS="\t"; print "architecture","group","variant","exit_code","count"}
  NR > 1 && ($2 == "m2_lock" || $2 == "m1_concurrency" || $2 == "async" || $2 == "concurrency") {
    variant=($3 ~ /^fixed_/) ? "fixed" : (($3 ~ /^reference_/) ? "reference" : "baseline");
    key=$1 FS $2 FS variant FS $5; n[key]++
  }
  END {for (key in n) print key, n[key]}' "$tables/all_matrix_cells.tsv" | sort > "$tables/repeated_counts.tsv.tmp"
{
  printf 'architecture\tgroup\tvariant\texit_code\tcount\n'
  tail -n +2 "$tables/repeated_counts.tsv.tmp"
} > "$tables/repeated_counts.tsv"

{
  printf 'architecture\truntime\ttest\texit_code\n'
  awk -F '\t' 'BEGIN {OFS="\t"} NR > 1 {print "x86_64",$1,$2,$3}' "$workspace/progress/R67/raw/x86_64/057b_upstream_x86_final.tsv"
  awk -F '\t' 'BEGIN {OFS="\t"} NR > 1 && $1 == "upstream" {name=$2; runtime=name; sub(/_.*/,"",runtime); test=name; sub(/^[^_]*_/,"",test); print "armv7l",runtime,test,$4}' "$workspace/progress/R67/raw/armv7l/061_board_matrix_final.tsv"
} > "$tables/upstream_results.tsv"

printf 'source\tsha256\n' > "$tables/patch_sha256.tsv"
for patch in "$workspace"/progress/R67/code/patches/*.patch; do
  printf '%s\t%s\n' "$(basename "$patch")" "$(sha256sum "$patch" | awk '{print $1}')" >> "$tables/patch_sha256.tsv"
done

rm "$tables/group_exit_counts.tsv.tmp" "$tables/repeated_counts.tsv.tmp"
