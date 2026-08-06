#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/raw"
IDENTITY="$OUT/identity"
LEDGER="$OUT/commands/02a_validate_successful_preflight.log"
LOG="$OUT/build/standard_export_preflight.full.log"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260806-c2"
SOURCE_EXPORT="$ROOT/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1"

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
record_to classify_original_preflight_exit "$RAW/classify_original_preflight_exit.stdout.txt" bash -o pipefail -c \
    'rg -q "^EXIT_CODE=0$" "$1"; rg -q "^LABEL=assert_standard_export_mode" "$2"; rg -q -A3 "^LABEL=assert_standard_export_mode" "$2" | rg -q "EXIT_CODE=1"; rg -q "^OVERALL_EXIT_CODE=1$" "$2"; printf "CLASSIFICATION=SPEC_ERROR_IN_EVIDENCE_ASSERTION\\nGBS_EXPORT_EXIT=0\\nRETRY_EXPORT=NOT_PERFORMED\\n"' _ \
    "$LOG" "$OUT/commands/02_prepare_and_export_preflight.log" || overall=1
record_to assert_standard_patch_export_from_actual_log "$RAW/assert_standard_patch_export_from_actual_log.stdout.txt" bash -o pipefail -c \
    'rg -q "gbs --debug export" "$1"; rg -q "Generating patches from git" "$1"; rg -q "Starting autoupdate patch numbering from 0" "$1"; rg -q "Adding .Patch5:. tag" "$1"; ! rg -q -- "--no-patch-export" "$1"; printf "STANDARD_PATCH_EXPORT=CONFIRMED\\nEVIDENCE=Generating patches from git; Patch0..Patch5 injection; no --no-patch-export\\n"' _ "$LOG" || overall=1
record_to assert_successful_export_identity "$RAW/assert_successful_export_identity.stdout.txt" bash -o pipefail -c \
    'test "$(sha256sum "$1/llvm-22.1.8.tar.gz" | cut -d" " -f1)" = 1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2; test "$(find "$1" -maxdepth 1 -type f -name "0*.patch" | wc -l)" -eq 6; test "$(find "$1" -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1; rg -q "^Name:[[:space:]]+libcxx-runtimes$" "$1/libcxx-runtimes.spec"; cmp -s <(cut -f1-3 "$2") <(cut -f1-3 "$3")' _ \
    "$SOURCE_EXPORT" "$IDENTITY/preflight_patch_inventory.tsv" "$IDENTITY/provenance_patch_inventory.tsv" || overall=1
record_to assert_no_build_started "$RAW/assert_no_build_started_in_preflight.stdout.txt" bash -o pipefail -c \
    '! rg -q "init_buildsystem|Executing\\(%build\\)|rpmbuild[[:space:]]+-b|Building (CXX|C|ASM) object" "$1"; test ! -d "$2/local/BUILD-ROOTS"' _ "$LOG" "$ROOT" || overall=1
record_to preflight_retirement "$RAW/preflight_retirement.stdout.txt" cat "$ROOT/RETIRED_SINGLE_USE" || overall=1
record_to authoritative_source_clean_after_preflight "$RAW/authoritative_source_clean_after_preflight.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$REPO/codes/llvm" || overall=1

if [[ $overall -eq 0 ]]; then
    printf 'STANDARD_EXPORT_PREFLIGHT=PASS_SINGLE_PACKAGE_SOURCE0_R9C_6_PATCHES\n' >"$OUT/preflight.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
