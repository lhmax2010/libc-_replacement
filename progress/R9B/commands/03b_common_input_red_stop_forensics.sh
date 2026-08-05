#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LEDGER="$OUT/commands/03b_common_input_red_stop_forensics.log"
EXPORT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1"
GENERATED="$EXPORT/llvm-22.1.8.tar.gz"
FROZEN="$REPO/tmp/R9B/input/canonical/llvm-22.1.8.tar.gz"
COMPARE="$REPO/tmp/R9B/archive-content-compare"
EXPECTED=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e
ACTUAL=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2

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
record_expected() {
    local expected_rc=$1 label=$2 outfile=$3
    shift 3
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
    local rc=$?
    printf 'EXIT_CODE=%s\nEXPECTED_EXIT_CODE=%s\n\n' "$rc" "$expected_rc" >>"$LEDGER"
    [[ $rc -eq $expected_rc ]]
}

cd "$REPO" || exit 125
overall=0
record_to frozen_and_generated_sha256 "$RAW/red_stop_frozen_and_generated_sha256.stdout.txt" sha256sum "$FROZEN" "$GENERATED" || overall=1
record_to assert_observed_hashes "$RAW/red_stop_assert_observed_hashes.stdout.txt" bash -o pipefail -c \
    'test "$(sha256sum "$1" | cut -d" " -f1)" = "$3" && test "$(sha256sum "$2" | cut -d" " -f1)" = "$4"' _ "$FROZEN" "$GENERATED" "$EXPECTED" "$ACTUAL" || overall=1
record_expected 1 frozen_vs_generated_cmp "$RAW/red_stop_frozen_vs_generated_cmp.stdout.txt" cmp "$FROZEN" "$GENERATED" || overall=1
record_to archive_name_lists "$RAW/red_stop_archive_name_lists.stdout.txt" bash -o pipefail -c \
    'for f in "$1" "$2"; do printf "FILE=%s\\n" "$f"; tar -tzf "$f" | sha256sum; tar -tzf "$f" | wc -l; done' _ "$FROZEN" "$GENERATED" || overall=1
record_to assert_archive_name_lists_equal "$RAW/red_stop_assert_archive_name_lists_equal.stdout.txt" bash -o pipefail -c \
    'cmp <(tar -tzf "$1") <(tar -tzf "$2")' _ "$FROZEN" "$GENERATED" || overall=1
record_to archive_uncompressed_sha256 "$RAW/red_stop_archive_uncompressed_sha256.stdout.txt" bash -o pipefail -c \
    'for f in "$1" "$2"; do printf "FILE=%s\\n" "$f"; gzip -dc "$f" | sha256sum; done' _ "$FROZEN" "$GENERATED" || overall=1
record_to archival_substitution_contents "$RAW/red_stop_archival_substitution_contents.stdout.txt" bash -o pipefail -c \
    'for f in "$1" "$2"; do printf "FILE=%s\\n" "$f"; tar -xOzf "$f" llvm-22.1.8/clang/bindings/python/.git_archival.txt; done' _ "$FROZEN" "$GENERATED" || overall=1
record_to assert_compare_dir_fresh "$RAW/red_stop_assert_compare_dir_fresh.stdout.txt" test ! -e "$COMPARE" || overall=1
if [[ $overall -eq 0 ]]; then
    record_to create_compare_dirs "$RAW/red_stop_create_compare_dirs.stdout.txt" mkdir -p "$COMPARE/frozen" "$COMPARE/generated" || overall=1
    record_to extract_frozen_archive "$RAW/red_stop_extract_frozen_archive.stdout.txt" tar -xzf "$FROZEN" -C "$COMPARE/frozen" || overall=1
    record_to extract_generated_archive "$RAW/red_stop_extract_generated_archive.stdout.txt" tar -xzf "$GENERATED" -C "$COMPARE/generated" || overall=1
    record_expected 1 extracted_tree_diff "$RAW/red_stop_extracted_tree_diff.stdout.txt" diff -qr "$COMPARE/frozen" "$COMPARE/generated" || overall=1
    record_to assert_only_archival_file_differs "$RAW/red_stop_assert_only_archival_file_differs.stdout.txt" bash -o pipefail -c \
        'test "$(diff -qr "$1/frozen" "$1/generated" | wc -l)" -eq 1 && diff -qr "$1/frozen" "$1/generated" | rg -q "clang/bindings/python/.git_archival.txt"' _ "$COMPARE" || overall=1
