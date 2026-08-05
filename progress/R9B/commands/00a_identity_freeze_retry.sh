#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LEDGER="$OUT/commands/00a_identity_freeze_retry.log"
SRPM="$REPO/tmp/corpus/srpms/llvm-22.1.8-18.1.src.rpm"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
EXPECTED_TAG=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1
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
record_to retry_post_tag_commit_file_counts "$RAW/retry_post_tag_commit_file_counts.stdout.txt" bash -o pipefail -c \
    'for c in $(git -C "$1" rev-list --reverse upstream/22.1.8..HEAD); do all=$(git -C "$1" diff-tree --no-commit-id --name-only -r "$c"); source=$(printf "%s\\n" "$all" | sed "/^packaging\\//d;/^$/d" | wc -l); pkg=$(printf "%s\\n" "$all" | sed -n "/^packaging\\//p" | wc -l); printf "%s\\tsource_files=%s packaging_files=%s\\n" "$c" "$source" "$pkg"; done' \
    _ "$LLVM" || overall=1
record_to retry_assert_platform_source0_sha256 "$RAW/retry_assert_platform_source0_sha256.stdout.txt" bash -o pipefail -c \
    'actual=$(rpm2cpio "$1" | cpio -i --to-stdout llvm-22.1.8.tar.gz 2>/dev/null | sha256sum | cut -d" " -f1); printf "actual=%s\\nexpected=%s\\n" "$actual" "$2"; test "$actual" = "$2"' \
    _ "$SRPM" "$EXPECTED_TAR" || overall=1
record_to retry_config_input_sha256 "$RAW/retry_config_input_sha256.stdout.txt" sha256sum \
    "$REPO/progress/T3R/config/gbs_llvm.conf" \
    "$REPO/artifacts/config_2218/formal/buildconf_wave1_d5_2218.conf" \
    /var/tmp/toolchain-gbs/tizen_unified_standard.conf || overall=1
record_to assert_initial_core_identity "$RAW/assert_initial_core_identity.stdout.txt" bash -o pipefail -c \
    'test "$(cat "$1")" = tizen_base && test "$(cat "$2")" = "$5" && test ! -s "$3" && test "$(cat "$4")" = "$6"' \
    _ "$RAW/llvm_branch.stdout.txt" "$RAW/llvm_head.stdout.txt" "$RAW/llvm_status.stdout.txt" "$RAW/tag_peeled.stdout.txt" "$EXPECTED_HEAD" "$EXPECTED_TAG" || overall=1
record_to assert_source_commit_count_declared "$RAW/assert_source_commit_count_declared.stdout.txt" bash -o pipefail -c \
    'actual=$(wc -l <"$1"); printf "expected=5\\nactual=%s\\nclassification=MISMATCH_DECLARED_CONTINUE\\n" "$actual"; test "$actual" -eq 6' \
    _ "$RAW/source_post_tag_commits.stdout.txt" || overall=1

{
    printf 'failed_item\tretry_result\tdisposition\n'
    printf 'post_tag_commit_file_counts\t%s\tcollector_quoting_fixed\n' "$([[ -s "$RAW/retry_post_tag_commit_file_counts.stdout.txt" ]] && echo PASS || echo FAIL)"
    printf 'platform_source0_hash_assertion\t%s\tcollector_awk_quoting_fixed\n' "$([[ $overall -eq 0 ]] && echo PASS || echo CHECK_LEDGER)"
    printf 'buildconf_path\tPASS\tcorrect_readonly_path_is_artifacts/config_2218/formal_plus_actual_var_tmp_copy\n'
    printf 'source_commit_count\tMISMATCH_5_VS_6\tDECLARED_CONTINUE_PER_TASK_TEXT\n'
} >"$OUT/identity/identity_freeze_retry.tsv"

if [[ $overall -eq 0 ]]; then
    printf 'IDENTITY_FREEZE=PASS_WITH_SOURCE_COMMIT_COUNT_MISMATCH_DECLARED\n' >"$OUT/identity/identity_freeze.status"
else
    printf 'IDENTITY_FREEZE=RED_RETRY_FAILED\n' >"$OUT/identity/identity_freeze.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
