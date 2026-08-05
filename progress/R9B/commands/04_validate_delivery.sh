#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LEDGER="$OUT/commands/04_validate_delivery.log"

: >"$LEDGER"
record_to() {
    local label=$1 outfile=$2
    shift 2
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
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
record_to validate_final_identity_status "$RAW/validate_final_identity_status.stdout.txt" rg -n '^IDENTITY_FREEZE=RED_STOP_TAG_ARCHIVE_SHA_MISMATCH$' "$OUT/identity/identity_freeze.status" || overall=1
record_to validate_execution_status "$RAW/validate_execution_status.stdout.txt" bash -o pipefail -c \
    'rg -q "^common_identity_freeze[[:space:]]+RED_STOP_TAG_ARCHIVE_SHA_MISMATCH" "$1" && test "$(rg -c "NOT_RUN_COMMON_INPUT_RED_STOP" "$1")" -eq 4 && rg -q "SOURCE_PROVENANCE_update[[:space:]]+ROLLED_BACK_NOT_COMMITTED" "$1"' _ "$OUT/execution_status.tsv" || overall=1
record_to validate_patch_inventory "$RAW/validate_patch_inventory.stdout.txt" bash -o pipefail -c \
    'test "$(tail -n +2 "$1" | wc -l)" -eq 6 && test "$(rg -c "[[:space:]]MATCH$" "$1")" -eq 6 && rg -q "^0006-.*[[:space:]]3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b[[:space:]].*[[:space:]]ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479[[:space:]]MATCH$" "$1"' _ "$OUT/export_patch_inventory.tsv" || overall=1
record_to validate_source0_adjudication "$RAW/validate_source0_adjudication.stdout.txt" bash -o pipefail -c \
    'rg -q "local_standard_GBS_tag_archive_sha256.*1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2.*MISMATCH_RED_STOP" "$1" && rg -q "extracted_tree_diff.*CONFIRMED" "$1"' _ "$OUT/identity/tag_archive_adjudication.tsv" || overall=1
record_to validate_corrected_gbs_command "$RAW/validate_corrected_gbs_command.stdout.txt" bash -o pipefail -c \
    'sed -n "/LABEL=gbs_corrected_standard_export_preflight/,+3p" "$1" | tee "$2"; rg -q -- "--packaging-dir[[:space:]]+packaging" "$2" && rg -q -- "--export-only" "$2" && ! rg -q -- "--no-patch-export" "$2" && rg -q "EXIT_CODE=0" "$2"' _ "$OUT/commands/02b_standard_export_preflight_corrected.log" "$RAW/validate_corrected_gbs_command.extract.txt" || overall=1
record_to validate_preflight_roots_retired "$RAW/validate_preflight_roots_retired.stdout.txt" bash -o pipefail -c \
    'for r in "$@"; do test -f "$r/RETIRED_SINGLE_USE" && cat "$r/RETIRED_SINGLE_USE"; done' _ \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std" \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2" || overall=1
record_to validate_no_builds_or_new_artifacts "$RAW/validate_no_builds_or_new_artifacts.stdout.txt" bash -o pipefail -c \
    'for a in armv7l aarch64 x86_64; do test ! -e "$1/tmp/GBS-ROOT/LIBCXX-2218-${a}-20260805-std" || exit 1; done; test "$(find "$1/artifacts/rebuild_2218_std" -type f 2>/dev/null | wc -l)" -eq 0' _ "$REPO" || overall=1
record_to validate_old_artifacts_unchanged_location "$RAW/validate_old_artifacts_unchanged_location.stdout.txt" bash -o pipefail -c \
    'test -d "$1/artifacts/rebuild_2218" && test ! -e "$1/artifacts/quarantine/20260805_pre_std_export"' _ "$REPO" || overall=1
record_to validate_packaging_unchanged "$RAW/validate_packaging_unchanged.stdout.txt" git diff --exit-code -- packaging || overall=1
record_to validate_llvm_final_clean "$RAW/validate_llvm_final_clean.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" branch --show-current)" = tizen_base && test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$REPO/codes/llvm" || overall=1
record_to validate_report_markers "$RAW/validate_report_markers.stdout.txt" bash -o pipefail -c \
    'rg -q "RED_STOP_TAG_ARCHIVE_SHA_MISMATCH" "$1" && rg -q "未进入任何架构构建" "$1" && rg -q "PENDING_BOARD_VERIFICATION" "$1"' _ "$OUT/R9B_report.md" || overall=1

if [[ $overall -eq 0 ]]; then
    printf 'DELIVERY_VALIDATION=PASS_RED_STOP_EVIDENCE_COMPLETE\n' >"$OUT/delivery_validation.status"
else
    printf 'DELIVERY_VALIDATION=FAIL\n' >"$OUT/delivery_validation.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
