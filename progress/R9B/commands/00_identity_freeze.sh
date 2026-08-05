#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LEDGER="$OUT/commands/00_identity_freeze.log"
SRPM="$REPO/tmp/corpus/srpms/llvm-22.1.8-18.1.src.rpm"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
EXPECTED_TAG=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1
EXPECTED_TAR=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e

mkdir -p "$RAW" "$OUT/identity"
: >"$LEDGER"

record_to() {
    local label=$1
    local outfile=$2
    shift 2
    {
        printf 'LABEL=%s\n' "$label"
        printf 'PWD=%s\n' "$PWD"
        printf 'COMMAND='
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
record_to project_status_before "$RAW/project_status_before.stdout.txt" git status --short || overall=1
record_to llvm_branch "$RAW/llvm_branch.stdout.txt" git -C "$LLVM" branch --show-current || overall=1
record_to llvm_head "$RAW/llvm_head.stdout.txt" git -C "$LLVM" rev-parse HEAD || overall=1
record_to llvm_status "$RAW/llvm_status.stdout.txt" git -C "$LLVM" status --porcelain=v1 || overall=1
record_to assert_llvm_identity "$RAW/assert_llvm_identity.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" branch --show-current)" = tizen_base && test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' \
    _ "$LLVM" "$EXPECTED_HEAD" || overall=1
record_to assert_backport_at_head "$RAW/assert_backport_at_head.stdout.txt" git -C "$LLVM" merge-base --is-ancestor "$EXPECTED_HEAD" HEAD || overall=1
record_to backport_show "$RAW/backport_show.stdout.txt" git -C "$LLVM" show -s --format=fuller "$EXPECTED_HEAD" || overall=1

record_to all_post_tag_commits "$RAW/all_post_tag_commits.stdout.txt" git -C "$LLVM" log --format='%H%x09%s' --reverse upstream/22.1.8..HEAD || overall=1
record_to source_post_tag_commits "$RAW/source_post_tag_commits.stdout.txt" git -C "$LLVM" log --format='%H%x09%s' --reverse upstream/22.1.8..HEAD -- . ':(exclude)packaging/**' || overall=1
record_to packaging_only_post_tag_commits "$RAW/packaging_only_post_tag_commits.stdout.txt" bash -o pipefail -c \
    'comm -23 <(git -C "$1" rev-list --reverse upstream/22.1.8..HEAD | sort) <(git -C "$1" log --format=%H upstream/22.1.8..HEAD -- . ":(exclude)packaging/**" | sort)' \
    _ "$LLVM" || overall=1
record_to post_tag_commit_file_counts "$RAW/post_tag_commit_file_counts.stdout.txt" bash -o pipefail -c \
    'for c in $(git -C "$1" rev-list --reverse upstream/22.1.8..HEAD); do printf "%s\\t" "$c"; git -C "$1" diff-tree --no-commit-id --name-only -r "$c" | awk '\''BEGIN{s=0;p=0}{if ($0 ~ /^packaging\\//) p++; else s++}END{printf "source_files=%d packaging_files=%d\\n",s,p}'\''; done' \
    _ "$LLVM" || overall=1

record_to tag_show_ref "$RAW/tag_show_ref.stdout.txt" git -C "$LLVM" show-ref --dereference refs/tags/upstream/22.1.8 || overall=1
record_to tag_peeled "$RAW/tag_peeled.stdout.txt" git -C "$LLVM" rev-parse 'upstream/22.1.8^{}' || overall=1
record_to assert_tag_peeled "$RAW/assert_tag_peeled.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse "upstream/22.1.8^{}")" = "$2"' _ "$LLVM" "$EXPECTED_TAG" || overall=1
record_to platform_source0_sha256 "$RAW/platform_source0_sha256.stdout.txt" bash -o pipefail -c \
    'rpm2cpio "$1" | cpio -i --to-stdout llvm-22.1.8.tar.gz 2>/dev/null | sha256sum' _ "$SRPM" || overall=1
record_to assert_platform_source0_sha256 "$RAW/assert_platform_source0_sha256.stdout.txt" bash -o pipefail -c \
    'test "$(rpm2cpio "$1" | cpio -i --to-stdout llvm-22.1.8.tar.gz 2>/dev/null | sha256sum | awk "{print \\$1}")" = "$2"' \
    _ "$SRPM" "$EXPECTED_TAR" || overall=1

record_to packaging_input_sha256_before "$RAW/packaging_input_sha256_before.stdout.txt" sha256sum \
    "$REPO/packaging/libcxx-runtimes.spec" "$REPO/packaging/libcxx-llvm22.map" \
    "$REPO/packaging/libcxx-runtimes.manifest" "$REPO/packaging/SOURCE_PROVENANCE" || overall=1
record_to config_input_sha256 "$RAW/config_input_sha256.stdout.txt" sha256sum \
    "$REPO/progress/T3R/config/gbs_llvm.conf" "$REPO/config_2218/formal/buildconf_wave1_d5_2218.conf" || overall=1
record_to existing_artifact_dirs "$RAW/existing_artifact_dirs.stdout.txt" find "$REPO/artifacts" -maxdepth 1 -mindepth 1 -type d -printf '%f\n' || overall=1
record_to buildroot_freshness "$RAW/buildroot_freshness.stdout.txt" bash -o pipefail -c \
    'for a in armv7l aarch64 x86_64; do r="$1/tmp/GBS-ROOT/LIBCXX-2218-${a}-20260805-std"; if test -e "$r"; then echo "EXISTS:$r"; exit 1; else echo "FRESH:$r"; fi; done' \
    _ "$REPO" || overall=1

all_count=$(wc -l <"$RAW/all_post_tag_commits.stdout.txt")
source_count=$(wc -l <"$RAW/source_post_tag_commits.stdout.txt")
{
    printf 'item\texpected\tactual\tstatus\n'
    printf 'branch\ttizen_base\t%s\t%s\n' "$(cat "$RAW/llvm_branch.stdout.txt")" "$([[ $(cat "$RAW/llvm_branch.stdout.txt") == tizen_base ]] && echo PASS || echo FAIL)"
    printf 'head\t%s\t%s\t%s\n' "$EXPECTED_HEAD" "$(cat "$RAW/llvm_head.stdout.txt")" "$([[ $(cat "$RAW/llvm_head.stdout.txt") == "$EXPECTED_HEAD" ]] && echo PASS || echo FAIL)"
    printf 'worktree_status\tzero_output\t%s_bytes\t%s\n' "$(wc -c <"$RAW/llvm_status.stdout.txt")" "$([[ ! -s "$RAW/llvm_status.stdout.txt" ]] && echo PASS || echo FAIL)"
    printf 'all_post_tag_commits\tNA\t%s\tRECORDED\n' "$all_count"
    printf 'source_post_tag_commits\t5\t%s\t%s\n' "$source_count" "$([[ $source_count -eq 5 ]] && echo PASS || echo MISMATCH_DECLARED_CONTINUE)"
    printf 'tag_peeled\t%s\t%s\t%s\n' "$EXPECTED_TAG" "$(cat "$RAW/tag_peeled.stdout.txt")" "$([[ $(cat "$RAW/tag_peeled.stdout.txt") == "$EXPECTED_TAG" ]] && echo PASS || echo FAIL)"
    printf 'platform_tag_archive_sha256\t%s\t%s\t%s\n' "$EXPECTED_TAR" "$(awk '{print $1}' "$RAW/platform_source0_sha256.stdout.txt")" "$([[ $(awk '{print $1}' "$RAW/platform_source0_sha256.stdout.txt") == "$EXPECTED_TAR" ]] && echo PASS || echo FAIL)"
} >"$OUT/identity/identity_freeze.tsv"

if [[ $overall -ne 0 ]]; then
    printf 'IDENTITY_FREEZE=RED\n' >"$OUT/identity/identity_freeze.status"
else
    printf 'IDENTITY_FREEZE=PASS_WITH_SOURCE_COMMIT_COUNT_MISMATCH_DECLARED\n' >"$OUT/identity/identity_freeze.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
