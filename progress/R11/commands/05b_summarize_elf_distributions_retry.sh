#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/05b_summarize_elf_distributions_retry.log"
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
run summarize_distributions_retry python3 progress/R11/tools/summarize_elf_distributions.py \
    --package-records progress/R11/tables/binary_package_records.tsv \
    --extraction progress/R11/tables/rpm_extraction_status.tsv \
    --elfs progress/R11/tables/elf_inventory.tsv \
    --unwind progress/R11/tables/unwind_undefined_symbols.tsv \
    --needed progress/R11/tables/dt_needed_edges.tsv \
    --failures progress/R11/tables/elf_analysis_failures.tsv \
    --output-root progress/R11/tables || exit $?
run distribution_retry_sha256 sha256sum \
    progress/R11/tables/measured_denominators.tsv \
    progress/R11/tables/c_cpp_unwind_shape.tsv \
    progress/R11/tables/unwind_reference_version_distribution.tsv \
    progress/R11/tables/unwind_symbol_distribution.tsv \
    progress/R11/tables/abi_unwind_reference_version_distribution.tsv \
    progress/R11/tables/abi_unwind_undefined_symbols.tsv \
    progress/R11/tables/unwinder_dt_needed_fanin.tsv \
    progress/R11/tables/dlopen_and_plugin_operational.tsv \
    progress/R11/tables/plugin_candidate_elfs.tsv \
    progress/R11/tables/plugin_mechanism_packages.tsv \
    progress/R11/tables/armv7l_special_distribution.tsv \
    progress/R11/tables/binary_analysis_failures.tsv
