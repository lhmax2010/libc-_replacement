#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
ledger="$out/commands/14_finalize_evidence.log"
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
run_recorded REPORT_REQUIRED_SECTIONS bash -o pipefail -c \
    "for s in '## 0. 结论' '## 1. 失败点上下文' '## 2. 21.1.1→22.1.8 版本差异' '## 3. 声明来源实测' '## 4. 21.1.1 armv7l 成功构建实证复核' '## 5. 22.1.8 上游配置语义' '## 6. 可选处置路径清单'; do rg -F \"\$s\" '$out/diagnosis_report.md'; done" || exit 90
run_recorded INDEX_PASS_ASSERT bash -o pipefail -c \
    "test \"\$(awk -F '\\t' 'NR>1 && \$4 != \"PASS\" {n++} END{print n+0}' '$out/evidence_index.tsv')\" -eq 0" || exit 91
run_recorded WORKTREE_STATUS git status --short || exit 92
run_recorded EVIDENCE_COUNTS bash -o pipefail -c \
    "find '$out' -path '$out/commands' -prune -o -type f -print | wc -l; du -sh '$out'" || exit 93
run_recorded EVIDENCE_MANIFEST bash -o pipefail -c \
    "find '$out' -path '$out/commands' -prune -o -type f ! -name evidence_manifest.sha256 -print0 | LC_ALL=C sort -z | xargs -0 sha256sum > '$out/evidence_manifest.sha256'" || exit 94
exit 0
