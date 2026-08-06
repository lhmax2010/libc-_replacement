#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/24_generate_full_manifest.log"
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
run generate_full_manifest bash -o pipefail -c 'cd progress/R11 && find . -type f ! -name MANIFEST.sha256 ! -path "./commands/24_generate_full_manifest.log" ! -path "./raw/generate_full_manifest.*" ! -path "./raw/verify_full_manifest.*" ! -path "./tools/__pycache__/*" -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256' || exit $?
run verify_full_manifest bash -o pipefail -c 'cd progress/R11 && sha256sum -c MANIFEST.sha256' || exit $?
