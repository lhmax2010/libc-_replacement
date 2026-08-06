#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/19_finalize_static_summaries.log"
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
run finalize_static_summaries python3 progress/R11/tools/finalize_static_summaries.py \
    --elf-inventory progress/R11/tables/elf_inventory.tsv \
    --plugins progress/R11/tables/plugin_candidate_elfs.tsv \
    --plugin-packages progress/R11/tables/plugin_mechanism_packages.tsv \
    --repodata progress/R11/tables/repodata_inventory.tsv \
    --output-root progress/R11/tables || exit $?
run finalized_static_tables_sha256 sha256sum \
    progress/R11/tables/counting_units_final.tsv \
    progress/R11/tables/plugin_mechanism_summary.tsv \
    progress/R11/tables/snapshot_identity.tsv \
    progress/R11/tables/classification_methodology.tsv
