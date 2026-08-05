#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R6"
raw="$out/raw"
src="$repo/codes/llvm"
commit=8119f1854948b50358bbfaea08f207f51970f06c
patch="$out/upstream/$commit.github.patch"
ledger="$out/commands/03a_strict_patch_line_validation.log"
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

cd "$src" || exit 125
run_recorded PATCH_STILL_VALID bash -o pipefail -c \
    "test \"\$(sha256sum '$patch' | awk '{print \$1}')\" = d4a8fdcec396dbd7a2d8c70904be674b8bf03e5b3177d0116619435e7599b3d0; git apply --check '$patch'; test \"\$(git apply --numstat '$patch')\" = \$'2\\t0\\tlibcxxabi/src/cxa_personality.cpp'" || exit 90
run_recorded STRICT_ADDED_LINE_CLASSIFICATION bash -o pipefail -c \
    "set -euo pipefail; awk 'BEGIN{d=0} /^diff --git /{d=1} d && /^\\+/{if (substr(\$0,1,3)!=\"+++\") {if (\$0==\"+\") printf \"%d:<BLANK>\\n\",NR; else printf \"%d:%s\\n\",NR,\$0}}' '$patch' > '$raw/upstream_patch_added_lines.strict.txt'; test \"\$(wc -l < '$raw/upstream_patch_added_lines.strict.txt')\" -eq 2; test \"\$(rg -c '^.*:\+extern \"C\" _Unwind_Reason_Code __gnu_unwind_frame\\(_Unwind_Exception\\*, _Unwind_Context\\*\\);$' '$raw/upstream_patch_added_lines.strict.txt')\" -eq 1; test \"\$(rg -c '^.*:<BLANK>$' '$raw/upstream_patch_added_lines.strict.txt')\" -eq 1" || exit 91
run_recorded STRICT_FILE_SCOPE bash -o pipefail -c \
    "set -euo pipefail; test \"\$(rg -c '^diff --git ' '$patch')\" -eq 1; test \"\$(sed -n 's#^diff --git a/\\([^ ]*\\) b/.*#\\1#p' '$patch')\" = libcxxabi/src/cxa_personality.cpp" || exit 92
printf 'ONE_FILE_TWO_ADDITIONS_ONE_DECLARATION_ONE_BLANK_PASS\n' >"$out/upstream_patch_strict_conclusion.txt"
exit 0
