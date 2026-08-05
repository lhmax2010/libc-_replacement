#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/codes/llvm"
OUT="$REPO/progress/R7"
RAW="$OUT/raw"
LEDGER="$OUT/commands/01_push_sandbox.log"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
EXPECTED_PATCH_ID=ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479
EXPECTED_REMOTE=ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm
SANDBOX_REF=refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport

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

record_combined() {
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
  "$@" >"$outfile" 2>&1
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

cd "$SOURCE" || exit 125
preflight_ok=0
record_to assert_preflight_pass "$RAW/push_assert_preflight_pass.stdout.txt" \
  rg -q '^PRE_PUSH_VERIFICATION=PASS$' "$OUT/pre_push_verification.status" || preflight_ok=1
record_to push_recheck_branch "$RAW/push_recheck_branch.stdout.txt" bash -o pipefail -c \
  'test "$(git branch --show-current)" = tizen_base' || preflight_ok=1
record_to push_recheck_head "$RAW/push_recheck_head.stdout.txt" bash -o pipefail -c \
  'test "$(git rev-parse HEAD)" = "$1"' _ "$EXPECTED_HEAD" || preflight_ok=1
record_to push_recheck_clean "$RAW/push_recheck_clean.stdout.txt" bash -o pipefail -c \
  'test -z "$(git status --porcelain=v1)"' || preflight_ok=1
record_to push_recheck_remote "$RAW/push_recheck_remote.stdout.txt" bash -o pipefail -c \
  'test "$(git remote get-url origin)" = "$1"' _ "$EXPECTED_REMOTE" || preflight_ok=1
record_to push_recheck_patch_id "$RAW/push_recheck_patch_id.stdout.txt" bash -o pipefail -c \
  'test "$(git show --pretty=email --patch "$1" | git patch-id --stable | cut -d" " -f1)" = "$2"' \
  _ "$EXPECTED_HEAD" "$EXPECTED_PATCH_ID" || preflight_ok=1
record_to push_recheck_tizen_base "$RAW/push_recheck_tizen_base.stdout.txt" bash -o pipefail -c \
  'test "$(git rev-parse origin/tizen_base)" = "$(cut -f1 "$1")"' \
  _ "$RAW/remote_tizen_base_before.stdout.txt" || preflight_ok=1

if [[ $preflight_ok -ne 0 ]]; then
  printf 'PUSH_STATUS=NOT_ATTEMPTED_PREFLIGHT_RECHECK_FAILED\n' >"$OUT/push.status"
  printf 'OVERALL_EXIT_CODE=90\n' >>"$LEDGER"
  exit 90
fi

record_combined push_attempt_1 "$RAW/push_attempt_1.combined.txt" \
  git push origin "HEAD:$SANDBOX_REF"
push_rc=$?
if [[ $push_rc -ne 0 ]]; then
  printf 'PUSH_STATUS=FAILED\nATTEMPTS=1\nEXIT_CODE=%s\n' "$push_rc" >"$OUT/push.status"
  printf 'OVERALL_EXIT_CODE=%s\n' "$push_rc" >>"$LEDGER"
  exit "$push_rc"
fi

printf 'PUSH_STATUS=SUCCESS\nATTEMPTS=1\nEXIT_CODE=0\n' >"$OUT/push.status"
printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
exit 0
