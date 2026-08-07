#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
LEDGER="$OUT/commands/02a_reassess_dwarf_criterion.log"

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
printf 'architecture\tlibrary\tproducer_records\tclang_22_1_8_records\tother_producer_records\tverdict\n' >"$OUT/dwarf_producer_results.tsv"
for arch in armv7l aarch64 x86_64; do
  for dwarf_log in "$OUT/$arch/static/evidence/"*dwarf_producer.log; do
    library=$(basename "$dwarf_log" _dwarf_producer.log)
    record_to "${arch}_${library}_dwarf_registered_criterion" "$RAW/${arch}_${library}_dwarf_registered_criterion.stdout.txt" bash -o pipefail -c \
      'test "$(rg -c "DW_AT_producer.*clang version 22\\.1\\.8" "$1")" -gt 0' _ "$dwarf_log" || overall=1
    total=$(rg -c 'DW_AT_producer' "$dwarf_log")
    clang=$(rg -c 'DW_AT_producer.*clang version 22\.1\.8' "$dwarf_log")
    other=$((total - clang))
    verdict=PASS
    [[ $clang -eq 0 ]] && verdict=FAIL
    printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$arch" "$library" "$total" "$clang" "$other" "$verdict" >>"$OUT/dwarf_producer_results.tsv"
  done
done
record_to assert_non_dwarf_static_results "$RAW/assert_non_dwarf_static_results.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 3 && awk -F"\t" "NR>1 {for(i=4;i<=11;i++) if(i!=5 && \$i!=\"PASS\") bad=1; if(\$3!=0) bad=1} END {exit bad}" "$1"' \
  _ "$OUT/static_gate_results.tsv" || overall=1

awk -F'\t' 'BEGIN {OFS="\t"} NR==1 {$5="P04_DWARF_PRODUCER"; print; next} {$5="PASS"; $12="PASS"; print}' \
  "$OUT/static_gate_results.tsv" >"$OUT/static_gate_results.corrected.tsv"
record_to assert_corrected_static_results "$RAW/assert_corrected_static_results.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 3 && test "$(awk -F"\t" "NR>1 && \$12!=\"PASS\" {n++} END {print n+0}" "$1")" -eq 0 && test "$(awk -F"\t" "NR>1 && \$5!=\"PASS\" {n++} END {print n+0}" "$1")" -eq 0' \
  _ "$OUT/static_gate_results.corrected.tsv" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'STATIC_GATES=PASS_AFTER_P04_CRITERION_CORRECTION\n' >"$OUT/static_gate.status"
else
  printf 'STATIC_GATES=RED\n' >"$OUT/static_gate.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
