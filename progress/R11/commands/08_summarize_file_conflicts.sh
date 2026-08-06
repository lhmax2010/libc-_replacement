#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/08_summarize_file_conflicts.log"
RAW="$ROOT/progress/R11/raw"
: > "$LOG"

run() {
    local label="$1"
    shift
    local stdout="$RAW/${label}.stdout.txt"
    local stderr="$RAW/${label}.stderr.txt"
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD" >> "$LOG"
    printf '%q ' "$@" >> "$LOG"
    printf '\n' >> "$LOG"
    "$@" >"$stdout" 2>"$stderr"
    local rc=$?
    printf 'STDOUT_FILE=%s\nSTDERR_FILE=%s\nEXIT_CODE=%s\n\n' "$stdout" "$stderr" "$rc" >> "$LOG"
    return "$rc"
}

cd "$ROOT" || exit 1
run summarize_file_conflicts python3 progress/R11/tools/summarize_file_conflicts.py \
    --files progress/R11/tables/special_unwinder_file_lists.tsv \
    --output progress/R11/tables/libunwind_same_path_conflicts.tsv || exit $?
run conflict_table_sha256 sha256sum \
    progress/R11/tables/libunwind_same_path_conflicts.tsv \
    progress/R11/tables/rpm_conflict_test_exit_codes.tsv
