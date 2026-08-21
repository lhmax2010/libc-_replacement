#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
root="$workspace/docs/progress/R56"
index="$root/INDEX.tsv"

test -f "$root/REPORT.md"
test -f "$root/RESULT.txt"
test -f "$root/NOT_OBSERVED.md"
test -f "$root/EXCLUDED.tsv"
test -f "$index"
test "$(cat "$root/RESULT.txt")" = NOT_VIABLE
test "$(wc -l < "$root/EXCLUDED.tsv")" -eq 1
test -z "$(find "$root" -type f -size +5M -print -quit)"

tail -n +2 "$index" | while IFS=$'\t' read -r source upload size expected_sha; do
  file="$workspace/$upload"
  test -f "$file"
  test "$(stat -c '%s' "$file")" = "$size"
  test "$(sha256sum "$file" | awk '{print $1}')" = "$expected_sha"
done

test ! -e "$workspace/tmp/R56/feasibility/forward_libcxx.o"
test ! -e "$workspace/tmp/R56/feasibility/forward_libstdcxx.o"
test -e "$workspace/tmp/R56/feasibility/include_libcxx.o"
test -e "$workspace/tmp/R56/feasibility/include_libstdcxx.o"

printf 'delivery_validation=PASS\n'
printf 'result=NOT_VIABLE\n'
printf 'indexed_files=%d\n' "$(( $(wc -l < "$index") - 1 ))"
printf 'excluded_files=0\n'
printf 'files_over_5MiB=0\n'
