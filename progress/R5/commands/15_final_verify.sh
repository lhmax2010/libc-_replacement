#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
ledger="$out/commands/15_final_verify.log"
: >"$ledger"

run_recorded() {
    local label=$1
    shift
    local rc
    {
        printf 'LABEL=%s\n' "$label"
        printf 'WORKING_DIRECTORY=%s\n' "$PWD"
        printf 'COMMAND_BEGIN\n'
        printf '%q ' "$@"
        printf '\nCOMMAND_END\n'
    } >>"$ledger"
    "$@" >>"$ledger" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n' "$rc" >>"$ledger"
    printf '%s\t%s\n' "$label" "$rc"
    return "$rc"
}

cd "$repo" || exit 125
run_recorded MANIFEST_VERIFY sha256sum -c "$out/evidence_manifest.sha256" || exit 90
run_recorded REPORT_LINKS_EXIST bash -o pipefail -c \
    "rg -o '\\]\([^)]+\\)' '$out/diagnosis_report.md' | sed -e 's/^](//' -e 's/)$//' | while IFS= read -r rel; do test -f '$out/'\"\$rel\" || { printf 'MISSING=%s\\n' \"\$rel\"; exit 1; }; done" || exit 91
run_recorded TRACKED_DIFF_ASSERT bash -o pipefail -c \
    "test -z \"\$(git diff --name-only)\"" || exit 92
run_recorded FINAL_STATUS git status --short || exit 93
exit 0
