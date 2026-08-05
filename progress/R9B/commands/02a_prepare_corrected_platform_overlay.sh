#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OLD_WORKTREE="$REPO/tmp/R9B/gbs-source-std"
WORKTREE="$REPO/tmp/R9B/gbs-source-std-r2"
RETIRED_PACKAGING="$REPO/tmp/R9B/original-platform-packaging-r2"
CANONICAL_SOURCE="$REPO/tmp/R9B/input/canonical/llvm-22.1.8.tar.gz"
SRPM="$REPO/tmp/corpus/srpms/llvm-22.1.8-18.1.src.rpm"
INPUT="$REPO/packaging"
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LEDGER="$OUT/commands/02a_prepare_corrected_platform_overlay.retry.log"
HEAD_SHA=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
EXPECTED_TAR=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e

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
record_to assert_first_preflight_classification "$RAW/assert_first_preflight_classification.stdout.txt" bash -o pipefail -c \
    'rg -q "^STANDARD_EXPORT_PREFLIGHT=RED_STOP$" "$1" && ! rg -q "circle found" "$2" && rg -q "EXIT_CODE=0" "$2" && rg -q "0001-Add-packaging.patch.gz" "$3" && rg -q "actual=1f5fe8" "$4"' \
    _ "$OUT/build/standard_export_preflight.status" "$OUT/build/standard_export_preflight.full.log" \
    "$RAW/preflight_export_file_list.stdout.txt" "$RAW/preflight_assert_source0_identity.stdout.txt" || overall=1
record_to assert_first_preflight_retired "$RAW/assert_first_preflight_retired.stdout.txt" cat \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std/RETIRED_SINGLE_USE" || overall=1
record_to assert_no_build_in_first_preflight "$RAW/assert_no_build_in_first_preflight.stdout.txt" bash -o pipefail -c \
    '! rg -q "init_buildsystem|Executing\\(%build\\)|BUILD-ROOTS|rpmbuild[[:space:]]+-b" "$1"' _ "$OUT/build/standard_export_preflight.full.log" || overall=1

record_to assert_corrected_paths_fresh "$RAW/assert_corrected_paths_fresh.stdout.txt" bash -o pipefail -c \
    'test ! -e "$1" && test ! -e "$2" && test ! -e "$3"' _ "$WORKTREE" "$RETIRED_PACKAGING" "$CANONICAL_SOURCE" || overall=1
record_to assert_authoritative_source_clean "$RAW/assert_authoritative_source_clean.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" "$HEAD_SHA" || overall=1

if [[ $overall -eq 0 ]]; then
    record_to create_canonical_source_dir "$RAW/create_canonical_source_dir.stdout.txt" mkdir -p "$(dirname "$CANONICAL_SOURCE")" || overall=1
    record_to extract_canonical_source0 "$RAW/extract_canonical_source0.stdout.txt" bash -o pipefail -c \
        'rpm2cpio "$1" | cpio -i --to-stdout llvm-22.1.8.tar.gz 2>"$3" >"$2"' _ "$SRPM" "$CANONICAL_SOURCE" "$RAW/extract_canonical_source0.cpio.stderr.txt" || overall=1
    record_to canonical_source0_sha256 "$RAW/canonical_source0_sha256.stdout.txt" sha256sum "$CANONICAL_SOURCE" || overall=1
    record_to assert_canonical_source0_sha256 "$RAW/assert_canonical_source0_sha256.stdout.txt" bash -o pipefail -c \
        'test "$(sha256sum "$1" | cut -d" " -f1)" = "$2"' _ "$CANONICAL_SOURCE" "$EXPECTED_TAR" || overall=1
    record_to add_corrected_detached_worktree "$RAW/add_corrected_detached_worktree.stdout.txt" git -C "$LLVM" worktree add --detach "$WORKTREE" "$HEAD_SHA" || overall=1
    record_to create_retired_packaging_dir "$RAW/create_retired_packaging_dir.stdout.txt" mkdir "$RETIRED_PACKAGING" || overall=1
    record_to preserve_original_packaging "$RAW/preserve_original_packaging.stdout.txt" bash -o pipefail -c \
        'find "$1/packaging" -mindepth 1 -maxdepth 1 -exec mv -t "$2" -- {} +' _ "$WORKTREE" "$RETIRED_PACKAGING" || overall=1
    record_to overlay_copy_spec "$RAW/overlay_copy_spec.stdout.txt" cp -a "$INPUT/libcxx-runtimes.spec" "$WORKTREE/packaging/" || overall=1
    record_to overlay_copy_map "$RAW/overlay_copy_map.stdout.txt" cp -a "$INPUT/libcxx-llvm22.map" "$WORKTREE/packaging/" || overall=1
    record_to overlay_copy_manifest "$RAW/overlay_copy_manifest.stdout.txt" cp -a "$INPUT/libcxx-runtimes.manifest" "$WORKTREE/packaging/" || overall=1
    record_to overlay_copy_provenance "$RAW/overlay_copy_provenance.stdout.txt" cp -a "$INPUT/SOURCE_PROVENANCE" "$WORKTREE/packaging/" || overall=1
    record_to overlay_copy_source0 "$RAW/overlay_copy_source0.stdout.txt" cp -a "$CANONICAL_SOURCE" "$WORKTREE/packaging/" || overall=1
