#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/16_compare_project_platform_unwind.log"
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
run compare_project_platform_unwind python3 progress/R11/tools/compare_project_platform_unwind.py \
    --platform-abi-unwind progress/R11/tables/abi_unwind_undefined_symbols.tsv \
    --project-unwind progress/R11/tables/project_c2_unwind_undefined_symbols.tsv \
    --output progress/R11/tables/project_c2_vs_platform_unwind.tsv || exit $?
run comparison_sha256 sha256sum progress/R11/tables/project_c2_vs_platform_unwind.tsv
