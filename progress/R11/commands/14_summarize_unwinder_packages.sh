#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/14_summarize_unwinder_packages.log"
RAW="$ROOT/progress/R11/raw"
: > "$LOG"

run() {
    local label="$1"; shift
    local stdout="$RAW/${label}.stdout.txt" stderr="$RAW/${label}.stderr.txt"
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD" >> "$LOG"; printf '%q ' "$@" >> "$LOG"; printf '\n' >> "$LOG"
    "$@" >"$stdout" 2>"$stderr"; local rc=$?
    printf 'STDOUT_FILE=%s\nSTDERR_FILE=%s\nEXIT_CODE=%s\n\n' "$stdout" "$stderr" "$rc" >> "$LOG"; return "$rc"
}

cd "$ROOT" || exit 1
run summarize_unwinder_packages python3 progress/R11/tools/summarize_unwinder_packages.py \
    --packages progress/R11/tables/special_unwinder_packages.tsv \
    --files progress/R11/tables/special_unwinder_file_lists.tsv \
    --sonames progress/R11/tables/special_unwinder_sonames.tsv \
    --exports progress/R11/tables/special_unwinder_exports.tsv \
    --dependencies progress/R11/tables/libunwind_dependency_metadata.tsv \
    --output-root progress/R11/tables || exit $?
run unwinder_summary_sha256 sha256sum \
    progress/R11/tables/special_unwinder_export_summary.tsv \
    progress/R11/tables/unwinder_soname_comparison.tsv \
    progress/R11/tables/llvm_libunwind_reference_path_presence.tsv \
    progress/R11/tables/libunwind_dependency_metadata_summary.tsv
