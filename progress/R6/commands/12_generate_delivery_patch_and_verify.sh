#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/codes/llvm"
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
DELIVERY="$OUT/delivery"
UPSTREAM="$OUT/upstream/8119f1854948b50358bbfaea08f207f51970f06c.github.patch"
FORMAT_PATCH="$DELIVERY/0001-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personality.patch"
LEDGER="$OUT/commands/12_generate_delivery_patch_and_verify.log"
mkdir -p "$DELIVERY"
: >"$LEDGER"

run_record() {
  local label=$1
  shift
  {
    printf 'LABEL=%s\n' "$label"
    printf 'PWD=%s\n' "$PWD"
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$RAW/$label.stdout.txt" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

overall=0
cd "$SOURCE" || exit 125
run_record delivery_source_branch git branch --show-current || overall=1
run_record delivery_source_head git rev-parse HEAD || overall=1
run_record delivery_source_status git status --porcelain=v1 || overall=1
run_record assert_delivery_identity bash -o pipefail -c 'test "$(git branch --show-current)" = tizen_base && test "$(git rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git status --porcelain=v1)"' || overall=1
run_record delivery_source_remote git remote -v || overall=1
run_record assert_gerrit_origin bash -o pipefail -c 'git remote get-url origin | rg -q -- "review.tizen.org.*platform/upstream/llvm"' || overall=1
run_record local_unpushed_commit git log --oneline --decorate origin/tizen_base..tizen_base || overall=1
run_record full_backport_commit git show --format=fuller --stat --patch HEAD || overall=1
run_record format_patch git format-patch -1 --stdout HEAD || overall=1
run_record write_format_patch cp "$RAW/format_patch.stdout.txt" "$FORMAT_PATCH" || overall=1
run_record delivery_sha256 sha256sum "$UPSTREAM" "$FORMAT_PATCH" "$DELIVERY/suggested_gerrit_commit_message.md" || overall=1
run_record upstream_patch_id bash -o pipefail -c 'git patch-id --stable <"$1"' _ "$UPSTREAM" || overall=1
run_record backport_patch_id bash -o pipefail -c 'git patch-id --stable <"$1"' _ "$FORMAT_PATCH" || overall=1
run_record assert_patch_ids_equal bash -o pipefail -c 'test "$(cut -d" " -f1 "$1")" = "$(cut -d" " -f1 "$2")"' _ "$RAW/upstream_patch_id.stdout.txt" "$RAW/backport_patch_id.stdout.txt" || overall=1
run_record format_patch_stat git apply --numstat "$FORMAT_PATCH" || overall=1
run_record assert_format_patch_scope bash -o pipefail -c 'test "$(git apply --numstat "$1")" = $'\''2\t0\tlibcxxabi/src/cxa_personality.cpp'\''' _ "$FORMAT_PATCH" || overall=1
run_record suggested_message cat "$DELIVERY/suggested_gerrit_commit_message.md" || overall=1

{
  printf 'EXTERNAL_REPOSITORY=%s\n' "$(git remote get-url origin)"
  printf 'EXTERNAL_PUSH_EXECUTED=NO\n'
  printf 'LOCAL_BRANCH=tizen_base\n'
  printf 'LOCAL_COMMIT=%s\n' "$(git rev-parse HEAD)"
  printf 'UPLOAD_OWNER=HUMAN\n'
  printf 'FORMAT_PATCH=%s\n' "$FORMAT_PATCH"
} >"$OUT/no_external_push_declaration.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
