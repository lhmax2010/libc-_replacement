#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C"
RAW="$OUT/raw"
LEDGER="$OUT/commands/04_validate_segment1_report.attempt2.log"

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
record_to validate_required_outputs "$RAW/validate_required_outputs.stdout.txt" bash -o pipefail -c \
    'for f in "$@"; do test -s "$f"; printf "%s\\t%s\\n" "$f" "$(stat -c %s "$f")"; done' _ \
    "$OUT/R9C_segment1_report.md" "$OUT/archival_versions.tsv" "$OUT/source_reference_evidence.tsv" \
    "$OUT/build_input_consumption.tsv" "$OUT/build_impact_verdict.tsv" "$OUT/tarball_features/comparison.tsv" || overall=1
record_to validate_verdict_and_stop_boundary "$RAW/validate_verdict_and_stop_boundary.stdout.txt" bash -o pipefail -c \
    'rg -n "NO_BUILD_IMPACT|第 1 段已完成|未修改.*SOURCE_PROVENANCE|未执行标准导出、三架构重建" "$1"; test "$(rg -c "^libcxx-runtimes-22.1.8-build\\tNO_BUILD_IMPACT" "$2")" -eq 1' _ \
    "$OUT/R9C_segment1_report.md" "$OUT/build_impact_verdict.tsv" || overall=1
record_to validate_archival_sha_values "$RAW/validate_archival_sha_values.stdout.txt" bash -o pipefail -c \
    'sha256sum "$1" "$2" "$3"; test "$(sha256sum "$1" | cut -d" " -f1)" = ff3062a8c3a6bc5b817afe7ab58ef9dc162f06fec4c5d3f25a281776091349f2; test "$(sha256sum "$2" | cut -d" " -f1)" = 4262cb68194f38e3a11aa48b55a177067f868ee414ea955728018971e6d69b2b; test "$(sha256sum "$3" | cut -d" " -f1)" = c15a4452dd41aae4140e41918ba8871b53d3db3c4c29b4bb8ea69ba9588ee4db' _ \
    "$OUT/inputs/platform.git_archival.txt" "$OUT/inputs/local_standard_export.git_archival.txt" "$OUT/inputs/official_release.git_archival.txt" || overall=1
record_to validate_existing_tree_difference "$RAW/validate_existing_tree_difference.stdout.txt" bash -o pipefail -c \
    'set +e; out=$(diff -qr "$1" "$2"); rc=$?; set -e; printf "%s\\n" "$out"; test "$rc" -eq 1; test "$(printf "%s\\n" "$out" | sed "/^$/d" | wc -l)" -eq 1; printf "%s\\n" "$out" | rg -q "/clang/bindings/python/\\.git_archival\\.txt .* differ$"' _ \
    "$REPO/tmp/R9B/archive-content-compare/frozen/llvm-22.1.8" "$REPO/tmp/R9B/archive-content-compare/generated/llvm-22.1.8" || overall=1
record_to validate_official_digest "$RAW/validate_official_digest.stdout.txt" bash -o pipefail -c \
    'api=$(jq -r ".assets[] | select(.name == \"llvm-project-22.1.8.src.tar.xz\") | .digest" "$1"); actual=sha256:$(sha256sum "$2" | cut -d" " -f1); printf "API=%s\\nACTUAL=%s\\n" "$api" "$actual"; test "$api" = "$actual"' _ \
    "$OUT/raw/github_release_22.1.8.json" "$REPO/tmp/R9C/official/llvm-project-22.1.8.src.tar.xz" || overall=1
record_to output_sha256_manifest "$OUT/output_sha256.txt" bash -o pipefail -c \
    'find "$1" -type f ! -path "$2" ! -path "$3" -print0 | sort -z | xargs -0 sha256sum' _ \
    "$OUT" "$OUT/output_sha256.txt" "$LEDGER" || overall=1
record_to final_readonly_boundary "$RAW/final_readonly_boundary.stdout.txt" bash -o pipefail -c \
    'printf "LLVM_BRANCH=%s\\nLLVM_HEAD=%s\\nLLVM_STATUS_BEGIN\\n" "$(git -C "$1" branch --show-current)" "$(git -C "$1" rev-parse HEAD)"; git -C "$1" status --porcelain=v1; printf "LLVM_STATUS_END\\nPROJECT_STATUS_BEGIN\\n"; git -C "$2" status --short; printf "PROJECT_STATUS_END\\n"; test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b; test -z "$(git -C "$1" status --porcelain=v1)"' _ \
    "$REPO/codes/llvm" "$REPO" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
