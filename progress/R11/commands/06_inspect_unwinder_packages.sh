#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/06_inspect_unwinder_packages.log"
RAW="$ROOT/progress/R11/raw"
mkdir -p "$RAW"
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
run assert_special_inputs awk -F '\t' '
    NR == 1 { next }
    { count++; command = "test -f \047" $4 "\047"; if (system(command) != 0) missing++ }
    END { printf "COUNT=%d\nMISSING=%d\n", count, missing; exit (count != 24 || missing != 0) }
' progress/R11/inputs/special_unwinder_rpms.tsv || exit $?

run inspect_special_packages python3 progress/R11/tools/inspect_unwinder_packages.py \
    --manifest progress/R11/inputs/special_unwinder_rpms.tsv \
    --extract-root tmp/R11/special_unwinder_extract \
    --table-root progress/R11/tables \
    --raw-root progress/R11/raw \
    --ledger progress/R11/commands/06_special_unwinder_individual_commands.log || exit $?

run special_table_sha256 sha256sum \
    progress/R11/tables/special_unwinder_packages.tsv \
    progress/R11/tables/special_unwinder_file_lists.tsv \
    progress/R11/tables/special_unwinder_sonames.tsv \
    progress/R11/tables/special_unwinder_exports.tsv \
    progress/R11/tables/libunwind_file_conflicts.tsv
