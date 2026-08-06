#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/10b_inspect_source_specs_absolute_path_retry.log"
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
run inspect_all_source_specs_absolute_path_retry python3 progress/R11/tools/inspect_source_specs.py \
    --download-status progress/R11/tables/source_download_status.tsv \
    --extract-root tmp/R11/source_specs_v2 \
    --shard-root tmp/R11/source_spec_shards_v2 \
    --table-root progress/R11/tables \
    --ledger progress/R11/commands/10b_source_spec_individual_commands.log \
    --jobs 12 || exit $?
run source_spec_retry_tables_sha256 sha256sum \
    progress/R11/tables/source_spec_extraction_status.tsv \
    progress/R11/tables/source_spec_inventory.tsv \
    progress/R11/tables/source_spec_text_hits.tsv \
    progress/R11/tables/source_spec_link_driver_text_classification.tsv
