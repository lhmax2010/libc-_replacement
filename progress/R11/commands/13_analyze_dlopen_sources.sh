#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/13_analyze_dlopen_sources.log"
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
run analyze_dlopen_sources python3 progress/R11/tools/analyze_dlopen_sources.py \
    --elf-inventory progress/R11/tables/elf_inventory.tsv \
    --source-download-status progress/R11/tables/source_download_status.tsv \
    --output-root progress/R11/tables \
    --work-root tmp/R11/dlopen_sources \
    --ledger progress/R11/commands/13_dlopen_source_individual_commands.log || exit $?
run dlopen_source_tables_sha256 sha256sum \
    progress/R11/tables/dlopen_source_analysis_status.tsv \
    progress/R11/tables/dlopen_source_archive_status.tsv \
    progress/R11/tables/dlopen_source_token_hits.tsv \
    progress/R11/tables/dlopen_source_call_scope_approximation.tsv
