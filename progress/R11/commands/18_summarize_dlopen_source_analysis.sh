#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/18_summarize_dlopen_source_analysis.log"
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
run summarize_dlopen_source_analysis python3 progress/R11/tools/summarize_dlopen_source_analysis.py \
    --status progress/R11/tables/dlopen_source_analysis_status.tsv \
    --archives progress/R11/tables/dlopen_source_archive_status.tsv \
    --calls progress/R11/tables/dlopen_source_call_scope_approximation.tsv \
    --output-summary progress/R11/tables/dlopen_source_analysis_summary.tsv \
    --output-limitations progress/R11/tables/dlopen_scope_measurement_limitations.tsv || exit $?
run dlopen_source_summary_sha256 sha256sum \
    progress/R11/tables/dlopen_source_analysis_summary.tsv \
    progress/R11/tables/dlopen_scope_measurement_limitations.tsv
