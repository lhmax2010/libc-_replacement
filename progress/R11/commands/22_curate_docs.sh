#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/22_curate_docs.log"
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
run curate_r11_files rsync -a --files-from=progress/R11/inputs/curated_files.txt progress/R11/ docs/progress/R11/ || exit $?
run curated_file_presence bash -o pipefail -c 'while IFS= read -r path; do test -f "docs/progress/R11/$path" || exit 1; done < progress/R11/inputs/curated_files.txt' || exit $?
run curated_manifest bash -o pipefail -c 'cd docs/progress/R11 && find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256' || exit $?
run curated_manifest_verify bash -o pipefail -c 'cd docs/progress/R11 && sha256sum -c MANIFEST.sha256' || exit $?
