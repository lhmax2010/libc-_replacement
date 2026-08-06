#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/12c_finalize_dwarf_denominator_label.log"
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
run finalize_dwarf_denominator_label python3 progress/R11/tools/analyze_dwarf_languages.py \
    --elf-inventory progress/R11/tables/elf_inventory.tsv \
    --extract-root tmp/R11/extracted \
    --output-root progress/R11/tables \
    --ledger progress/R11/commands/12c_dwarf_individual_commands.log || exit $?
run final_dwarf_tables_sha256 sha256sum \
    progress/R11/tables/dwarf_language_per_runtime_elf.tsv \
    progress/R11/tables/dwarf_language_distribution.tsv
