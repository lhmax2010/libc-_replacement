#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/06b_reinspect_exported_unwinder_symbols.log"
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
run reinspect_exported_symbols python3 progress/R11/tools/inspect_unwinder_packages.py \
    --manifest progress/R11/inputs/special_unwinder_rpms.tsv \
    --extract-root tmp/R11/special_unwinder_extract_v2 \
    --table-root progress/R11/tables \
    --raw-root progress/R11/raw \
    --ledger progress/R11/commands/06b_special_unwinder_individual_commands.log || exit $?
run corrected_special_table_sha256 sha256sum \
    progress/R11/tables/special_unwinder_packages.tsv \
    progress/R11/tables/special_unwinder_file_lists.tsv \
    progress/R11/tables/special_unwinder_sonames.tsv \
    progress/R11/tables/special_unwinder_exports.tsv \
    progress/R11/tables/libunwind_file_conflicts.tsv