fi
record_to exported_spec_patch_lines "$RAW/red_stop_exported_spec_patch_lines.stdout.txt" rg -n '^# Patches auto-generated|^Patch[0-9]+:|^# 000[0-9]-|^%patch[0-9]+ ' "$EXPORT/libcxx-runtimes.spec" || overall=1
record_to assert_six_patches_and_patch5 "$RAW/red_stop_assert_six_patches_and_patch5.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1" -maxdepth 1 -type f -name "0*.patch*" | wc -l)" -eq 6 && test -f "$1/0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch" && rg -q "^Patch5:.*0006-libcxxabi" "$1/libcxx-runtimes.spec" && rg -q "^%patch5 -p1$" "$1/libcxx-runtimes.spec"' _ "$EXPORT" || overall=1
record_to exported_patch_commit_mapping "$RAW/red_stop_exported_patch_commit_mapping.stdout.txt" bash -o pipefail -c \
    'export_dir=$1; llvm=$2; shift 2; printf "patch_file\\tsha256\\tcorresponding_commit\\tsubject\\texported_patch_id\\tcommit_patch_id\\tstatus\\n"; n=1; for c in "$@"; do p=$(find "$export_dir" -maxdepth 1 -type f -name "$(printf "%04d" "$n")-*.patch*" -print -quit); case "$p" in *.gz) export_id=$(gzip -dc "$p" | git patch-id --stable | awk "{print \\$1}");; *.xz) export_id=$(xz -dc "$p" | git patch-id --stable | awk "{print \\$1}");; *) export_id=$(git patch-id --stable <"$p" | awk "{print \\$1}");; esac; commit_id=$(git -C "$llvm" show --format= "$c" -- . ":(exclude)packaging/**" | git patch-id --stable | awk "{print \\$1}"); status=MISMATCH; test -n "$export_id" && test "$export_id" = "$commit_id" && status=MATCH; printf "%s\\t%s\\t%s\\t%s\\t%s\\t%s\\t%s\\n" "$(basename "$p")" "$(sha256sum "$p" | cut -d" " -f1)" "$c" "$(git -C "$llvm" show -s --format=%s "$c")" "$export_id" "$commit_id" "$status"; n=$((n+1)); done' \
    _ "$EXPORT" "$REPO/codes/llvm" \
    53accdc47cd72c0daf61bf70e52946b2d8d80f2e \
    f31417256f953cc5d26cd188704c89ad41572b50 \
    9576175b59fa577c940eff1712a59b129ae03f03 \
    9ad40bd946a75b44d8e05de8ba3263a5ce2ed885 \
    fd7a4929c0e424d564cdd1f0c55af9153dcea321 \
    3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b || overall=1
record_to assert_patch_commit_mapping "$RAW/red_stop_assert_patch_commit_mapping.stdout.txt" bash -o pipefail -c \
    'test "$(tail -n +2 "$1" | wc -l)" -eq 6 && test "$(awk -F"\\t" "NR>1 && \\$7 != \\"MATCH\\" {n++} END {print n+0}" "$1")" -eq 0' _ "$RAW/red_stop_exported_patch_commit_mapping.stdout.txt" || overall=1
record_to preflight_roots_retired "$RAW/red_stop_preflight_roots_retired.stdout.txt" bash -o pipefail -c \
    'for r in "$@"; do printf "ROOT=%s\\n" "$r"; cat "$r/RETIRED_SINGLE_USE"; done' _ \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std" \
    "$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2" || overall=1
record_to assert_no_arch_build_roots "$RAW/red_stop_assert_no_arch_build_roots.stdout.txt" bash -o pipefail -c \
    'for a in armv7l aarch64 x86_64; do test ! -e "$1/tmp/GBS-ROOT/LIBCXX-2218-${a}-20260805-std" || exit 1; done' _ "$REPO" || overall=1
record_to assert_no_new_artifacts "$RAW/red_stop_assert_no_new_artifacts.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1/artifacts/rebuild_2218_std" -type f 2>/dev/null | wc -l)" -eq 0' _ "$REPO" || overall=1
record_to assert_old_artifacts_not_quarantined "$RAW/red_stop_assert_old_artifacts_not_quarantined.stdout.txt" bash -o pipefail -c \
    'test -d "$1/artifacts/rebuild_2218" && test ! -e "$1/artifacts/quarantine/20260805_pre_std_export"' _ "$REPO" || overall=1
record_to assert_provenance_rollback "$RAW/red_stop_assert_provenance_rollback.stdout.txt" git diff --exit-code -- packaging/SOURCE_PROVENANCE || overall=1
record_to authoritative_llvm_final_identity "$RAW/red_stop_authoritative_llvm_final_identity.stdout.txt" bash -o pipefail -c \
    'printf "branch="; git -C "$1" branch --show-current; printf "head="; git -C "$1" rev-parse HEAD; printf "status_begin\\n"; git -C "$1" status --porcelain=v1; printf "status_end\\n"' _ "$REPO/codes/llvm" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
