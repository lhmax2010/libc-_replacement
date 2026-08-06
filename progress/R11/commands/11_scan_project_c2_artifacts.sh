#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/11_scan_project_c2_artifacts.log"
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
run scan_project_c2 python3 progress/R11/tools/scan_project_c2_artifacts.py \
    --artifacts-root artifacts/rebuild_2218_c2 \
    --extract-root tmp/R11/project_c2_extract \
    --table-root progress/R11/tables \
    --ledger progress/R11/commands/11_project_c2_individual_commands.log || exit $?
run project_c2_tables_sha256 sha256sum \
    progress/R11/tables/project_c2_packages.tsv \
    progress/R11/tables/project_c2_elf_inventory.tsv \
    progress/R11/tables/project_c2_unwind_undefined_symbols.tsv
