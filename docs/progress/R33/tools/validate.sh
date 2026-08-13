#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
root=$workspace/progress/R33
test "$(tail -n +2 "$root/tables/s4_classification.tsv" | wc -l)" -eq 50
test "$(tail -n +2 "$root/tables/s4_classification.tsv" | cut -f1 | sort -u | wc -l)" -eq 10
test "$(tail -n +2 "$root/tables/s4_classification.tsv" | cut -f2 | sort -u | wc -l)" -eq 8
test "$(awk -F '\t' 'NR>1&&$10=="NO_BCD_OPTION_CONFIRMED"{n++} END{print n+0}' "$root/tables/s4_classification.tsv")" -eq 50
test "$(awk -F '\t' 'NR>1&&$3==0{n++} END{print n+0}' "$root/tables/s1_matrix_r3.tsv")" -eq 4
test "$(awk -F '\t' 'NR>1&&$1=="libcxx"&&$3==134{n++} END{print n+0}' "$root/tables/s1_matrix_r3.tsv")" -eq 2
test "$(awk -F '\t' 'NR>1&&$3==0{n++} END{print n+0}' "$root/tables/s2_matrix.tsv")" -eq 4
grep -q 'foreign:caught:type=NULL:exception_ptr=EMPTY' "$root/results/s2_matrix/libcxx_foreign.log"
grep -q 'foreign:caught:type=NULL:exception_ptr=EMPTY' "$root/results/s2_matrix/libstdcxx_foreign.log"
grep -q 'cancel:forced-handler' "$root/results/s1_matrix_r3/libcxx_cancel.log"
grep -q 'EXIT_CODE=134' "$root/results/s1_matrix_r3/libcxx_cancel.log"
grep -q 'RED_STOP_RESOURCE' "$root/commands/037_resource_final.log"
test ! -e "$workspace/codes/llvm/.git/rebase-apply"
test -z "$(git -C "$workspace/codes/llvm" status --porcelain)"
find "$root" -type f -size +5M -print -quit | grep -q '^$' || test -z "$(find "$root" -type f -size +5M -print -quit)"
printf 'VALIDATION_PASS\n'
