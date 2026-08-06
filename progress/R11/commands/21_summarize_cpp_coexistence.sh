#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/21_summarize_cpp_coexistence.log"
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
run summarize_cpp_coexistence python3 progress/R11/tools/summarize_cpp_coexistence.py \
    --elf-inventory progress/R11/tables/elf_inventory.tsv \
    --dwarf progress/R11/tables/dwarf_language_per_runtime_elf.tsv \
    --output-package progress/R11/tables/c_cpp_package_coexistence.tsv \
    --output-same-elf progress/R11/tables/c_cpp_same_elf_measurement.tsv || exit $?
run cpp_coexistence_tables_sha256 sha256sum \
    progress/R11/tables/c_cpp_package_coexistence.tsv \
    progress/R11/tables/c_cpp_same_elf_measurement.tsv
