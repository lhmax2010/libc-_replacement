#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/raw"
IDENTITY="$OUT/identity"
LEDGER="$OUT/commands/01a_strict_provenance_patch_validation.log"
PROV="$REPO/packaging/SOURCE_PROVENANCE"

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
record_to provenance_patch_inventory "$IDENTITY/provenance_patch_inventory.tsv" bash -o pipefail -c \
    'printf "patch_file\\tsha256\\tpatch_id\\tfrom_commit\\n"; for n in 1 2 3 4 5 6; do key=$(printf "%04d" "$n"); file=$(sed -n "s/^export_patch_${key}_file=//p" "$1"); sha=$(sed -n "s/^export_patch_${key}_sha256=//p" "$1"); pid=$(sed -n "s/^export_patch_${key}_patch_id=//p" "$1"); commit=$(sed -n "s/^export_patch_${key}_commit=//p" "$1"); test -n "$file" && test -n "$sha" && test -n "$pid" && test -n "$commit" || exit 1; printf "%s\\t%s\\t%s\\t%s\\n" "$file" "$sha" "$pid" "$commit"; done' _ "$PROV" || overall=1
record_to compare_provenance_to_frozen_inventory "$IDENTITY/provenance_vs_frozen_patch_inventory.diff" diff -u \
    "$IDENTITY/frozen_patch_inventory.tsv" "$IDENTITY/provenance_patch_inventory.tsv"
diff_rc=$?
printf 'EXPECTED_EXIT_CODE=0\n\n' >>"$LEDGER"
[[ $diff_rc -eq 0 ]] || overall=1
record_to assert_ehabi_mapping_exact "$RAW/assert_ehabi_mapping_exact.stdout.txt" bash -o pipefail -c \
    'rg -qx "ehabi_backport_export_patch=0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch" "$1" && rg -qx "ehabi_backport_spec_patch_entry=Patch5" "$1"' _ "$PROV" || overall=1
record_to assert_validation_stderr_empty "$RAW/assert_strict_provenance_validation_stderr_empty.stdout.txt" bash -o pipefail -c \
    'for f in "$@"; do test ! -s "$f" || { printf "NONEMPTY=%s\\n" "$f"; exit 1; }; done' _ \
    "$RAW/provenance_patch_inventory.stderr.txt" "$RAW/compare_provenance_to_frozen_inventory.stderr.txt" "$RAW/assert_ehabi_mapping_exact.stderr.txt" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
