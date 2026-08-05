#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LEDGER="$OUT/commands/03c_forensics_collector_retry.log"
EXPORT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1"
COMPARE="$REPO/tmp/R9B/archive-content-compare"

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
record_to assert_prior_collector_failures_are_non_substantive "$RAW/retry_assert_prior_collector_failures_are_non_substantive.stdout.txt" bash -o pipefail -c \
    'rg -q "LABEL=assert_only_archival_file_differs" "$1" && rg -q "LABEL=assert_patch_commit_mapping" "$1" && rg -q "OVERALL_EXIT_CODE=1" "$1" && test "$(wc -l <"$2")" -eq 1 && rg -q "clang/bindings/python/.git_archival.txt" "$2"' \
    _ "$OUT/commands/03b_common_input_red_stop_forensics.log" "$RAW/red_stop_extracted_tree_diff.stdout.txt" || overall=1
record_to retry_assert_only_archival_file_differs "$RAW/retry_assert_only_archival_file_differs.stdout.txt" bash -o pipefail -c \
    'd=$(diff -qr "$1/frozen" "$1/generated" || true); printf "%s\\n" "$d"; test "$(printf "%s\\n" "$d" | wc -l)" -eq 1 && printf "%s\\n" "$d" | rg -q "clang/bindings/python/.git_archival.txt"' _ "$COMPARE" || overall=1
record_to retry_exported_patch_commit_mapping "$RAW/retry_exported_patch_commit_mapping.stdout.txt" bash -o pipefail -c \
    'export_dir=$1; llvm=$2; shift 2; printf "patch_file\\tsha256\\tcorresponding_commit\\tsubject\\texported_patch_id\\tcommit_patch_id\\tstatus\\n"; n=1; for c in "$@"; do p=$(find "$export_dir" -maxdepth 1 -type f -name "$(printf "%04d" "$n")-*.patch*" -print -quit); case "$p" in *.gz) export_id=$(gzip -dc "$p" | git patch-id --stable | cut -d" " -f1);; *.xz) export_id=$(xz -dc "$p" | git patch-id --stable | cut -d" " -f1);; *) export_id=$(git patch-id --stable <"$p" | cut -d" " -f1);; esac; commit_id=$(git -C "$llvm" show --format= "$c" -- . ":(exclude)packaging/**" | git patch-id --stable | cut -d" " -f1); status=MISMATCH; test -n "$export_id" && test "$export_id" = "$commit_id" && status=MATCH; printf "%s\\t%s\\t%s\\t%s\\t%s\\t%s\\t%s\\n" "$(basename "$p")" "$(sha256sum "$p" | cut -d" " -f1)" "$c" "$(git -C "$llvm" show -s --format=%s "$c")" "$export_id" "$commit_id" "$status"; n=$((n+1)); done' \
    _ "$EXPORT" "$REPO/codes/llvm" \
    53accdc47cd72c0daf61bf70e52946b2d8d80f2e \
    f31417256f953cc5d26cd188704c89ad41572b50 \
    9576175b59fa577c940eff1712a59b129ae03f03 \
    9ad40bd946a75b44d8e05de8ba3263a5ce2ed885 \
    fd7a4929c0e424d564cdd1f0c55af9153dcea321 \
    3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b || overall=1
record_to retry_assert_patch_commit_mapping "$RAW/retry_assert_patch_commit_mapping.stdout.txt" bash -o pipefail -c \
    'test "$(tail -n +2 "$1" | wc -l)" -eq 6 && test "$(rg -c "[[:space:]]MATCH$" "$1")" -eq 6 && rg -q "^0006-.*[[:space:]]3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b[[:space:]].*[[:space:]]ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479[[:space:]]ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479[[:space:]]MATCH$" "$1"' \
    _ "$RAW/retry_exported_patch_commit_mapping.stdout.txt" || overall=1
{
    printf 'item\toriginal_collector_issue\tretry_result\n'
    printf 'archive_tree_assert\tdiff_expected_rc_1_propagated_by_pipefail\t%s\n' "$([[ $overall -eq 0 ]] && printf PASS || printf CHECK_LOG)"
    printf 'patch_id_mapping\tawk_escaping_produced_empty_fields_and_assert_rc_2\t%s\n' "$([[ $overall -eq 0 ]] && printf PASS || printf CHECK_LOG)"
} >"$OUT/collector_retry.tsv"
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