fi

record_to corrected_worktree_status "$RAW/corrected_worktree_status.stdout.txt" git -C "$WORKTREE" status --porcelain=v1 || overall=1
record_to corrected_worktree_diff_names "$RAW/corrected_worktree_diff_names.stdout.txt" git -C "$WORKTREE" diff --name-status || overall=1
record_to corrected_packaging_file_list "$RAW/corrected_packaging_file_list.stdout.txt" find "$WORKTREE/packaging" -maxdepth 1 -type f -printf '%f\t%s\n' || overall=1
record_to corrected_packaging_sha256 "$RAW/corrected_packaging_sha256.stdout.txt" bash -o pipefail -c \
    'find "$1/packaging" -maxdepth 1 -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$WORKTREE" || overall=1
record_to assert_corrected_overlay_shape "$RAW/assert_corrected_overlay_shape.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test "$(find "$1/packaging" -maxdepth 1 -type f | wc -l)" -eq 5 && test "$(find "$1/packaging" -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1 && rg -q "^Name:[[:space:]]+libcxx-runtimes$" "$1/packaging/libcxx-runtimes.spec" && test "$(sha256sum "$1/packaging/llvm-22.1.8.tar.gz" | cut -d" " -f1)" = "$3" && test "$(git -C "$1" diff --name-only | sed "/^packaging\\//d;/^$/d" | wc -l)" -eq 0' \
    _ "$WORKTREE" "$HEAD_SHA" "$EXPECTED_TAR" || overall=1
record_to assert_authoritative_source_unchanged_after_overlay "$RAW/assert_authoritative_source_unchanged_after_overlay.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" "$HEAD_SHA" || overall=1
record_to corrected_worktree_list "$RAW/corrected_worktree_list.stdout.txt" git -C "$LLVM" worktree list --porcelain || overall=1

{
    printf 'item\tclassification\tevidence\n'
    printf 'first_preflight_custom_packaging_dir\tSPEC_ERROR_IN_PREFLIGHT_PACKAGING_DIR\toriginal_packaging_history_not_excluded_produced_8_patches\n'
    printf 'first_preflight_source0\tMISSING_FROZEN_SOURCE_INPUT\tGBS_recompressed_tag_to_1f5fe8_not_required_16a932\n'
    printf 'first_preflight_build\tNOT_STARTED\texport_only_no_init_buildsystem_no_compiler\n'
    printf 'retry_permission\tALLOWED_NO_FULL_SCAN_OR_CIRCLE\ttask_prohibits_second_context_only_after_full_scan_or_circle\n'
    printf 'corrected_packaging_dir\tpackaging\tGBS_standard_ignore_path_matches_platform_layout\n'
    printf 'corrected_source0\tverbatim_platform_SRPM_Source0_16a932\tnot_repacked\n'
    printf 'authoritative_branch_content\tUNCHANGED\tcodes/llvm_status_zero\n'
} >"$OUT/preflight_incident.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
