#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R6"
raw="$out/raw"
src="$repo/codes/llvm"
ledger="$out/commands/01_source_identity.attempt2.log"
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
run_recorded SOURCE_GIT_IDENTITY bash -o pipefail -c \
    "{ printf 'SOURCE_PATH=%s\\n' '$src'; printf 'BRANCH='; git branch --show-current; printf 'HEAD='; git rev-parse HEAD; printf 'REMOTE_BEGIN\\n'; git remote -v; printf 'REMOTE_END\\n'; printf 'STATUS_PORCELAIN_BEGIN\\n'; git status --porcelain=v1; printf 'STATUS_PORCELAIN_END\\n'; } > '$raw/source_git_identity.raw.txt'" || exit 90
run_recorded CLEAN_ASSERT bash -o pipefail -c \
    "test -z \"\$(git status --porcelain=v1)\"" || exit 91
run_recorded BRANCH_REFS bash -o pipefail -c \
    "{ git branch -avv; git show-ref --verify refs/heads/tizen_base; } > '$raw/source_branch_refs.raw.txt'" || exit 92
run_recorded VERSION_SOURCE bash -o pipefail -c \
    "{ rg -n 'set\\(LLVM_VERSION_(MAJOR|MINOR|PATCH)' '$src/cmake/Modules/LLVMVersion.cmake'; rg -n 'set\\(LLVM_VERSION_(MAJOR|MINOR|PATCH)' '$src/llvm/CMakeLists.txt' || true; } > '$raw/source_version.raw.txt'" || exit 93
run_recorded VERSION_ASSERT bash -o pipefail -c \
    "test \"\$(awk '\$1==\"set(LLVM_VERSION_MAJOR\" {gsub(/\\)/,\"\",\$2); print \$2}' '$src/cmake/Modules/LLVMVersion.cmake')\" = 22 && test \"\$(awk '\$1==\"set(LLVM_VERSION_MINOR\" {gsub(/\\)/,\"\",\$2); print \$2}' '$src/cmake/Modules/LLVMVersion.cmake')\" = 1 && test \"\$(awk '\$1==\"set(LLVM_VERSION_PATCH\" {gsub(/\\)/,\"\",\$2); print \$2}' '$src/cmake/Modules/LLVMVersion.cmake')\" = 8" || exit 94
run_recorded PERSONALITY_OCCURRENCES bash -o pipefail -c \
    "rg -n -C 5 '__gnu_unwind_frame' '$src/libcxxabi/src/cxa_personality.cpp' > '$raw/prepatch_gnu_unwind_frame_occurrences.raw.txt'" || exit 95
run_recorded DECLARATION_ABSENT rg -n 'extern "C" _Unwind_Reason_Code __gnu_unwind_frame' "$src/libcxxabi/src/cxa_personality.cpp"
decl_rc=$?
printf 'EXPECTED_RG_EXIT=1\tACTUAL_RG_EXIT=%s\n' "$decl_rc" >"$out/prepatch_declaration_absence.tsv"
test "$decl_rc" -eq 1 || exit 96
run_recorded TARBALL_REFERENCE_SEARCH bash -o pipefail -c \
    "rg -n -C 2 '16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e|llvm-22\\.1\\.8\\.tar\\.gz' '$repo/progress/R3' '$repo/docs/progress/R3' '$repo/progress/R4' '$repo/docs/progress/R4' '$repo/progress/T3R' '$repo/progress/T3R2' 2>&1 > '$raw/tarball_reference_search.raw.txt'" || exit 97
exit 0
