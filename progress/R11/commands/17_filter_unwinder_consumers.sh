#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/17_filter_unwinder_consumers.log"
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
run filter_unwinder_consumers python3 progress/R11/tools/filter_unwinder_consumers.py \
    --needed progress/R11/tables/dt_needed_edges.tsv \
    --output progress/R11/tables/unwinder_dt_needed_consumers.tsv || exit $?
run consumer_table_sha256 sha256sum progress/R11/tables/unwinder_dt_needed_consumers.tsv
