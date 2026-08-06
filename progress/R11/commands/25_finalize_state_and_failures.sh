#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/25_finalize_state_and_failures.log"
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
run finalize_state_and_failures python3 progress/R11/tools/finalize_state_and_failures.py --table-root progress/R11/tables || exit $?
run state_and_failure_sha256 sha256sum progress/R11/tables/failure_registry.tsv progress/R11/tables/state.tsv
