#!/usr/bin/env bash
set -u

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
file22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp"
file21=/home/toolchain/plan_evaluation/p1/libcxx_packaging/source/libcxx-runtimes-21.1.1/libcxxabi/src/cxa_personality.cpp
log="$out/commands/04_failure_context.log"
: >"$log"

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
    } >>"$log"
    "$@" >>"$log" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n' "$rc" >>"$log"
    printf '%s\t%s\n' "$label" "$rc"
    return "$rc"
}

cd "$repo" || exit 125
run_recorded ASSERT_FILES test -f "$file22" -a -f "$file21" || exit 90
run_recorded CONTEXT_1100_1170 bash -o pipefail -c \
    "nl -ba '$file22' | sed -n '1100,1170p' > '$out/22_cxa_personality_1100_1170.raw.txt'" || exit 91
run_recorded ALL_OCCURRENCES_22 bash -o pipefail -c \
    "rg -n -C 8 '__gnu_unwind_frame' '$file22' > '$out/22_gnu_unwind_frame_occurrences.raw.txt'" || exit 92
run_recorded PP_STACK_22 bash -o pipefail -c \
    "awk -v needle='__gnu_unwind_frame' -f '$out/commands/pp_stack.awk' '$file22' > '$out/22_gnu_unwind_frame_guard_stack.tsv'" || exit 93
run_recorded ALL_OCCURRENCES_21 bash -o pipefail -c \
    "rg -n -C 8 '__gnu_unwind_frame' '$file21' > '$out/21_gnu_unwind_frame_occurrences.raw.txt'" || true
run_recorded PP_STACK_21 bash -o pipefail -c \
    "awk -v needle='__gnu_unwind_frame' -f '$out/commands/pp_stack.awk' '$file21' > '$out/21_gnu_unwind_frame_guard_stack.tsv'" || true
run_recorded DIRECTIVES_22 bash -o pipefail -c \
    "nl -ba '$file22' | sed -n '1,1170p' | grep -E '[[:space:]]#[[:space:]]*(if|ifdef|ifndef|elif|else|endif)' > '$out/22_preprocessor_directives_through_1170.raw.txt'" || exit 94
run_recorded FAILURE_LINE bash -o pipefail -c \
    "sed -n '1135p' '$file22' | grep -F '__gnu_unwind_frame' > '$out/22_failure_line_1135.raw.txt'" || exit 95
exit 0
