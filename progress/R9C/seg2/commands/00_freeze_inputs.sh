#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/raw"
IDENTITY="$OUT/identity"
COMPARISONS="$OUT/comparisons"
LEDGER="$OUT/commands/00_freeze_inputs.attempt3.log"
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
BUILDCONF=/var/tmp/toolchain-gbs/tizen_unified_standard.conf
OLD_EXPORT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
EXPECTED_SOURCE0=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2
EXPECTED_UPSTREAM=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1

mkdir -p "$RAW" "$IDENTITY" "$COMPARISONS" "$OUT/build" "$OUT/gates" "$OUT/architectures"
bootstrap_rc=$?
{
    printf 'LABEL=create_segment2_output_directories\nPWD=%s\nCOMMAND=' "$PWD"
    printf '%q ' mkdir -p "$RAW" "$IDENTITY" "$COMPARISONS" "$OUT/build" "$OUT/gates" "$OUT/architectures"
    printf '\nEXIT_CODE=%s\n\n' "$bootstrap_rc"
} >"$LEDGER"
[[ $bootstrap_rc -eq 0 ]] || exit "$bootstrap_rc"

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
record_to project_identity "$IDENTITY/project_identity.txt" bash -o pipefail -c \
    'printf "BRANCH=%s\\nHEAD=%s\\nSTATUS_BEGIN\\n" "$(git branch --show-current)" "$(git rev-parse HEAD)"; git status --short; printf "STATUS_END\\n"' || overall=1
record_to assert_only_known_untracked_project_file "$RAW/assert_only_known_untracked_project_file.stdout.txt" bash -o pipefail -c \
    'test "$(git status --short)" = "?? gbs_llvm.conf"' || overall=1
record_to llvm_identity "$IDENTITY/llvm_identity.txt" bash -o pipefail -c \
    'printf "BRANCH=%s\\nHEAD=%s\\nREMOTE_BEGIN\\n" "$(git -C "$1" branch --show-current)" "$(git -C "$1" rev-parse HEAD)"; git -C "$1" remote -v; printf "REMOTE_END\\nSTATUS_BEGIN\\n"; git -C "$1" status --porcelain=v1; printf "STATUS_END\\n"' _ "$LLVM" || overall=1
record_to assert_llvm_frozen_clean "$RAW/assert_llvm_frozen_clean.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" branch --show-current)" = tizen_base && test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ \
    "$LLVM" "$EXPECTED_HEAD" || overall=1
record_to upstream_tag_identity "$IDENTITY/upstream_tag_identity.txt" bash -o pipefail -c \
    'git -C "$1" show-ref --verify refs/tags/upstream/22.1.8; git -C "$1" rev-parse "refs/tags/upstream/22.1.8^{}"; git -C "$1" log --oneline upstream/22.1.8..tizen_base' _ "$LLVM" || overall=1
record_to assert_upstream_peeled "$RAW/assert_upstream_peeled.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse "refs/tags/upstream/22.1.8^{}")" = "$2"' _ "$LLVM" "$EXPECTED_UPSTREAM" || overall=1
record_to packaging_before_sha256 "$IDENTITY/packaging_before_sha256.txt" bash -o pipefail -c \
    'find "$1" -maxdepth 1 -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$REPO/packaging" || overall=1
record_to source0_anchor_recheck "$IDENTITY/source0_anchor_recheck.txt" sha256sum "$OLD_EXPORT/llvm-22.1.8.tar.gz" || overall=1
record_to assert_source0_anchor "$RAW/assert_source0_anchor.stdout.txt" bash -o pipefail -c \
    'test "$(sha256sum "$1" | cut -d" " -f1)" = "$2"' _ "$OLD_EXPORT/llvm-22.1.8.tar.gz" "$EXPECTED_SOURCE0" || overall=1
