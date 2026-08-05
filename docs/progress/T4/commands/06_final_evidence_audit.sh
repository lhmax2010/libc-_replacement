#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
LEDGER="$OUT/commands/06_final_evidence_audit.log"

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
record_to final_assert_statuses "$RAW/final_assert_statuses.stdout.txt" bash -o pipefail -c \
  'rg -q "PREFLIGHT=PASS" "$1" && rg -q "ARTIFACT_IDENTITY=PASS" "$2" && rg -q "STATIC_GATES=PASS" "$3" && rg -q "RUNTIME_FIXTURE_CARRIERS=PASS" "$4" && rg -q "EXPLICIT_LDD_AND_PROVIDER=PASS" "$5" && rg -q "T4_AGGREGATION=PASS" "$6"' \
  _ "$OUT/inventory_preflight.status" "$OUT/artifact_identity.status" "$OUT/static_gate.status" \
  "$OUT/runtime_carrier.status" "$OUT/explicit_ldd_provider.status" "$OUT/aggregation.status" || overall=1
record_to final_assert_report_conclusions "$RAW/final_assert_report_conclusions.stdout.txt" bash -o pipefail -c \
  'rg -q "RED_WITH_AARCH64_NOT_AVAILABLE" "$1" && rg -q "PENDING_BOARD_VERIFICATION" "$1" && rg -q "RED_UNEXPECTED_R6_GIT_EXPORT_DELTA" "$1" && rg -q "EXPECTED_BACKPORT_DELTA" "$1" && rg -q "不进入晋级台账" "$1"' \
  _ "$OUT/T4_report.md" || overall=1
record_to final_assert_gate_accounting "$RAW/final_assert_gate_accounting.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 14 && test "$(tail -n +2 "$2" | wc -l)" -eq 14 && test "$(tail -n +2 "$3" | wc -l)" -eq 3 && test "$(tail -n +2 "$4" | wc -l)" -eq 24' \
  _ "$OUT/gate_inventory.tsv" "$OUT/gate_results.tsv" "$OUT/architecture_summary.tsv" "$OUT/artifact_identity.tsv" || overall=1
record_to final_assert_originals_unmodified "$RAW/final_assert_originals_unmodified.stdout.txt" bash -o pipefail -c \
  'test "$(awk -F"\t" "NR>1 && \$5!=\"MATCH\" {n++} END {print n+0}" "$1")" -eq 0 && test "$(awk -F"\t" "NR>1 && \$5!=\"MATCH\" {n++} END {print n+0}" "$2")" -eq 0' \
  _ "$OUT/artifact_identity.tsv" "$OUT/static_tool_adapter.tsv" || overall=1
record_to final_manifest_without_self_and_live_ledger "$OUT/MANIFEST.sha256" bash -o pipefail -c \
  'find "$1" -type f ! -path "$1/MANIFEST.sha256" ! -path "$1/commands/06_final_evidence_audit.log" ! -path "$1/manifest_verify.log" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' \
  _ "$OUT" || overall=1
record_to final_manifest_verify "$OUT/manifest_verify.log" bash -o pipefail -c \
  'sha256sum -c "$1"' _ "$OUT/MANIFEST.sha256" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
