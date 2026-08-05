#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
LEDGER="$OUT/commands/06a_final_evidence_audit_retry.log"

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
record_to retry_final_report_assertion "$RAW/retry_final_report_assertion.stdout.txt" bash -o pipefail -c \
  'rg -q "RED_WITH_AARCH64_NOT_AVAILABLE" "$1" && rg -q "PENDING_BOARD_VERIFICATION" "$1" && rg -q "RED_UNEXPECTED_R6_GIT_EXPORT_DELTA" "$1" && rg -q "EXPECTED_BACKPORT_DELTA" "$1" && rg -q "未进入晋级台账" "$1"' \
  _ "$OUT/T4_report.md" || overall=1
record_to retry_assert_previous_successes "$RAW/retry_assert_previous_successes.stdout.txt" bash -o pipefail -c \
  'test "$(rg -c "^EXIT_CODE=0$" "$1")" -eq 5 && test "$(rg -c "^EXIT_CODE=1$" "$1")" -eq 1' \
  _ "$OUT/commands/06_final_evidence_audit.log" || overall=1
record_to retry_manifest_without_self_and_live_ledger "$OUT/MANIFEST.sha256" bash -o pipefail -c \
  'find "$1" -type f ! -path "$1/MANIFEST.sha256" ! -path "$1/commands/06a_final_evidence_audit_retry.log" ! -path "$1/manifest_verify.log" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' \
  _ "$OUT" || overall=1
record_to retry_manifest_verify "$OUT/manifest_verify.log" bash -o pipefail -c \
  'sha256sum -c "$1"' _ "$OUT/MANIFEST.sha256" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
