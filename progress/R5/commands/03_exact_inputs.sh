#!/usr/bin/env bash
set -u

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5/raw"
src211=/home/toolchain/plan_evaluation/p1/libcxx_packaging/source/libcxx-runtimes-21.1.1
scratch211=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
scratch2218="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0"
stagea=/home/toolchain/development/libc++_replacement_bak/wave1_execution/stageA

run_recorded() {
    local id=$1
    shift
    local log="$out/${id}.log"
    local rc
    {
        printf 'WORKING_DIRECTORY=%s\n' "$PWD"
        printf 'COMMAND_BEGIN\n'
        printf '%q ' "$@"
        printf '\nCOMMAND_END\n'
    } >"$log"
    "$@" >>"$log" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n' "$rc" >>"$log"
    printf '%s\t%s\n' "$id" "$rc"
    return 0
}

cd "$repo" || exit 125
run_recorded 21_exact_candidates bash -o pipefail -c \
    "for p in \
 '$src211/libcxxabi/src/cxa_personality.cpp' \
 '$scratch211/home/abuild/rpmbuild/BUILD/llvm-21.1.1/libcxxabi/src/cxa_personality.cpp' \
 '$scratch2218/home/abuild/rpmbuild/BUILD/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp' \
 '$scratch211/usr/lib/clang/21/include/unwind.h' \
 '$scratch2218/usr/lib/clang/22/include/unwind.h'; do \
 if test -e \"\$p\"; then printf 'FOUND\\t%s\\n' \"\$p\"; else printf 'NOT_FOUND\\t%s\\n' \"\$p\"; fi; done"
run_recorded 22_211_build_dir ls -la "$scratch211/home/abuild/rpmbuild/BUILD"
run_recorded 23_2218_build_dir ls -la "$scratch2218/home/abuild/rpmbuild/BUILD"
run_recorded 24_211_repo_tree find /home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/repos/tizen_unified_standard/armv7l -maxdepth 5 -type f -printf '%p\n'
run_recorded 25_211_source_tree find /home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/sources/tizen_unified_standard/libcxx-runtimes-21.1.1-2 -maxdepth 3 -printf '%y\t%p\t%l\n'
run_recorded 26_stagea_candidate_tree find "$stagea/candidate_build" -maxdepth 5 -type f -printf '%p\n'
run_recorded 27_stagea_log_names find "$stagea/logs" -maxdepth 3 -type f -printf '%p\n'
run_recorded 28_personality_sha bash -o pipefail -c \
    "sha256sum '$src211/libcxxabi/src/cxa_personality.cpp' '$scratch211/home/abuild/rpmbuild/BUILD/llvm-21.1.1/libcxxabi/src/cxa_personality.cpp' '$scratch2218/home/abuild/rpmbuild/BUILD/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp'"
run_recorded 29_source_versions bash -o pipefail -c \
    "grep -nE 'set\\(LLVM_VERSION_(MAJOR|MINOR|PATCH)' '$src211/cmake/Modules/LLVMVersion.cmake' '$scratch2218/home/abuild/rpmbuild/BUILD/llvm-22.1.8/cmake/Modules/LLVMVersion.cmake'"
exit 0