record_to frozen_patch_inventory "$IDENTITY/frozen_patch_inventory.tsv" bash -o pipefail -c \
    'printf "patch_file\\tsha256\\tpatch_id\\tfrom_commit\\n"; for p in "$1"/0*.patch; do pid=$(git patch-id --stable < "$p" | cut -d" " -f1); from=$(grep -F "$(basename "$p")" "$2" | cut -f3); printf "%s\\t%s\\t%s\\t%s\\n" "$(basename "$p")" "$(sha256sum "$p" | cut -d" " -f1)" "$pid" "$from"; done' _ \
    "$OLD_EXPORT" "$REPO/progress/R9B/export_patch_inventory.tsv" || overall=1
record_to assert_six_frozen_patches "$RAW/assert_six_frozen_patches.stdout.txt" bash -o pipefail -c \
    'test "$(tail -n +2 "$1" | wc -l)" -eq 6 && rg -q "^0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal\\.patch\\tb9fdf66418276bbb97183be952aab96606c0662b17b95de077e99fecc7d2d79c\\tab69099b6f2dd679eeaf8b1dfe82bb13fdabc479\\t3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b$" "$1"' _ \
    "$IDENTITY/frozen_patch_inventory.tsv" || overall=1
record_to config_sha256 "$IDENTITY/config_sha256.txt" sha256sum "$CONFIG" "$BUILDCONF" || overall=1
record_to config_contents "$IDENTITY/config_contents.txt" bash -o pipefail -c \
    'for f in "$@"; do printf "FILE=%s\\n" "$f"; nl -ba "$f"; done' _ "$CONFIG" "$BUILDCONF" || overall=1
record_to disk_capacity "$IDENTITY/disk_capacity.txt" df -h "$REPO" /var/tmp || overall=1
record_to inode_capacity "$IDENTITY/inode_capacity.txt" df -i "$REPO" || overall=1
record_to assert_new_paths_fresh "$RAW/assert_new_paths_fresh.stdout.txt" bash -o pipefail -c \
    'for p in "$@"; do test ! -e "$p" || { printf "EXISTS=%s\\n" "$p"; exit 1; }; done' _ \
    "$REPO/tmp/R9C/seg2/gbs-source-c2" \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260806-c2" \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2" \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260806-c2" \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2" \
    "$REPO/artifacts/rebuild_2218_c2" \
    "$REPO/artifacts/quarantine/20260806_pre_c2" || overall=1
record_to old_artifact_file_inventory "$COMPARISONS/old_artifact_file_inventory.tsv" bash -o pipefail -c \
    'printf "path\\tsize\\n"; find "$1" -type f -printf "%p\\t%s\\n" | LC_ALL=C sort' _ "$REPO/artifacts/rebuild_2218" || overall=1
record_to old_rpm_sha256 "$COMPARISONS/old_rpm_sha256.txt" bash -o pipefail -c \
    'find "$1" -type f -name "*.rpm" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$REPO/artifacts/rebuild_2218" || overall=1
record_to t4_gate_inventory "$IDENTITY/t4_gate_inventory.tsv" cp -a "$REPO/progress/T4/gate_inventory.tsv" "$IDENTITY/t4_gate_inventory.tsv" || overall=1
record_to t4_tool_sha256 "$IDENTITY/t4_tool_sha256.txt" sha256sum "$REPO/gates/tools/run_static_gates.sh" \
    /home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/run_runtime_gates.sh || overall=1
record_to first_segment_verdict "$IDENTITY/segment1_verdict.txt" bash -o pipefail -c \
    'cat "$1"; rg -n "NO_BUILD_IMPACT" "$2"' _ "$REPO/progress/R9C/build_impact_verdict.tsv" "$REPO/progress/R9C/R9C_segment1_report.md" || overall=1
record_to platform_vs_local_archival_diff "$IDENTITY/platform_vs_local.git_archival.diff" cp -a \
    "$REPO/progress/R9C/inputs/platform_vs_local.git_archival.diff" "$IDENTITY/platform_vs_local.git_archival.diff" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
