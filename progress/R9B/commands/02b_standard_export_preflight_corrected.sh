#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
WORKTREE="$REPO/tmp/R9B/gbs-source-std-r2"
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2"
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LOG="$OUT/build/standard_export_preflight_corrected.full.log"
LEDGER="$OUT/commands/02b_standard_export_preflight_corrected.log"
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
record_to assert_corrected_preflight_root_fresh "$RAW/assert_corrected_preflight_root_fresh.stdout.txt" test ! -e "$ROOT" || exit 91
record_to corrected_preflight_worktree_status "$RAW/corrected_preflight_worktree_status.stdout.txt" git status --porcelain=v1 || overall=1
record_to assert_corrected_preflight_worktree_shape "$RAW/assert_corrected_preflight_worktree_shape.stdout.txt" bash -o pipefail -c \
    'test "$(git rev-parse HEAD)" = "$1" && test "$(find packaging -maxdepth 1 -type f | wc -l)" -eq 5 && test "$(find packaging -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1 && test "$(sha256sum packaging/llvm-22.1.8.tar.gz | cut -d" " -f1)" = "$2" && test "$(git diff --name-only | sed "/^packaging\\//d;/^$/d" | wc -l)" -eq 0' \
    _ 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b "$EXPECTED_TAR" || overall=1
[[ $overall -eq 0 ]] || exit 92

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
    printf 'PURPOSE=CORRECTED_PLATFORM_STANDARD_EXPORT_ONLY_PREFLIGHT\n'
    printf 'COMMAND_BEGIN\n'
    printf '%q ' "${CMD[@]}"
    printf '\nCOMMAND_END\n'
} >"$LOG"
"${CMD[@]}" >>"$LOG" 2>&1
rc=$?
printf 'EXIT_CODE=%s\n' "$rc" >>"$LOG"
printf 'LABEL=gbs_corrected_standard_export_preflight\nPWD=%s\nCOMMAND=' "$PWD" >>"$LEDGER"
printf '%q ' "${CMD[@]}" >>"$LEDGER"
printf '\nEXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
[[ $rc -eq 0 ]] || overall=1

SOURCE_EXPORT=$(find "$ROOT/local/sources" -type d -path '*/libcxx-runtimes-22.1.8-1' -print -quit 2>/dev/null)
if [[ -z "$SOURCE_EXPORT" ]]; then
    printf 'NOT_FOUND\n' >"$RAW/corrected_preflight_source_export_dir.stdout.txt"
    overall=1
else
    printf '%s\n' "$SOURCE_EXPORT" >"$RAW/corrected_preflight_source_export_dir.stdout.txt"
fi

record_to assert_corrected_standard_export_option "$RAW/assert_corrected_standard_export_option.stdout.txt" bash -o pipefail -c \
    'rg -q "gbs --debug export" "$1" && rg -q -- "--packaging-dir=packaging" "$1" && ! rg -q -- "--no-patch-export" "$1"' _ "$LOG" || overall=1
record_to assert_corrected_export_scope "$RAW/assert_corrected_export_scope.stdout.txt" bash -o pipefail -c \
    '! rg -q "circle found|init_buildsystem|Executing\\(%build\\)|BUILD-ROOTS|rpmbuild[[:space:]]+-b" "$1" && test "$(find "$2" -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1 && rg -q "^Name:[[:space:]]+libcxx-runtimes$" "$2/libcxx-runtimes.spec"' _ "$LOG" "$SOURCE_EXPORT" || overall=1
record_to corrected_preflight_target_lines "$RAW/corrected_preflight_target_lines.stdout.txt" rg -n \
    'gbs --debug export|--git-patch-export|--packaging-dir=|circle found|init_buildsystem' "$LOG" || overall=1
record_to corrected_preflight_export_file_list "$RAW/corrected_preflight_export_file_list.stdout.txt" find "$SOURCE_EXPORT" -maxdepth 1 -type f -printf '%f\t%s\n' || overall=1
record_to corrected_preflight_export_sha256 "$RAW/corrected_preflight_export_sha256.stdout.txt" bash -o pipefail -c \
    'find "$1" -maxdepth 1 -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$SOURCE_EXPORT" || overall=1
