#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C"
RAW="$OUT/raw"
LEDGER="$OUT/commands/03_finalize_segment1_checks.log"
R8="$REPO/progress/R8"
R9B="$REPO/progress/R9B"

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
    local expected=$1 label=$2 outfile=$3
    shift 3
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
    local rc=$?
    printf 'EXIT_CODE=%s\nEXPECTED_EXIT_CODE=%s\n\n' "$rc" "$expected" >>"$LEDGER"
    [[ $rc -eq $expected ]]
}

cd "$REPO" || exit 125
overall=0
record_to r9b_exact_single_file_tree_difference "$RAW/r9b_exact_single_file_tree_difference.stdout.txt" bash -o pipefail -c \
    'nl -ba "$1" | sed -n "55,78p"; printf "TREE_DIFF_ORIGINAL\\n"; cat "$2"; printf "SINGLE_FILE_ASSERT_RETRY\\n"; cat "$3"' _ \
    "$R9B/R9B_report.md" "$R9B/raw/red_stop_extracted_tree_diff.stdout.txt" \
    "$R9B/raw/retry_assert_only_archival_file_differs.stdout.txt" || overall=1
record_expected 1 archival_in_r6_version_cmake_inputs "$RAW/archival_in_r6_version_cmake_inputs.stdout.txt" rg -n \
    'VersionFromVCS|GenerateVersionFromVCS|\.git_archival\.txt|git_archival' "$R8/r6_cmake_consumed_inputs.tsv" || overall=1
record_to build_consumption_zero_hit_assertion "$RAW/build_consumption_zero_hit_assertion.stdout.txt" bash -o pipefail -c \
    'for f in "$@"; do n=$(rg -c "(^|/)clang/bindings/python/\\.git_archival\\.txt|git_archival" "$f" 2>/dev/null || true); printf "%s\\t%s\\n" "$f" "${n:-0}"; test "${n:-0}" -eq 0; done' _ \
    "$R8/r6_actual_compiled_tu.tsv" "$R8/r6_actual_object_dependencies.tsv" \
    "$R8/r6_cmake_consumed_inputs.tsv" "$R8/r6_consumed_source_paths.tsv" || overall=1
record_to evidence_ledgers_status "$RAW/evidence_ledgers_status.stdout.txt" bash -o pipefail -c \
    'for f in "$@"; do printf "FILE=%s\\n" "$f"; tail -n 1 "$f"; done; test "$(tail -n1 "$1")" = "OVERALL_EXIT_CODE=0"; test "$(tail -n1 "$2")" = "OVERALL_EXIT_CODE=0"' _ \
    "$OUT/commands/01_build_consumption_evidence.attempt2.log" "$OUT/commands/02_tarball_feature_comparison.log" || overall=1
record_expected 1 assert_no_export_or_build_command "$RAW/assert_no_export_or_build_command.stdout.txt" rg -n \
    '^COMMAND=(gbs|rpmbuild|cmake|ninja|make)( |$)' "$OUT/commands" || overall=1
record_to assert_no_r9c_buildroot "$RAW/assert_no_r9c_buildroot.stdout.txt" bash -o pipefail -c \
    'matches=$(find "$1" -mindepth 1 -maxdepth 1 -type d -iname "*R9C*" -print); printf "%s" "$matches"; test -z "$matches"' _ \
    "$REPO/tmp/GBS-ROOT" || overall=1
record_to final_source_identity "$RAW/final_source_identity.stdout.txt" bash -o pipefail -c \
    'printf "BRANCH=%s\\nHEAD=%s\\nSTATUS_PORCELAIN_BEGIN\\n" "$(git -C "$1" branch --show-current)" "$(git -C "$1" rev-parse HEAD)"; git -C "$1" status --porcelain=v1; printf "STATUS_PORCELAIN_END\\n"; test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b; test -z "$(git -C "$1" status --porcelain=v1)"' _ \
    "$REPO/codes/llvm" || overall=1
record_to project_worktree_status "$RAW/project_worktree_status.stdout.txt" git status --short || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
