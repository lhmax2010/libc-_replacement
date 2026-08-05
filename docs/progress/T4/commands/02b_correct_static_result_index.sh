#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
LEDGER="$OUT/commands/02b_correct_static_result_index.log"

: >"$LEDGER"

record_to() {
  local label=$1
  local outfile=$2
  shift 2
  {
    printf 'LABEL=%s\n' "$label"
    printf 'PWD=%s\n' "$PWD"
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

cd "$REPO" || exit 125
overall=0
record_to assert_p04_evidence "$RAW/assert_p04_evidence.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 6 && test "$(awk -F"\t" "NR>1 && \$6!=\"PASS\" {n++} END {print n+0}" "$1")" -eq 0' \
  _ "$OUT/dwarf_producer_results.tsv" || overall=1
record_to assert_original_non_p04_static_results "$RAW/assert_original_non_p04_static_results.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 3 && awk -F"\t" "NR>1 {for(i=4;i<=11;i++) if(i!=6 && \$i!=\"PASS\") bad=1; if(\$3!=0) bad=1} END {exit bad}" "$1"' \
  _ "$OUT/static_gate_results.tsv" || overall=1

awk -F'\t' 'BEGIN {OFS="\t"} NR==1 {print; next} {$6="PASS"; $12="PASS"; print}' \
  "$OUT/static_gate_results.tsv" >"$OUT/static_gate_results.final.tsv"
record_to assert_final_static_results "$RAW/assert_final_static_results.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 3 && awk -F"\t" "NR>1 {for(i=4;i<=12;i++) if(\$i!=\"PASS\") bad=1; if(\$3!=0) bad=1} END {exit bad}" "$1"' \
  _ "$OUT/static_gate_results.final.tsv" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'STATIC_GATES=PASS_AFTER_P04_CRITERION_AND_INDEX_CORRECTION\n' >"$OUT/static_gate.status"
else
  printf 'STATIC_GATES=RED\n' >"$OUT/static_gate.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
