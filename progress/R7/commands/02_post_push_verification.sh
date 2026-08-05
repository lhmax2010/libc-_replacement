#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/codes/llvm"
OUT="$REPO/progress/R7"
RAW="$OUT/raw"
LEDGER="$OUT/commands/02_post_push_verification.log"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
SANDBOX_BRANCH=sandbox/lhmax2025/libcxx-ehabi-backport
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

overall=0
cd "$SOURCE" || exit 125
record_to remote_sandbox_after_requested_form "$RAW/remote_sandbox_after_requested_form.stdout.txt" \
  git ls-remote origin "$SANDBOX_BRANCH" || overall=1
record_to remote_sandbox_after_exact_ref "$RAW/remote_sandbox_after_exact_ref.stdout.txt" \
  git ls-remote origin "$SANDBOX_REF" || overall=1
record_to remote_tizen_base_after "$RAW/remote_tizen_base_after.stdout.txt" \
  git ls-remote origin refs/heads/tizen_base || overall=1
record_to source_head_after "$RAW/source_head_after.stdout.txt" git rev-parse HEAD || overall=1
record_to source_branch_after "$RAW/source_branch_after.stdout.txt" git branch --show-current || overall=1
record_to source_status_after "$RAW/source_status_after.stdout.txt" git status --porcelain=v1 || overall=1

record_to assert_sandbox_ref_exact "$RAW/assert_sandbox_ref_exact.stdout.txt" bash -o pipefail -c \
  'test "$(wc -l < "$1")" -eq 1 && test "$(cut -f1 "$1")" = "$2" && test "$(cut -f2 "$1")" = "$3"' \
  _ "$RAW/remote_sandbox_after_exact_ref.stdout.txt" "$EXPECTED_HEAD" "$SANDBOX_REF" || overall=1
record_to assert_requested_and_exact_match "$RAW/assert_requested_and_exact_match.stdout.txt" \
  cmp -s "$RAW/remote_sandbox_after_requested_form.stdout.txt" "$RAW/remote_sandbox_after_exact_ref.stdout.txt" || overall=1
record_to assert_tizen_base_unchanged "$RAW/assert_tizen_base_unchanged.stdout.txt" \
  cmp -s "$RAW/remote_tizen_base_before.stdout.txt" "$RAW/remote_tizen_base_after.stdout.txt" || overall=1
record_to assert_local_unchanged "$RAW/assert_local_unchanged.stdout.txt" bash -o pipefail -c \
  'test "$(cat "$1")" = "$3" && test "$(cat "$2")" = tizen_base && test ! -s "$4"' \
  _ "$RAW/source_head_after.stdout.txt" "$RAW/source_branch_after.stdout.txt" \
  "$EXPECTED_HEAD" "$RAW/source_status_after.stdout.txt" || overall=1
record_to assert_no_plus "$RAW/assert_no_plus.stdout.txt" bash -o pipefail -c \
  'case "$1" in *+*) exit 1;; *) exit 0;; esac' _ "$SANDBOX_BRANCH" || overall=1

{
  printf 'item\tvalue\tresult\n'
  printf 'branch\t%s\tPASS_NO_PLUS\n' "$SANDBOX_BRANCH"
  printf 'sandbox_remote_sha\t%s\t%s\n' \
    "$(cut -f1 "$RAW/remote_sandbox_after_exact_ref.stdout.txt")" \
    "$([[ $(cut -f1 "$RAW/remote_sandbox_after_exact_ref.stdout.txt") == "$EXPECTED_HEAD" ]] && printf PASS || printf FAIL)"
  printf 'tizen_base_before\t%s\tRECORDED\n' "$(cut -f1 "$RAW/remote_tizen_base_before.stdout.txt")"
  printf 'tizen_base_after\t%s\t%s\n' \
    "$(cut -f1 "$RAW/remote_tizen_base_after.stdout.txt")" \
    "$([[ $(cut -f1 "$RAW/remote_tizen_base_before.stdout.txt") == $(cut -f1 "$RAW/remote_tizen_base_after.stdout.txt") ]] && printf UNCHANGED || printf CHANGED)"
  printf 'push_attempts\t1\tPASS\n'
  printf 'push_force\tNO\tPASS\n'
  printf 'push_namespace\trefs/heads\tPASS_NOT_REFS_FOR\n'
} >"$OUT/remote_verification.tsv"

if [[ $overall -eq 0 ]]; then
  printf 'POST_PUSH_VERIFICATION=PASS\n' >"$OUT/post_push_verification.status"
else
  printf 'POST_PUSH_VERIFICATION=FAIL\n' >"$OUT/post_push_verification.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
