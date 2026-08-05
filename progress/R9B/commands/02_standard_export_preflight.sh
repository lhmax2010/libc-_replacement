#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
WORKTREE="$REPO/tmp/R9B/gbs-source-std"
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std"
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LOG="$OUT/build/standard_export_preflight.full.log"
LEDGER="$OUT/commands/02_standard_export_preflight.log"
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

cd "$WORKTREE" || exit 125
overall=0
record_to assert_preflight_root_fresh "$RAW/assert_preflight_root_fresh.stdout.txt" test ! -e "$ROOT" || exit 91
record_to assert_preflight_worktree_shape "$RAW/assert_preflight_worktree_shape.stdout.txt" bash -o pipefail -c \
    'test "$(git status --porcelain=v1)" = "?? r9b-packaging/" && test -z "$(git diff --name-only)" && test "$(find r9b-packaging -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1' || exit 92

CMD=(gbs -c "$CONFIG" build
    -A armv7l
    -B "$ROOT"
    -C
    --debug
    --style git
    --include-all
    --packaging-dir r9b-packaging
    --spec libcxx-runtimes.spec
    --export-only
    "$WORKTREE")
{
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'PURPOSE=GBS_SINGLE_TARGET_AND_STANDARD_EXPORT_PREFLIGHT\n'
    printf 'COMMAND_BEGIN\n'
    printf '%q ' "${CMD[@]}"
    printf '\nCOMMAND_END\n'
} >"$LOG"
"${CMD[@]}" >>"$LOG" 2>&1
rc=$?
printf 'EXIT_CODE=%s\n' "$rc" >>"$LOG"
printf 'LABEL=gbs_standard_export_preflight\nPWD=%s\nCOMMAND=' "$PWD" >>"$LEDGER"
printf '%q ' "${CMD[@]}" >>"$LEDGER"
printf '\nEXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
[[ $rc -eq 0 ]] || overall=1

SOURCE_EXPORT=$(find "$ROOT/local/sources" -type d -path '*/libcxx-runtimes-22.1.8-1' -print -quit 2>/dev/null)
if [[ -z "$SOURCE_EXPORT" ]]; then
    printf 'NOT_FOUND\n' >"$RAW/preflight_source_export_dir.stdout.txt"
    overall=1
else
    printf '%s\n' "$SOURCE_EXPORT" >"$RAW/preflight_source_export_dir.stdout.txt"
fi

record_to assert_single_target "$RAW/preflight_assert_single_target.stdout.txt" bash -o pipefail -c \
    'rg -q "there is no circle in 1 packages" "$1" && ! rg -q "circle found|in [2-9][0-9]* packages" "$1"' _ "$LOG" || overall=1
record_to assert_standard_export_option "$RAW/preflight_assert_standard_export_option.stdout.txt" bash -o pipefail -c \
    'rg -q "gbs --debug export" "$1" && rg -q -- "--packaging-dir=r9b-packaging" "$1" && ! rg -q -- "--no-patch-export" "$1"' _ "$LOG" || overall=1
record_to preflight_target_lines "$RAW/preflight_target_lines.stdout.txt" rg -n \
    'start building packages from|there is no circle|building libcxx-runtimes|gbs --debug export|--git-patch-export|circle found' "$LOG" || overall=1
record_to preflight_export_file_list "$RAW/preflight_export_file_list.stdout.txt" find "$SOURCE_EXPORT" -maxdepth 1 -type f -printf '%f\t%s\n' || overall=1
record_to preflight_export_sha256 "$RAW/preflight_export_sha256.stdout.txt" bash -o pipefail -c \
    'find "$1" -maxdepth 1 -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$SOURCE_EXPORT" || overall=1
record_to assert_source0_identity "$RAW/preflight_assert_source0_identity.stdout.txt" bash -o pipefail -c \
    'actual=$(sha256sum "$1/llvm-22.1.8.tar.gz" | cut -d" " -f1); printf "actual=%s\\nexpected=%s\\n" "$actual" "$2"; test "$actual" = "$2"' \
    _ "$SOURCE_EXPORT" "$EXPECTED_TAR" || overall=1
record_to assert_patch_count_and_ehabi "$RAW/preflight_assert_patch_count_and_ehabi.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1" -maxdepth 1 -type f -name "0*.patch*" | wc -l)" -eq 6 && test "$(find "$1" -maxdepth 1 -type f -name "0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa-personal.patch" | wc -l)" -eq 1 && rg -q "^Patch5:[[:space:]]+0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa-personal.patch$" "$1/libcxx-runtimes.spec" && rg -q "^%patch5 -p1$" "$1/libcxx-runtimes.spec"' \
    _ "$SOURCE_EXPORT" || overall=1
record_to exported_spec_patch_lines "$RAW/exported_spec_patch_lines.stdout.txt" rg -n '^# Patches auto-generated|^Patch[0-9]+:|^# 000[0-9]-|^%patch[0-9]+ ' "$SOURCE_EXPORT/libcxx-runtimes.spec" || overall=1
record_to exported_patch_headers "$RAW/exported_patch_headers.stdout.txt" bash -o pipefail -c \
    'for p in "$1"/0*.patch*; do echo "FILE=$(basename "$p")"; case "$p" in *.xz) xz -dc "$p";; *.gz) gzip -dc "$p";; *.bz2) bzip2 -dc "$p";; *) cat "$p";; esac | sed -n "1,16p"; done' _ "$SOURCE_EXPORT" || overall=1

if [[ -d "$ROOT" ]]; then
    {
        printf 'RETIRED_SINGLE_USE\n'
        printf 'purpose=standard_export_preflight_only\n'
        printf 'gbs_exit_code=%s\n' "$rc"
        printf 'root=%s\n' "$ROOT"
    } >"$ROOT/RETIRED_SINGLE_USE"
fi
record_to preflight_root_retired "$RAW/preflight_root_retired.stdout.txt" cat "$ROOT/RETIRED_SINGLE_USE" || overall=1

if [[ $overall -eq 0 ]]; then
    printf 'STANDARD_EXPORT_PREFLIGHT=PASS_SINGLE_PACKAGE_SOURCE0_AND_6_PATCHES\n' >"$OUT/build/standard_export_preflight.status"
else
    printf 'STANDARD_EXPORT_PREFLIGHT=RED_STOP\n' >"$OUT/build/standard_export_preflight.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
