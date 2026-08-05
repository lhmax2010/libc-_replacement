#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R6"
raw="$out/raw"
ledger="$out/commands/00_identity_preflight.log"
mkdir -p "$raw" "$repo/progress/state"
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
run_recorded STATE_STARTED bash -o pipefail -c \
    "printf 'R6\\tSTARTED\\t%s\\n' \"\$(date --iso-8601=seconds)\" > '$repo/progress/state/R6.tsv'" || exit 90
run_recorded DISK_GUARD bash -o pipefail -c \
    "df -B1 '$repo' | tee '$raw/disk_guard.raw.txt'; test \"\$(df -B1 --output=avail '$repo' | tail -1)\" -ge 85899345920" || exit 91
run_recorded PROJECT_IDENTITY bash -o pipefail -c \
    "{ pwd; git branch --show-current; git rev-parse HEAD; git status --porcelain=v1; git remote -v; } > '$raw/project_identity.raw.txt'" || exit 92
run_recorded CODES_TOP bash -o pipefail -c \
    "find '$repo/codes' -mindepth 1 -maxdepth 2 -printf '%y\\t%p\\t%l\\n' | LC_ALL=C sort > '$raw/codes_top.raw.txt'" || exit 93
run_recorded CODES_GIT_ROOTS bash -o pipefail -c \
    "find '$repo/codes' -mindepth 2 -maxdepth 3 -type d -name .git -printf '%h\\n' | LC_ALL=C sort > '$raw/codes_git_roots.raw.txt'" || exit 94
exit 0
