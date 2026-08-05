#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/codes/llvm"
OUT="$REPO/progress/R7"
RAW="$OUT/raw"
LEDGER="$OUT/commands/03_final_audit.log"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b

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
record_to assert_status_files "$RAW/assert_status_files.stdout.txt" bash -o pipefail -c \
  'rg -q "^PRE_PUSH_VERIFICATION=PASS$" "$1" && rg -q "^PUSH_STATUS=SUCCESS$" "$2" && rg -q "^POST_PUSH_VERIFICATION=PASS$" "$3"' \
  _ "$OUT/pre_push_verification.status" "$OUT/push.status" "$OUT/post_push_verification.status" || overall=1
record_to assert_remote_summary "$RAW/assert_remote_summary.stdout.txt" bash -o pipefail -c \
  'rg -q "^sandbox_remote_sha\t3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b\tPASS$" "$1" && rg -q "^tizen_base_after\tfd7a4929c0e424d564cdd1f0c55af9153dcea321\tUNCHANGED$" "$1" && rg -q "^push_namespace\trefs/heads\tPASS_NOT_REFS_FOR$" "$1"' \
  _ "$OUT/remote_verification.tsv" || overall=1
record_to assert_push_command_scope "$RAW/assert_push_command_scope.stdout.txt" bash -o pipefail -c \
  'rg -q "COMMAND=git push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport" "$1" && ! rg -q -- "--force|refs/for/|sandbox/[^ ]*\\+" "$1"' \
  _ "$OUT/commands/01_push_sandbox.log" || overall=1
record_to assert_source_unchanged_final "$RAW/assert_source_unchanged_final.stdout.txt" bash -o pipefail -c \
  'test "$(git branch --show-current)" = tizen_base && test "$(git rev-parse HEAD)" = "$1" && test -z "$(git status --porcelain=v1)"' \
  _ "$EXPECTED_HEAD" || overall=1
record_to assert_report_content "$RAW/assert_report_content.stdout.txt" rg -n \
  '状态：`PASS`|sandbox/lhmax2025/libcxx-ehabi-backport|tizen_base.*UNCHANGED|QuickBuild' \
  "$OUT/sandbox_push_report.md" || overall=1
record_to manifest_without_self_and_live_ledger "$OUT/MANIFEST.sha256" bash -o pipefail -c \
  'find "$1" -type f ! -path "$1/MANIFEST.sha256" ! -path "$1/commands/03_final_audit.log" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' \
  _ "$OUT" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
