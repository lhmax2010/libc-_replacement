#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C"
RAW="$OUT/raw"
LEDGER="$OUT/commands/05_final_manifest_and_boundary.log"

: >"$LEDGER"
record_to() {
    local label=$1 outfile=$2
    shift 2
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
    local rc=$?
    printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
    return "$rc"
}

cd "$REPO" || exit 125
overall=0
record_to final_phase_boundary_assertions "$RAW/final_phase_boundary_assertions.stdout.txt" bash -o pipefail -c \
    'test "$(tail -n1 "$1")" = "OVERALL_EXIT_CODE=0"; test "$(rg -c "^libcxx-runtimes-22.1.8-build\\tNO_BUILD_IMPACT" "$2")" -eq 1; test "$(git -C "$3" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b; test -z "$(git -C "$3" status --porcelain=v1)"; test -z "$(find "$4" -mindepth 1 -maxdepth 1 -type d -iname "*R9C*" -print)"; printf "REPORT_SHA256=%s\\nLLVM_HEAD=%s\\nLLVM_STATUS_BYTES=%s\\n" "$(sha256sum "$5" | cut -d" " -f1)" "$(git -C "$3" rev-parse HEAD)" "$(git -C "$3" status --porcelain=v1 | wc -c)"' _ \
    "$OUT/commands/04_validate_segment1_report.attempt2.log" "$OUT/build_impact_verdict.tsv" "$REPO/codes/llvm" \
    "$REPO/tmp/GBS-ROOT" "$OUT/R9C_segment1_report.md" || overall=1
record_to final_project_status "$RAW/final_project_status.stdout.txt" git status --short || overall=1
record_to final_output_sha256_manifest "$OUT/output_sha256.txt" bash -o pipefail -c \
    'find "$1" -type f ! -path "$2" ! -path "$3" -print0 | sort -z | xargs -0 sha256sum' _ \
    "$OUT" "$OUT/output_sha256.txt" "$LEDGER" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
