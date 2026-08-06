#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/raw"
IDENTITY="$OUT/identity"
LEDGER="$OUT/commands/01_validate_provenance.log"
PROV="$REPO/packaging/SOURCE_PROVENANCE"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b

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
record_to provenance_numbered "$IDENTITY/SOURCE_PROVENANCE.numbered.txt" nl -ba "$PROV" || overall=1
record_to provenance_sha256 "$IDENTITY/SOURCE_PROVENANCE.sha256" sha256sum "$PROV" || overall=1
record_to provenance_diff "$IDENTITY/SOURCE_PROVENANCE.diff" git diff -- "$PROV" || overall=1
record_to assert_project_diff_scope "$RAW/assert_project_diff_scope.stdout.txt" bash -o pipefail -c \
    'test "$(git diff --name-only)" = "packaging/SOURCE_PROVENANCE" && test "$(git status --short | sed "/^ M packaging\/SOURCE_PROVENANCE$/d;/^?? gbs_llvm.conf$/d;/^$/d" | wc -l)" -eq 0' || overall=1
record_to assert_primary_anchors "$RAW/assert_primary_anchors.stdout.txt" bash -o pipefail -c \
    'rg -qx "source_tarball_sha256=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2" "$1" && rg -qx "source_tarball_anchor=R9C_GBS_STANDARD_EXPORT" "$1" && rg -qx "upstream_peeled_commit=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1" "$1" && rg -qx "official_release_archive_sha256=922f1817a0df7b1489272d18134ee0087a8b068828f87ac63b9861b1a9965888" "$1"' _ "$PROV" || overall=1
record_to assert_superseded_anchors "$RAW/assert_superseded_anchors.stdout.txt" bash -o pipefail -c \
    'rg -qx "platform_frozen_tarball_sha256=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e" "$1" && rg -qx "platform_frozen_tarball_status=SUPERSEDED_BY_R9C" "$1" && rg -qx "superseded_platform_spec_vcs=platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146" "$1" && rg -qx "superseded_platform_spec_vcs_status=SUPERSEDED_BY_R9C_TEST_COMMIT_ID_NON_AUTHORITATIVE" "$1"' _ "$PROV" || overall=1
record_to assert_delta_and_no_build_impact "$RAW/assert_delta_and_no_build_impact.stdout.txt" bash -o pipefail -c \
    'rg -qx "platform_vs_r9c_source0_difference=clang/bindings/python/.git_archival.txt only" "$1" && rg -qx "platform_vs_r9c_source0_difference_cause=git export-subst expansion in local standard export" "$1" && rg -qx "platform_vs_r9c_source0_build_impact=NO_BUILD_IMPACT" "$1" && rg -q "libcxx-runtimes-22.1.8-build.*NO_BUILD_IMPACT" "$2"' _ \
    "$PROV" "$REPO/progress/R9C/build_impact_verdict.tsv" || overall=1
record_to assert_patch_anchors_match_freeze "$RAW/assert_patch_anchors_match_freeze.stdout.txt" bash -o pipefail -c \
    'test "$(rg -c "^export_patch_[0-9]{4}_file=" "$1")" -eq 6; test "$(rg -c "^export_patch_[0-9]{4}_sha256=" "$1")" -eq 6; test "$(rg -c "^export_patch_[0-9]{4}_patch_id=" "$1")" -eq 6; test "$(rg -c "^export_patch_[0-9]{4}_commit=" "$1")" -eq 6; while IFS=$'\''\\t'\'' read -r file sha pid commit; do [[ $file == patch_file ]] && continue; n=${file%%-*}; key=${n#0}; key=$(printf "%04d" "$((10#$key))"); rg -qx "export_patch_${key}_file=${file}" "$1"; rg -qx "export_patch_${key}_sha256=${sha}" "$1"; rg -qx "export_patch_${key}_patch_id=${pid}" "$1"; rg -qx "export_patch_${key}_commit=${commit}" "$1"; done < "$2"; rg -qx "ehabi_backport_spec_patch_entry=Patch5" "$1"' _ \
    "$PROV" "$IDENTITY/frozen_patch_inventory.tsv" || overall=1
record_to assert_nonprovenance_packaging_unchanged "$RAW/assert_nonprovenance_packaging_unchanged.stdout.txt" bash -o pipefail -c \
    'for f in "$1"/*; do b=$(basename "$f"); [[ $b == SOURCE_PROVENANCE ]] && continue; old=$(grep " /home/toolchain/development/libc++_replacement/packaging/$b$" "$2" | cut -d" " -f1); now=$(sha256sum "$f" | cut -d" " -f1); printf "%s\\t%s\\t%s\\n" "$b" "$old" "$now"; test -n "$old" && test "$old" = "$now"; done' _ \
    "$REPO/packaging" "$IDENTITY/packaging_before_sha256.txt" || overall=1
record_to assert_llvm_still_clean "$RAW/assert_llvm_still_clean_after_provenance.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" "$EXPECTED_HEAD" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
