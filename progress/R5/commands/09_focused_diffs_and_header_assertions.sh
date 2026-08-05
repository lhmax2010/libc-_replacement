#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
src21=/home/toolchain/plan_evaluation/p1/libcxx_packaging/source/libcxx-runtimes-21.1.1
root21=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
root22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0"
src22="$root22/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
ledger="$out/commands/09_focused_diffs_and_header_assertions.attempt2.log"
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
for rel in include/__cxxabi_config.h src/cxa_exception.cpp src/cxa_exception.h; do
    safe=${rel//\//_}
    run_recorded "FULL_DIFF_$safe" bash -o pipefail -c \
        "diff -u --label llvm-21.1.1/libcxxabi/$rel --label llvm-22.1.8/libcxxabi/$rel '$src21/libcxxabi/$rel' '$src22/libcxxabi/$rel' > '$out/${safe}_21_1_1_to_22_1_8.full.diff'"
    rc=$?
    test "$rc" -eq 1 || exit 90
done
run_recorded SRC_CMAKE_FULL_DIFF bash -o pipefail -c \
    "diff -u --label llvm-21.1.1/libcxxabi/src/CMakeLists.txt --label llvm-22.1.8/libcxxabi/src/CMakeLists.txt '$src21/libcxxabi/src/CMakeLists.txt' '$src22/libcxxabi/src/CMakeLists.txt' > '$out/libcxxabi_src_CMakeLists_21_1_1_to_22_1_8.full.diff'"
src_cmake_rc=$?
test "$src_cmake_rc" -eq 1 || exit 91
run_recorded ROOT_CMAKE_UNWINDER_HUNK_ASSERT bash -o pipefail -c \
    "if rg -n '^[+-].*(LIBCXXABI_USE_LLVM_UNWINDER|LIBCXXABI_ENABLE_STATIC_UNWINDER|LIBCXXABI_LIBUNWIND|LLVM_NATIVE_ARCH MATCHES ARM)' '$out/libcxxabi_CMakeLists_21_1_1_to_22_1_8.full.diff' > '$out/root_CMake_unwinder_changed_hunks.raw.txt'; then exit 1; else printf 'NO_UNWINDER_RELATED_CHANGED_HUNK\n' > '$out/root_CMake_unwinder_changed_hunks.raw.txt'; fi" || exit 92
run_recorded PERSONALITY_INCLUDES_21 bash -o pipefail -c \
    "rg -n -C 3 '^#include .*unwind' '$src21/libcxxabi/src/cxa_personality.cpp' > '$out/21_cxa_personality_unwind_includes.raw.txt'" || exit 93
run_recorded PERSONALITY_INCLUDES_22 bash -o pipefail -c \
    "rg -n -C 3 '^#.*include.*unwind' '$src22/libcxxabi/src/cxa_personality.cpp' '$src22/libcxxabi/src/cxa_exception.h' > '$out/22_cxa_personality_unwind_includes.raw.txt'" || exit 94
run_recorded ACTUAL_CLANG_UNWIND_21 bash -o pipefail -c \
    "nl -ba '$root21/usr/lib/clang/21/include/unwind.h' > '$out/21_actual_clang_unwind_h.full.txt'" || exit 95
run_recorded ACTUAL_CLANG_UNWIND_22 bash -o pipefail -c \
    "nl -ba '$root22/usr/lib/clang/22/include/unwind.h' > '$out/22_actual_clang_unwind_h.full.txt'" || exit 96
run_recorded GCC_ARM_COMMON_RELEVANT_21 bash -o pipefail -c \
    "nl -ba '$root21/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h' | sed -n '24,222p' > '$out/21_gcc_unwind_arm_common_24_222.raw.txt'" || exit 97
run_recorded GCC_ARM_COMMON_RELEVANT_22 bash -o pipefail -c \
    "nl -ba '$root22/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h' | sed -n '24,222p' > '$out/22_gcc_unwind_arm_common_24_222.raw.txt'" || exit 98

: >"$out/arm_eabi_unwinder_macro_assertion.tsv"
: >"$out/preprocessed_unwind_declaration_assertion.tsv"
for v in 21 22; do
    run_recorded "MACRO_ABSENCE_$v" rg -n '^#define __ARM_EABI_UNWINDER__' "$out/${v}_armv7l_clang_dM_E.full.txt"
    rc=$?
    printf 'VERSION=%s\tEXPECTED_RG_EXIT=1\tACTUAL_RG_EXIT=%s\tINTERPRETATION=%s\n' "$v" "$rc" "$([ "$rc" -eq 1 ] && printf NOT_DEFINED || printf UNEXPECTED)" >>"$out/arm_eabi_unwinder_macro_assertion.tsv"
    test "$rc" -eq 1 || exit 99
    run_recorded "PREPROCESSED_DECL_ABSENCE_$v" rg -n '__gnu_unwind_frame' "$out/${v}_unwind_include_preprocessed.full.txt"
    rc=$?
    printf 'VERSION=%s\tEXPECTED_RG_EXIT=1\tACTUAL_RG_EXIT=%s\tINTERPRETATION=%s\n' "$v" "$rc" "$([ "$rc" -eq 1 ] && printf DECLARATION_NOT_VISIBLE || printf UNEXPECTED)" >>"$out/preprocessed_unwind_declaration_assertion.tsv"
    test "$rc" -eq 1 || exit 100
done
run_recorded INCLUDE_CHAIN_FIRST_HEADERS bash -o pipefail -c \
    "for v in 21 22; do printf 'VERSION=%s\\t' \"\$v\"; rg -m1 '^\\. .*unwind\\.h$' '$out/'\"\$v\"'_unwind_include_chain.full.txt' | sed 's/^\\. /ACTUAL_HEADER=/'; done > '$out/actual_unwind_header_paths.tsv'" || exit 101
run_recorded GCC_DECLARATION_ASSERT_21 rg -n '__gnu_unwind_frame' "$root21/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h" || exit 102
run_recorded GCC_DECLARATION_ASSERT_22 rg -n '__gnu_unwind_frame' "$root22/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h" || exit 103
exit 0
