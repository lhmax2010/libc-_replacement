#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/raw"
IDENTITY="$OUT/identity"
BUILD="$OUT/build"
LEDGER="$OUT/commands/02_prepare_and_export_preflight.log"
WORKTREE="$REPO/tmp/R9C/seg2/gbs-source-c2"
RETIRED_PACKAGING="$REPO/tmp/R9C/seg2/original-platform-packaging"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260806-c2"
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
SEED_SOURCE="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/llvm-22.1.8.tar.gz"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
EXPECTED_SOURCE0=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2
LOG="$BUILD/standard_export_preflight.full.log"

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
record_to assert_preflight_paths_fresh "$RAW/assert_preflight_paths_fresh.stdout.txt" bash -o pipefail -c \
    'test ! -e "$1" && test ! -e "$2" && test ! -e "$3"' _ "$WORKTREE" "$RETIRED_PACKAGING" "$ROOT" || exit 90
record_to assert_authoritative_source_preworktree "$RAW/assert_authoritative_source_preworktree.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" "$EXPECTED_HEAD" || exit 91
record_to create_worktree_parent "$RAW/create_worktree_parent.stdout.txt" mkdir -p "$(dirname "$WORKTREE")" || overall=1
record_to add_detached_worktree "$RAW/add_detached_worktree.stdout.txt" git -C "$LLVM" worktree add --detach "$WORKTREE" "$EXPECTED_HEAD" || overall=1
record_to create_retired_packaging_dir "$RAW/create_retired_packaging_dir.stdout.txt" mkdir "$RETIRED_PACKAGING" || overall=1
record_to preserve_original_platform_packaging "$RAW/preserve_original_platform_packaging.stdout.txt" bash -o pipefail -c \
    'find "$1/packaging" -mindepth 1 -maxdepth 1 -exec mv -t "$2" -- {} +' _ "$WORKTREE" "$RETIRED_PACKAGING" || overall=1
record_to overlay_copy_spec "$RAW/overlay_copy_spec.stdout.txt" cp -a "$REPO/packaging/libcxx-runtimes.spec" "$WORKTREE/packaging/" || overall=1
record_to overlay_copy_map "$RAW/overlay_copy_map.stdout.txt" cp -a "$REPO/packaging/libcxx-llvm22.map" "$WORKTREE/packaging/" || overall=1
record_to overlay_copy_manifest "$RAW/overlay_copy_manifest.stdout.txt" cp -a "$REPO/packaging/libcxx-runtimes.manifest" "$WORKTREE/packaging/" || overall=1
record_to overlay_copy_provenance "$RAW/overlay_copy_provenance.stdout.txt" cp -a "$REPO/packaging/SOURCE_PROVENANCE" "$WORKTREE/packaging/" || overall=1
record_to overlay_copy_source0_seed "$RAW/overlay_copy_source0_seed.stdout.txt" cp -a "$SEED_SOURCE" "$WORKTREE/packaging/llvm-22.1.8.tar.gz" || overall=1
record_to worktree_packaging_sha256 "$IDENTITY/worktree_packaging_sha256.txt" bash -o pipefail -c \
    'find "$1/packaging" -maxdepth 1 -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$WORKTREE" || overall=1
record_to worktree_status "$IDENTITY/worktree_status.txt" git -C "$WORKTREE" status --porcelain=v1 || overall=1
record_to assert_worktree_shape "$RAW/assert_worktree_shape.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2"; test "$(find "$1/packaging" -maxdepth 1 -type f | wc -l)" -eq 5; test "$(find "$1/packaging" -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1; rg -q "^Name:[[:space:]]+libcxx-runtimes$" "$1/packaging/libcxx-runtimes.spec"; test "$(sha256sum "$1/packaging/llvm-22.1.8.tar.gz" | cut -d" " -f1)" = "$3"; test "$(git -C "$1" diff --name-only | sed "/^packaging\\//d;/^$/d" | wc -l)" -eq 0' _ \
    "$WORKTREE" "$EXPECTED_HEAD" "$EXPECTED_SOURCE0" || overall=1
[[ $overall -eq 0 ]] || exit 92

cd "$WORKTREE" || exit 125
CMD=(gbs -c "$CONFIG" build
    -A armv7l
    -B "$ROOT"
    -C
    --debug
    --style git
    --include-all
    --packaging-dir packaging
    --spec libcxx-runtimes.spec
    --export-only
    "$WORKTREE")
{
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'PURPOSE=R9C_SEG2_STANDARD_EXPORT_ONLY_PREFLIGHT\n'
    printf 'COMMAND_BEGIN\n'
    printf '%q ' "${CMD[@]}"
    printf '\nCOMMAND_END\nOUTPUT_BEGIN\n'
} >"$LOG"
"${CMD[@]}" >>"$LOG" 2>&1
gbs_rc=$?
printf 'OUTPUT_END\nEXIT_CODE=%s\n' "$gbs_rc" >>"$LOG"
{
    printf 'LABEL=gbs_standard_export_preflight\nPWD=%s\nCOMMAND=' "$PWD"
    printf '%q ' "${CMD[@]}"
    printf '\nEXIT_CODE=%s\n\n' "$gbs_rc"
} >>"$LEDGER"
[[ $gbs_rc -eq 0 ]] || overall=1

SOURCE_EXPORT=$(find "$ROOT/local/sources" -type d -path '*/libcxx-runtimes-22.1.8-1' -print -quit 2>/dev/null)
printf '%s\n' "${SOURCE_EXPORT:-NOT_FOUND}" >"$IDENTITY/preflight_source_export_dir.txt"
[[ -n "$SOURCE_EXPORT" ]] || overall=1

