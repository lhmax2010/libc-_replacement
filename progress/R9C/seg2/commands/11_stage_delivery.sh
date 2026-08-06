#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
LEDGER="$OUT/commands/11_stage_delivery.log"
: >"$LEDGER"

record() {
  local label=$1
  shift
  {
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >>"$LEDGER" 2>&1
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

cd "$REPO" || exit 125
record add_provenance git add -- packaging/SOURCE_PROVENANCE || exit 90
record write_publish_paths bash -o pipefail -c 'cut -f3 "$1" >"$2"' _ "$OUT/evidence_manifest.tsv" "$OUT/publish_paths.txt" || exit 91
record add_curated_manifest_paths git add -f --pathspec-from-file="$OUT/publish_paths.txt" || exit 92
record add_manifest_and_final_audit git add -f -- \
  "$OUT/evidence_manifest.tsv" "$OUT/final_audit.status" \
  "$OUT/publish_paths.txt" \
  "$OUT/commands/10_final_audit_and_manifest.log" \
  "$OUT/commands/10_final_audit_and_manifest.sh" \
  "$OUT/commands/11_stage_delivery.attempt1.log" \
  "$OUT/commands/11_stage_delivery.log" \
  "$OUT/commands/11_stage_delivery.sh" || exit 93
record assert_user_file_not_staged bash -o pipefail -c \
  '! git diff --cached --name-only | rg -q "^gbs_llvm.conf$"' || exit 94
record assert_no_large_staged_file bash -o pipefail -c \
  'while IFS= read -r f; do test -f "$f" || continue; test "$(stat -c %s "$f")" -lt 100000000 || { printf "oversize: %s\n" "$f"; exit 1; }; done < <(git diff --cached --name-only)' || exit 95
record authored_diff_check git diff --cached --check -- \
  packaging/SOURCE_PROVENANCE \
  progress/R9C/seg2/R9C_segment2_report.md \
  'progress/R9C/seg2/commands/*.sh' \
  'progress/R9C/seg2/probes/*.c' || exit 96
record staged_name_status git diff --cached --name-status || exit 97
record staged_stat git diff --cached --stat || exit 98
printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
exit 0
