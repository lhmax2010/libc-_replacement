#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
LEDGER="$OUT/commands/04a_validate_identity_evidence.log"
EXPECTED=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2
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

fail=0
record classify_prior_nonzero bash -o pipefail -c \
    'test "$(rg -c "^EXIT_CODE=1$" "$1")" -eq 1 && awk '\''$0=="LABEL=compare_patch_frozen" {seen=1; next} seen && $0=="EXIT_CODE=1" {ok=1; exit} END {exit !ok}'\'' "$1"' _ \
    "$OUT/commands/04_identity_selfcheck.log" || fail=1

record compare_srpm_patch_to_frozen_without_header bash -o pipefail -c \
    'cmp -s "$1" <(tail -n +2 "$2" | cut -f1-3)' _ \
    "$OUT/identity/srpm_patch_inventory.armv7l.tsv" "$OUT/identity/frozen_patch_inventory.tsv" || fail=1
record assert_source0_three_way bash -o pipefail -c \
    'test "$(tail -n +2 "$1" | cut -f4 | sort -u | wc -l)" -eq 1 && test "$(tail -n +2 "$1" | cut -f4 | sort -u)" = "$2"' _ \
    "$OUT/identity/srpm_source_identity.tsv" "$EXPECTED" || fail=1
record compare_patch_three_way bash -o pipefail -c \
    'cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ \
    "$OUT/identity/srpm_patch_inventory.armv7l.tsv" \
    "$OUT/identity/srpm_patch_inventory.aarch64.tsv" \
    "$OUT/identity/srpm_patch_inventory.x86_64.tsv" || fail=1
record compare_payload_three_way bash -o pipefail -c \
    'cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ \
    "$OUT/identity/srpm_payload_manifest.armv7l.tsv" \
    "$OUT/identity/srpm_payload_manifest.aarch64.tsv" \
    "$OUT/identity/srpm_payload_manifest.x86_64.tsv" || fail=1
record compare_provenance_three_way bash -o pipefail -c \
    'cmp -s "$1" "$4" && cmp -s "$2" "$4" && cmp -s "$3" "$4"' _ \
    "$REPO/tmp/R9C/seg2/srpm_identity/armv7l/SOURCE_PROVENANCE" \
    "$REPO/tmp/R9C/seg2/srpm_identity/aarch64/SOURCE_PROVENANCE" \
    "$REPO/tmp/R9C/seg2/srpm_identity/x86_64/SOURCE_PROVENANCE" \
    "$REPO/packaging/SOURCE_PROVENANCE" || fail=1
record assert_ehabi_patch5 bash -o pipefail -c \
    'rg -q "^0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch[[:space:]]+b9fdf66418276bbb97183be952aab96606c0662b17b95de077e99fecc7d2d79c[[:space:]]+ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479$" "$1"' _ \
    "$OUT/identity/srpm_patch_inventory.armv7l.tsv" || fail=1

if [[ $fail -eq 0 ]]; then
    {
        printf 'IDENTITY_SELFCHECK=PASS\n'
        printf 'PRIOR_EXIT_CLASSIFICATION=SPEC_ERROR_IN_EVIDENCE_ASSERTION_HEADER_INCLUDED\n'
        printf 'SOURCE0_THREE_WAY=%s\n' "$EXPECTED"
        printf 'PATCHES_THREE_WAY=IDENTICAL_6_AND_MATCH_FROZEN\n'
        printf 'SRPM_PAYLOAD_THREE_WAY=IDENTICAL\n'
        printf 'SOURCE_PROVENANCE_THREE_WAY=IDENTICAL_AND_MATCHES_PACKAGING\n'
        printf 'EHABI_PATCH=0006/Patch5\n'
    } >"$OUT/identity/identity_selfcheck.status"
    printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
    exit 0
fi
printf 'IDENTITY_SELFCHECK=RED\n' >"$OUT/identity/identity_selfcheck.status"
printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
exit 1
