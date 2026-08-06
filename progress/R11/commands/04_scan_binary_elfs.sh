#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/04_scan_binary_elfs.log"
RAW="$ROOT/progress/R11/raw"
TABLES="$ROOT/progress/R11/tables"
mkdir -p "$RAW" "$TABLES"
: > "$LOG"

run() {
    local label="$1"
    shift
    local stdout="$RAW/${label}.stdout.txt"
    local stderr="$RAW/${label}.stderr.txt"
    {
        printf 'LABEL=%s\n' "$label"
        printf 'PWD=%s\n' "$PWD"
        printf 'COMMAND='
        printf '%q ' "$@"
        printf '\n'
    } >> "$LOG"
    "$@" >"$stdout" 2>"$stderr"
    local rc=$?
    {
        printf 'STDOUT_FILE=%s\n' "$stdout"
        printf 'STDERR_FILE=%s\n' "$stderr"
        printf 'EXIT_CODE=%s\n\n' "$rc"
    } >> "$LOG"
    return "$rc"
}

cd "$ROOT" || exit 1
run assert_download_complete awk -F '\t' '
    NR == 1 { next }
    { total++; if ($14 == "PASS") pass++; else failed++ }
    END {
        printf "TOTAL=%d\nPASS=%d\nNON_PASS=%d\n", total, pass, failed
        exit ! (total == 11332)
    }
' progress/R11/tables/binary_download_status.tsv || exit $?

run scan_all_binary_rpms python3 progress/R11/tools/scan_binary_rpms.py \
    --download-status progress/R11/tables/binary_download_status.tsv \
    --extract-root tmp/R11/extracted \
    --shard-root tmp/R11/elf_scan_shards \
    --table-root progress/R11/tables \
    --ledger progress/R11/commands/04_binary_elf_individual_commands.log \
    --jobs 12
scan_rc=$?

run scan_table_sha256 sha256sum \
    progress/R11/tables/rpm_extraction_status.tsv \
    progress/R11/tables/elf_inventory.tsv \
    progress/R11/tables/unwind_undefined_symbols.tsv \
    progress/R11/tables/dt_needed_edges.tsv \
    progress/R11/tables/elf_analysis_failures.tsv
hash_rc=$?

printf 'SCAN_EXIT_CODE=%s\nHASH_EXIT_CODE=%s\n' "$scan_rc" "$hash_rc" >> "$LOG"
exit "$scan_rc"
