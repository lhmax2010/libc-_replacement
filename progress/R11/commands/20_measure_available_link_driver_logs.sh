#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/20_measure_available_link_driver_logs.log"
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
run measure_available_link_driver_logs python3 progress/R11/tools/measure_available_link_driver_logs.py \
    --link-log docs/progress/R2/link_commands_all.raw.log \
    --raw-candidate-log /home/toolchain/development/libc++_replacement_bak/wave1_execution/stageA/candidate_build/logs/01_libcynara-commons.raw.log \
    --output-measurement progress/R11/tables/link_driver_available_log_measurement.tsv \
    --output-scope progress/R11/tables/link_driver_search_scope.tsv || exit $?
run available_link_driver_tables_sha256 sha256sum \
    progress/R11/tables/link_driver_available_log_measurement.tsv \
    progress/R11/tables/link_driver_search_scope.tsv
