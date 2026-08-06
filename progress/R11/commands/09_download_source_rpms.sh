#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/09_download_source_rpms.log"
RAW="$ROOT/progress/R11/raw"
TABLES="$ROOT/progress/R11/tables"
mkdir -p "$RAW" "$TABLES"
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
run assert_source_download_root_fresh test ! -e tmp/R11/srpms || exit $?
run download_all_source_rpms python3 progress/R11/tools/download_rpms.py \
    --manifest progress/R11/tables/source_package_records.tsv \
    --repositories progress/R11/inputs/repositories.tsv \
    --output-root tmp/R11/srpms \
    --status progress/R11/tables/source_download_status.tsv \
    --ledger progress/R11/commands/09_source_rpm_individual_downloads.log \
    --jobs 24
download_rc=$?

run summarize_source_download awk -F '\t' '
    NR == 1 { next }
    { total++; bytes += $9; status[$14]++ }
    END {
        printf "metric\tvalue\nrecords\t%d\nrepo_size_bytes\t%.0f\n", total, bytes
        for (item in status) printf "status_%s\t%d\n", item, status[item]
    }
' progress/R11/tables/source_download_status.tsv
summary_rc=$?
cp "$RAW/summarize_source_download.stdout.txt" "$TABLES/source_download_summary.tsv"
awk -F '\t' 'NR == 1 || $14 != "PASS"' \
    progress/R11/tables/source_download_status.tsv > "$TABLES/source_download_failures.tsv"
printf 'DOWNLOAD_EXIT_CODE=%s\nSUMMARY_EXIT_CODE=%s\n' "$download_rc" "$summary_rc" >> "$LOG"
exit 0
