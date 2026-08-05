#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
src21=/home/toolchain/plan_evaluation/p1/libcxx_packaging/source/libcxx-runtimes-21.1.1
src22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
log="$out/commands/05_version_diff.log"
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
run_recorded PERSONALITY_FULL_DIFF bash -o pipefail -c \
    "diff -u --label llvm-21.1.1/libcxxabi/src/cxa_personality.cpp --label llvm-22.1.8/libcxxabi/src/cxa_personality.cpp '$src21/libcxxabi/src/cxa_personality.cpp' '$src22/libcxxabi/src/cxa_personality.cpp' > '$out/cxa_personality_21_1_1_to_22_1_8.full.diff'"
personality_rc=$?
test "$personality_rc" -eq 1 || exit 90

run_recorded CMAKE_FULL_DIFF bash -o pipefail -c \
    "diff -u --label llvm-21.1.1/libcxxabi/CMakeLists.txt --label llvm-22.1.8/libcxxabi/CMakeLists.txt '$src21/libcxxabi/CMakeLists.txt' '$src22/libcxxabi/CMakeLists.txt' > '$out/libcxxabi_CMakeLists_21_1_1_to_22_1_8.full.diff'"
cmake_rc=$?
test "$cmake_rc" -eq 1 || exit 91

run_recorded RELEVANT_LIST_21 bash -o pipefail -c \
    "{ rg -l -i 'EHABI|_Unwind|unwind' '$src21/libcxxabi/src' '$src21/libcxxabi/include' -g '*.{h,hpp,cpp,c,inc,S}' || true; find '$src21/libcxxabi/src' '$src21/libcxxabi/include' -type f \\( -name 'cxa_exception*' -o -name 'cxa_handlers*' -o -iname '*unwind*' \\) -print; } | sed 's#^$src21/libcxxabi/##' | LC_ALL=C sort -u > '$out/ehabi_unwind_files_21_1_1.txt'" || exit 92
run_recorded RELEVANT_LIST_22 bash -o pipefail -c \
    "{ rg -l -i 'EHABI|_Unwind|unwind' '$src22/libcxxabi/src' '$src22/libcxxabi/include' -g '*.{h,hpp,cpp,c,inc,S}' || true; find '$src22/libcxxabi/src' '$src22/libcxxabi/include' -type f \\( -name 'cxa_exception*' -o -name 'cxa_handlers*' -o -iname '*unwind*' \\) -print; } | sed 's#^$src22/libcxxabi/##' | LC_ALL=C sort -u > '$out/ehabi_unwind_files_22_1_8.txt'" || exit 93

printf 'path\tstatus\tsha256_21_1_1\tsha256_22_1_8\n' >"$out/ehabi_unwind_file_delta.tsv"
cat "$out/ehabi_unwind_files_21_1_1.txt" "$out/ehabi_unwind_files_22_1_8.txt" | LC_ALL=C sort -u | while IFS= read -r rel; do
    f21="$src21/libcxxabi/$rel"
    f22="$src22/libcxxabi/$rel"
    if test -f "$f21" && test -f "$f22"; then
        sha21=$(sha256sum "$f21" | awk '{print $1}')
        sha22=$(sha256sum "$f22" | awk '{print $1}')
        if cmp -s "$f21" "$f22"; then status=SAME; else status=CHANGED; fi
    elif test -f "$f21"; then
        sha21=$(sha256sum "$f21" | awk '{print $1}')
        sha22=NOT_PRESENT
        status=REMOVED_IN_22
    else
        sha21=NOT_PRESENT
        sha22=$(sha256sum "$f22" | awk '{print $1}')
        status=ADDED_IN_22
    fi
    printf '%s\t%s\t%s\t%s\n' "$rel" "$status" "$sha21" "$sha22" >>"$out/ehabi_unwind_file_delta.tsv"
done
printf 'FILE_DELTA_LOOP_EXIT_CODE=%s\n' "$?" >>"$log"

printf 'path\tadded_lines\tremoved_lines\n' >"$out/ehabi_unwind_changed_file_diffstat.tsv"
awk -F '\t' 'NR>1 && $2=="CHANGED" {print $1}' "$out/ehabi_unwind_file_delta.tsv" | while IFS= read -r rel; do
    counts=$(diff -u "$src21/libcxxabi/$rel" "$src22/libcxxabi/$rel" | awk 'BEGIN{a=0;d=0} /^\+[^+]/{a++} /^-[^-]/{d++} END{printf "%d\t%d",a,d}')
    printf '%s\t%s\n' "$rel" "$counts" >>"$out/ehabi_unwind_changed_file_diffstat.tsv"
done
printf 'DIFFSTAT_LOOP_EXIT_CODE=%s\n' "$?" >>"$log"

run_recorded CMAKE_UNWINDER_21 bash -o pipefail -c \
    "rg -n -C 5 'LIBCXXABI_USE_LLVM_UNWINDER|ARM_EHABI|_LIBUNWIND|unwind' '$src21/libcxxabi/CMakeLists.txt' > '$out/21_CMakeLists_unwinder.raw.txt'" || exit 94
run_recorded CMAKE_UNWINDER_22 bash -o pipefail -c \
    "rg -n -C 5 'LIBCXXABI_USE_LLVM_UNWINDER|ARM_EHABI|_LIBUNWIND|unwind' '$src22/libcxxabi/CMakeLists.txt' > '$out/22_CMakeLists_unwinder.raw.txt'" || exit 95
run_recorded OPTION_IMPACT_22 bash -o pipefail -c \
    "rg -n -C 4 'LIBCXXABI_USE_LLVM_UNWINDER|_LIBCXXABI_ARM_EHABI|__ARM_EABI_UNWINDER__|__gnu_unwind_frame' '$src22/libcxxabi' -g 'CMakeLists.txt' -g '*.cmake' -g '*.{h,cpp,c}' > '$out/22_unwinder_option_impact.raw.txt'" || exit 96
run_recorded OPTION_IMPACT_21 bash -o pipefail -c \
    "rg -n -C 4 'LIBCXXABI_USE_LLVM_UNWINDER|_LIBCXXABI_ARM_EHABI|__ARM_EABI_UNWINDER__|__gnu_unwind_frame' '$src21/libcxxabi' -g 'CMakeLists.txt' -g '*.cmake' -g '*.{h,cpp,c}' > '$out/21_unwinder_option_impact.raw.txt'" || exit 97
exit 0