record_to assert_corrected_source0_identity "$RAW/assert_corrected_source0_identity.stdout.txt" bash -o pipefail -c \
    'actual=$(sha256sum "$1/llvm-22.1.8.tar.gz" | cut -d" " -f1); printf "actual=%s\nexpected=%s\n" "$actual" "$2"; test "$actual" = "$2"' \
    _ "$SOURCE_EXPORT" "$EXPECTED_TAR" || overall=1
record_to assert_corrected_patch_count_and_ehabi "$RAW/assert_corrected_patch_count_and_ehabi.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1" -maxdepth 1 -type f -name "0*.patch*" | wc -l)" -eq 6 && test "$(find "$1" -maxdepth 1 -type f -name "0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch" | wc -l)" -eq 1 && ! find "$1" -maxdepth 1 -type f -name "0001-Add-packaging.patch*" | grep -q . && rg -q "^Patch5:[[:space:]]+0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch$" "$1/libcxx-runtimes.spec" && rg -q "^%patch5 -p1$" "$1/libcxx-runtimes.spec"' \
    _ "$SOURCE_EXPORT" || overall=1
record_to corrected_exported_spec_patch_lines "$RAW/corrected_exported_spec_patch_lines.stdout.txt" rg -n '^# Patches auto-generated|^Patch[0-9]+:|^# 000[0-9]-|^%patch[0-9]+ ' "$SOURCE_EXPORT/libcxx-runtimes.spec" || overall=1
record_to corrected_exported_patch_headers "$RAW/corrected_exported_patch_headers.stdout.txt" bash -o pipefail -c \
    'for p in "$1"/0*.patch*; do echo "FILE=$(basename "$p")"; case "$p" in *.xz) xz -dc "$p";; *.gz) gzip -dc "$p";; *.bz2) bzip2 -dc "$p";; *) sed -n "1,20p" "$p"; continue;; esac | sed -n "1,20p"; done' _ "$SOURCE_EXPORT" || overall=1
record_to corrected_exported_patch_inventory "$RAW/corrected_exported_patch_inventory.stdout.txt" bash -o pipefail -c \
    'printf "patch_file\\tsha256\\tfrom_commit\\tsubject\\n"; for p in "$1"/0*.patch*; do case "$p" in *.xz) h=$(xz -dc "$p" | sed -n "1s/^From \\([0-9a-f]\\{40\\}\\) .*/\\1/p"); s=$(xz -dc "$p" | sed -n "s/^Subject: \\[PATCH[^]]*\\] //p" | head -n1);; *.gz) h=$(gzip -dc "$p" | sed -n "1s/^From \\([0-9a-f]\\{40\\}\\) .*/\\1/p"); s=$(gzip -dc "$p" | sed -n "s/^Subject: \\[PATCH[^]]*\\] //p" | head -n1);; *) h=$(sed -n "1s/^From \\([0-9a-f]\\{40\\}\\) .*/\\1/p" "$p"); s=$(sed -n "s/^Subject: \\[PATCH[^]]*\\] //p" "$p" | head -n1);; esac; printf "%s\\t%s\\t%s\\t%s\\n" "$(basename "$p")" "$(sha256sum "$p" | cut -d" " -f1)" "$h" "$s"; done' _ "$SOURCE_EXPORT" || overall=1

if [[ -d "$ROOT" ]]; then
    {
        printf 'RETIRED_SINGLE_USE\n'
        printf 'purpose=corrected_platform_standard_export_only_preflight\n'
        printf 'gbs_exit_code=%s\n' "$rc"
        printf 'root=%s\n' "$ROOT"
    } >"$ROOT/RETIRED_SINGLE_USE"
fi
record_to corrected_preflight_root_retired "$RAW/corrected_preflight_root_retired.stdout.txt" cat "$ROOT/RETIRED_SINGLE_USE" || overall=1

if [[ $overall -eq 0 ]]; then
    printf 'CORRECTED_STANDARD_EXPORT_PREFLIGHT=PASS_SOURCE0_16A932_AND_6_PATCHES_EHABI_PATCH5\n' >"$OUT/build/standard_export_preflight_corrected.status"
else
    printf 'CORRECTED_STANDARD_EXPORT_PREFLIGHT=RED_STOP\n' >"$OUT/build/standard_export_preflight_corrected.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
