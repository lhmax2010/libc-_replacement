#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R6"
raw="$out/raw"
src="$repo/codes/llvm"
commit=8119f1854948b50358bbfaea08f207f51970f06c
patch="$out/upstream/$commit.github.patch"
original_head=fd7a4929c0e424d564cdd1f0c55af9153dcea321
ledger="$out/commands/04_git_am_backport.log"
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
run_recorded PRE_AM_ASSERT bash -o pipefail -c \
    "test \"\$(git branch --show-current)\" = tizen_base; test \"\$(git rev-parse HEAD)\" = '$original_head'; test -z \"\$(git status --porcelain=v1)\"; git apply --check '$patch'" || exit 90
run_recorded GIT_AM git am --no-gpg-sign "$patch" || exit 91
run_recorded AM_INTERMEDIATE_IDENTITY bash -o pipefail -c \
    "{ git rev-parse HEAD; git show -s --format=fuller HEAD; } > '$raw/git_am_intermediate_identity.raw.txt'" || exit 92
run_recorded SAVE_ORIGINAL_MESSAGE bash -o pipefail -c \
    "git log -1 --format=%B > '$out/upstream/original_commit_message.txt'; cp '$out/upstream/original_commit_message.txt' '$out/upstream/backport_commit_message.txt'" || exit 93
run_recorded APPEND_REQUIRED_TRAILERS git interpret-trailers --in-place \
    --trailer "Upstream-Status: Backport [$commit]" \
    --trailer "Link: https://github.com/llvm/llvm-project/pull/189787" \
    "$out/upstream/backport_commit_message.txt" || exit 94
run_recorded AMEND_MESSAGE git commit --amend --no-gpg-sign -F "$out/upstream/backport_commit_message.txt" || exit 95
run_recorded POST_AM_IDENTITY bash -o pipefail -c \
    "{ printf 'NEW_COMMIT='; git rev-parse HEAD; printf 'BRANCH='; git branch --show-current; printf 'STATUS_BEGIN\\n'; git status --porcelain=v1; printf 'STATUS_END\\n'; } > '$out/backport_commit_identity.txt'" || exit 96
run_recorded POST_AM_CLEAN_ASSERT bash -o pipefail -c \
    "test -z \"\$(git status --porcelain=v1)\"" || exit 97
run_recorded COMMIT_SCOPE_ASSERT bash -o pipefail -c \
    "test \"\$(git diff-tree --no-commit-id --name-only -r HEAD)\" = libcxxabi/src/cxa_personality.cpp; test \"\$(git diff-tree --no-commit-id --numstat -r HEAD)\" = \$'2\\t0\\tlibcxxabi/src/cxa_personality.cpp'" || exit 98
run_recorded COMMIT_MESSAGE_ASSERT bash -o pipefail -c \
    "git log -1 --format=%B > '$raw/backport_commit_message.actual.txt'; test \"\$(rg -c '^Upstream-Status: Backport \\[8119f1854948b50358bbfaea08f207f51970f06c\\]$' '$raw/backport_commit_message.actual.txt')\" -eq 1; test \"\$(rg -c '^Link: https://github.com/llvm/llvm-project/pull/189787$' '$raw/backport_commit_message.actual.txt')\" -eq 1" || exit 99
run_recorded GIT_SHOW bash -o pipefail -c \
    "git show --format=fuller --stat --patch HEAD > '$raw/backport_git_show.full.txt'" || exit 100
run_recorded PERSONALITY_SHA bash -o pipefail -c \
    "sha256sum '$src/libcxxabi/src/cxa_personality.cpp' > '$out/postpatch_cxa_personality_sha256.tsv'" || exit 101
run_recorded POSTPATCH_OCCURRENCES bash -o pipefail -c \
    "rg -n -C 5 '__gnu_unwind_frame' '$src/libcxxabi/src/cxa_personality.cpp' > '$raw/postpatch_gnu_unwind_frame_occurrences.raw.txt'" || exit 102
run_recorded POSTPATCH_COUNT_ORDER_ASSERT bash -o pipefail -c \
    "set -euo pipefail; test \"\$(rg -c '^extern \"C\" _Unwind_Reason_Code __gnu_unwind_frame' '$src/libcxxabi/src/cxa_personality.cpp')\" -eq 1; test \"\$(rg -c 'switch \\(__gnu_unwind_frame' '$src/libcxxabi/src/cxa_personality.cpp')\" -eq 1; decl=\$(rg -n '^extern \"C\" _Unwind_Reason_Code __gnu_unwind_frame' '$src/libcxxabi/src/cxa_personality.cpp' | cut -d: -f1); call=\$(rg -n 'switch \\(__gnu_unwind_frame' '$src/libcxxabi/src/cxa_personality.cpp' | cut -d: -f1); test \"\$decl\" -lt \"\$call\"; printf 'DECLARATION_LINE=%s\\nCALL_LINE=%s\\n' \"\$decl\" \"\$call\" > '$out/postpatch_declaration_call_order.tsv'" || exit 103
run_recorded POSTPATCH_GUARD_STACK bash -o pipefail -c \
    "awk -v needle='__gnu_unwind_frame' -f '$repo/progress/R5/commands/pp_stack.awk' '$src/libcxxabi/src/cxa_personality.cpp' > '$out/postpatch_gnu_unwind_frame_guard_stack.tsv'" || exit 104
run_recorded POSTPATCH_GUARD_ASSERT bash -o pipefail -c \
    "test \"\$(rg -c 'LEVEL_1_OPEN[[:space:]]+971:#if !defined\\(_LIBCXXABI_ARM_EHABI\\)' '$out/postpatch_gnu_unwind_frame_guard_stack.tsv')\" -eq 2; test \"\$(rg -c 'LEVEL_1_CURRENT_BRANCH[[:space:]]+1126:#else' '$out/postpatch_gnu_unwind_frame_guard_stack.tsv')\" -eq 2" || exit 105
exit 0
