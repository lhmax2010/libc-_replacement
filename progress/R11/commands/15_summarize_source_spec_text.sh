#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/15_summarize_source_spec_text.log"
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
run summarize_source_spec_text python3 progress/R11/tools/summarize_source_spec_text.py \
    --status progress/R11/tables/source_spec_extraction_status.tsv \
    --classifications progress/R11/tables/source_spec_link_driver_text_classification.tsv \
    --hits progress/R11/tables/source_spec_text_hits.tsv \
    --output-summary progress/R11/tables/source_spec_text_summary.tsv \
    --output-availability progress/R11/tables/link_driver_measurement_availability.tsv || exit $?
run source_spec_summary_sha256 sha256sum \
    progress/R11/tables/source_spec_text_summary.tsv \
    progress/R11/tables/link_driver_measurement_availability.tsv
