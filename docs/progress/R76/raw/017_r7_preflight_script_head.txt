#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/codes/llvm"
OUT="$REPO/progress/R7"
RAW="$OUT/raw"
LEDGER="$OUT/commands/00_pre_push_verification.log"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
EXPECTED_PATCH_ID=ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479
EXPECTED_REMOTE=ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm
SANDBOX_REF=refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport

mkdir -p "$RAW"
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

overall=0
cd "$SOURCE" || exit 125

record_to source_pwd "$RAW/source_pwd.stdout.txt" pwd || overall=1
record_to source_branch "$RAW/source_branch.stdout.txt" git branch --show-current || overall=1
record_to source_head "$RAW/source_head.stdout.txt" git rev-parse HEAD || overall=1
record_to source_status_porcelain "$RAW/source_status_porcelain.stdout.txt" git status --porcelain=v1 || overall=1
record_to source_remote_v "$RAW/source_remote_v.stdout.txt" git remote -v || overall=1
record_to source_remote_url "$RAW/source_remote_url.stdout.txt" git remote get-url origin || overall=1
record_to fetch_origin_tizen_base "$RAW/fetch_origin_tizen_base.stdout.txt" git fetch origin tizen_base || overall=1
record_to remote_tizen_base_before "$RAW/remote_tizen_base_before.stdout.txt" git ls-remote origin refs/heads/tizen_base || overall=1
record_to remote_sandbox_before "$RAW/remote_sandbox_before.stdout.txt" git ls-remote origin "$SANDBOX_REF" || overall=1
record_to origin_tizen_base_sha "$RAW/origin_tizen_base_sha.stdout.txt" git rev-parse origin/tizen_base || overall=1
record_to ahead_count "$RAW/ahead_count.stdout.txt" git rev-list --count origin/tizen_base..HEAD || overall=1
record_to behind_count "$RAW/behind_count.stdout.txt" git rev-list --count HEAD..origin/tizen_base || overall=1
record_to ahead_commit_list "$RAW/ahead_commit_list.stdout.txt" git log --oneline --decorate origin/tizen_base..HEAD || overall=1
record_to origin_is_ancestor "$RAW/origin_is_ancestor.stdout.txt" git merge-base --is-ancestor origin/tizen_base HEAD || overall=1
record_to patch_id "$RAW/patch_id.stdout.txt" bash -o pipefail -c \
  'git show --pretty=email --patch "$1" | git patch-id --stable' _ "$EXPECTED_HEAD" || overall=1
record_to commit_message "$RAW/commit_message.stdout.txt" git log -1 --format=%B "$EXPECTED_HEAD" || overall=1
record_to commit_show "$RAW/commit_show.stdout.txt" git show --format=fuller --stat --patch "$EXPECTED_HEAD" || overall=1
record_to cxa_personality_sha256 "$RAW/cxa_personality_sha256.stdout.txt" sha256sum \
  libcxxabi/src/cxa_personality.cpp || overall=1

record_to assert_branch "$RAW/assert_branch.stdout.txt" test "$(cat "$RAW/source_branch.stdout.txt")" = tizen_base || overall=1
record_to assert_head "$RAW/assert_head.stdout.txt" test "$(cat "$RAW/source_head.stdout.txt")" = "$EXPECTED_HEAD" || overall=1
record_to assert_clean "$RAW/assert_clean.stdout.txt" test ! -s "$RAW/source_status_porcelain.stdout.txt" || overall=1
record_to assert_remote "$RAW/assert_remote.stdout.txt" test "$(cat "$RAW/source_remote_url.stdout.txt")" = "$EXPECTED_REMOTE" || overall=1
record_to assert_one_ahead_zero_behind "$RAW/assert_one_ahead_zero_behind.stdout.txt" bash -o pipefail -c \
  'test "$(cat "$1")" = 1 && test "$(cat "$2")" = 0' _ \
  "$RAW/ahead_count.stdout.txt" "$RAW/behind_count.stdout.txt" || overall=1
record_to assert_patch_id "$RAW/assert_patch_id.stdout.txt" bash -o pipefail -c \
  'test "$(cut -d" " -f1 "$1")" = "$2"' _ "$RAW/patch_id.stdout.txt" "$EXPECTED_PATCH_ID" || overall=1
record_to assert_change_id "$RAW/assert_change_id.stdout.txt" rg -n \
  '^Change-Id: I[0-9a-f]+$' "$RAW/commit_message.stdout.txt" || overall=1
record_to assert_upstream_status "$RAW/assert_upstream_status.stdout.txt" rg -n \
  '^Upstream-Status: Backport \[8119f1854948b50358bbfaea08f207f51970f06c\]$' \
  "$RAW/commit_message.stdout.txt" || overall=1
record_to assert_link "$RAW/assert_link.stdout.txt" rg -n \
  '^Link: https://github.com/llvm/llvm-project/pull/189787$' "$RAW/commit_message.stdout.txt" || overall=1
record_to assert_cxa_hash "$RAW/assert_cxa_hash.stdout.txt" bash -o pipefail -c \
  'test "$(cut -d" " -f1 "$1")" = 55e136184adbaeee078a339afde2d6da103f4af7cb7c5c31926f2911127eff03' \
  _ "$RAW/cxa_personality_sha256.stdout.txt" || overall=1
record_to assert_remote_tracking_matches_remote "$RAW/assert_remote_tracking_matches_remote.stdout.txt" bash -o pipefail -c \
  'test "$(cut -f1 "$1")" = "$(cat "$2")"' _ \
  "$RAW/remote_tizen_base_before.stdout.txt" "$RAW/origin_tizen_base_sha.stdout.txt" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'PRE_PUSH_VERIFICATION=PASS\n' >"$OUT/pre_push_verification.status"
else
  printf 'PRE_PUSH_VERIFICATION=FAIL\n' >"$OUT/pre_push_verification.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