if [[ -n "$SOURCE_EXPORT" ]]; then
    record_to assert_standard_export_mode "$RAW/assert_standard_export_mode.stdout.txt" bash -o pipefail -c \
        'rg -q "gbs --debug export" "$1" && rg -q -- "--git-patch-export" "$1" && rg -q -- "--packaging-dir=packaging" "$1" && ! rg -q -- "--no-patch-export" "$1"' _ "$LOG" || overall=1
    record_to prebuild_single_target_assertion "$IDENTITY/prebuild_single_target_assertion.txt" bash -o pipefail -c \
        'test "$(find "$1" -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1; rg -q "^Name:[[:space:]]+libcxx-runtimes$" "$1/libcxx-runtimes.spec"; ! rg -q "circle found|init_buildsystem|Executing\\(%build\\)|BUILD-ROOTS|rpmbuild[[:space:]]+-b" "$2"; printf "TARGET=libcxx-runtimes\\nSPEC_COUNT=1\\nGBS_EXPORT_ONLY_EXIT=0\\nFULL_REPOSITORY_SCAN=NO\\nCIRCLE_FOUND=NO\\nSTATUS=PASS_SINGLE_PACKAGE_PREFLIGHT\\n"' _ \
        "$SOURCE_EXPORT" "$LOG" || overall=1
    record_to exported_source0_sha256 "$IDENTITY/preflight_source0_sha256.txt" sha256sum "$SOURCE_EXPORT/llvm-22.1.8.tar.gz" || overall=1
    record_to assert_exported_source0_anchor "$RAW/assert_exported_source0_anchor.stdout.txt" bash -o pipefail -c \
        'test "$(sha256sum "$1" | cut -d" " -f1)" = "$2"' _ "$SOURCE_EXPORT/llvm-22.1.8.tar.gz" "$EXPECTED_SOURCE0" || overall=1
    record_to assert_exported_provenance_identity "$RAW/assert_exported_provenance_identity.stdout.txt" cmp -s \
        "$SOURCE_EXPORT/SOURCE_PROVENANCE" "$REPO/packaging/SOURCE_PROVENANCE" || overall=1
    record_to exported_spec_patch_lines "$IDENTITY/preflight_exported_spec_patch_lines.txt" rg -n \
        '^# Patches auto-generated|^Patch[0-9]+:|^# 000[0-9]-|^%patch[0-9]+ ' "$SOURCE_EXPORT/libcxx-runtimes.spec" || overall=1
    record_to preflight_patch_inventory "$IDENTITY/preflight_patch_inventory.tsv" bash -o pipefail -c \
        'printf "patch_file\\tsha256\\tpatch_id\\n"; for p in "$1"/0*.patch; do printf "%s\\t%s\\t%s\\n" "$(basename "$p")" "$(sha256sum "$p" | cut -d" " -f1)" "$(git patch-id --stable < "$p" | cut -d" " -f1)"; done' _ "$SOURCE_EXPORT" || overall=1
    record_to assert_preflight_patch_identity "$RAW/assert_preflight_patch_identity.stdout.txt" bash -o pipefail -c \
        'test "$(tail -n +2 "$1" | wc -l)" -eq 6; while IFS=$'\''\\t'\'' read -r file sha pid; do [[ $file == patch_file ]] && continue; rg -q "^${file}\\t${sha}\\t${pid}\\t" "$2"; done < "$1"; rg -q "^Patch5:[[:space:]]+0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch$" "$3"; rg -q "^%patch5 -p1$" "$3"' _ \
        "$IDENTITY/preflight_patch_inventory.tsv" "$IDENTITY/frozen_patch_inventory.tsv" "$SOURCE_EXPORT/libcxx-runtimes.spec" || overall=1
    record_to preflight_local_archival_content "$IDENTITY/preflight_local.git_archival.txt" tar -xOzf \
        "$SOURCE_EXPORT/llvm-22.1.8.tar.gz" llvm-22.1.8/clang/bindings/python/.git_archival.txt || overall=1
    record_to assert_preflight_archival_matches_segment1 "$RAW/assert_preflight_archival_matches_segment1.stdout.txt" cmp -s \
        "$IDENTITY/preflight_local.git_archival.txt" "$REPO/progress/R9C/inputs/local_standard_export.git_archival.txt" || overall=1
fi

record_to retire_preflight_root "$RAW/retire_preflight_root.stdout.txt" bash -o pipefail -c \
    'printf "RETIRED_SINGLE_USE\\npurpose=R9C_segment2_standard_export_only_preflight\\ngbs_exit_code=%s\\nroot=%s\\n" "$2" "$1" > "$1/RETIRED_SINGLE_USE"; cat "$1/RETIRED_SINGLE_USE"' _ "$ROOT" "$gbs_rc" || overall=1
record_to assert_authoritative_source_postpreflight "$RAW/assert_authoritative_source_postpreflight.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" "$EXPECTED_HEAD" || overall=1
record_to final_worktree_status "$IDENTITY/worktree_status_after_preflight.txt" git -C "$WORKTREE" status --porcelain=v1 || overall=1

if [[ $overall -eq 0 ]]; then
    printf 'STANDARD_EXPORT_PREFLIGHT=PASS_SINGLE_PACKAGE_SOURCE0_R9C_6_PATCHES\n' >"$OUT/preflight.status"
else
    printf 'STANDARD_EXPORT_PREFLIGHT=RED_STOP\n' >"$OUT/preflight.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
