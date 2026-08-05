#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
root21=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
root22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0"
src21=/home/toolchain/plan_evaluation/p1/libcxx_packaging/source/libcxx-runtimes-21.1.1
src22="$root22/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
build21="$root21/home/abuild/rpmbuild/BUILD/libcxx-runtimes-21.1.1/build"
build22="$src22/build"
log21=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/repos/tizen_unified_standard/armv7l/logs/success/libcxx-runtimes-21.1.1-2/log.txt
log22="$repo/progress/T3R2/build_armv7l.full.log"
ledger="$out/commands/08_build_evidence_and_cmake.log"
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
run_recorded ASSERT_INPUTS test -f "$log21" -a -f "$log22" -a -f "$build21/build.ninja" -a -f "$build22/build.ninja" -a -f "$build21/CMakeCache.txt" -a -f "$build22/CMakeCache.txt" || exit 90
run_recorded INPUT_SHA256 bash -o pipefail -c \
    "sha256sum '$log21' '$log22' '$build21/build.ninja' '$build22/build.ninja' '$build21/CMakeCache.txt' '$build22/CMakeCache.txt' '$src21/libcxxabi/src/cxa_personality.cpp' '$src22/libcxxabi/src/cxa_personality.cpp' > '$out/build_evidence_input_sha256.tsv'" || exit 91
run_recorded SUCCESS_LOG_PERSONALITY bash -o pipefail -c \
    "rg -n -C 3 'cxa_personality\\.cpp' '$log21' > '$out/21_success_log_cxa_personality.raw.txt'" || exit 92
run_recorded SUCCESS_BUILD_PROOF bash -o pipefail -c \
    "rg -n 'Checking for unpackaged file|Wrote: .*libc\\+\\+|Wrote: .*libcxx-runtimes|finished \"build libcxx-runtimes\\.spec\"' '$log21' > '$out/21_success_log_products.raw.txt'" || exit 93
run_recorded BUILD_NINJA_PERSONALITY_21 bash -o pipefail -c \
    "rg -n -A 8 -B 2 '^build libcxxabi/src/CMakeFiles/cxxabi_(shared|static)_objects\\.dir/cxa_personality\\.cpp\\.o:' '$build21/build.ninja' > '$out/21_build_ninja_cxa_personality.raw.txt'" || exit 94
run_recorded BUILD_NINJA_PERSONALITY_22 bash -o pipefail -c \
    "rg -n -A 8 -B 2 '^build libcxxabi/src/CMakeFiles/cxxabi_(shared|static)_objects\\.dir/cxa_personality\\.cpp\\.o:' '$build22/build.ninja' > '$out/22_build_ninja_cxa_personality.raw.txt'" || exit 95
run_recorded FAILURE_LOG_CONTEXT_22 bash -o pipefail -c \
    "rg -n -C 12 'cxa_personality\\.cpp|__gnu_unwind_frame' '$log22' > '$out/22_failure_log_cxa_personality.raw.txt'" || exit 96
run_recorded CMAKE_INVOCATION_21 bash -o pipefail -c \
    "rg -n -C 2 'cmake .*LLVM_ENABLE_RUNTIMES|LIBCXXABI_USE_LLVM_UNWINDER|LIBCXXABI_ENABLE_STATIC_UNWINDER|LIBCXXABI_USE_COMPILER_RT' '$log21' > '$out/21_success_log_cmake_unwinder.raw.txt'" || exit 97
run_recorded CMAKE_INVOCATION_22 bash -o pipefail -c \
    "rg -n -C 2 'cmake .*LLVM_ENABLE_RUNTIMES|LIBCXXABI_USE_LLVM_UNWINDER|LIBCXXABI_ENABLE_STATIC_UNWINDER|LIBCXXABI_USE_COMPILER_RT' '$log22' > '$out/22_failure_log_cmake_unwinder.raw.txt'" || exit 98
run_recorded CMAKE_CACHE_21 bash -o pipefail -c \
    "rg -n '^(LIBCXXABI_USE_LLVM_UNWINDER|LIBCXXABI_ENABLE_STATIC_UNWINDER|LIBCXXABI_USE_COMPILER_RT|LLVM_ENABLE_RUNTIMES|LLVM_NATIVE_ARCH|CMAKE_(C|CXX)_COMPILER):' '$build21/CMakeCache.txt' > '$out/21_CMakeCache_unwinder.raw.txt'" || exit 99
run_recorded CMAKE_CACHE_22 bash -o pipefail -c \
    "rg -n '^(LIBCXXABI_USE_LLVM_UNWINDER|LIBCXXABI_ENABLE_STATIC_UNWINDER|LIBCXXABI_USE_COMPILER_RT|LLVM_ENABLE_RUNTIMES|LLVM_NATIVE_ARCH|CMAKE_(C|CXX)_COMPILER):' '$build22/CMakeCache.txt' > '$out/22_CMakeCache_unwinder.raw.txt'" || exit 100
run_recorded CONFIG_EHABI_21 bash -o pipefail -c \
    "rg -n -C 8 '_LIBCXXABI_ARM_EHABI|__ARM_EABI_UNWINDER__' '$src21/libcxxabi/include/__cxxabi_config.h' > '$out/21___cxxabi_config_EHABI.raw.txt'" || exit 101
run_recorded CONFIG_EHABI_22 bash -o pipefail -c \
    "rg -n -C 8 '_LIBCXXABI_ARM_EHABI|__ARM_EABI_UNWINDER__' '$src22/libcxxabi/include/__cxxabi_config.h' > '$out/22___cxxabi_config_EHABI.raw.txt'" || exit 102
run_recorded CMAKE_FOCUSED_21 bash -o pipefail -c \
    "{ nl -ba '$src21/libcxxabi/CMakeLists.txt' | sed -n '45,65p'; nl -ba '$src21/libcxxabi/CMakeLists.txt' | sed -n '465,505p'; rg -n -C 5 'LIBCXXABI_USE_LLVM_UNWINDER|LIBCXXABI_ENABLE_STATIC_UNWINDER' '$src21/libcxxabi/src/CMakeLists.txt'; } > '$out/21_CMake_unwinder_focused.raw.txt'" || exit 103
run_recorded CMAKE_FOCUSED_22 bash -o pipefail -c \
    "{ nl -ba '$src22/libcxxabi/CMakeLists.txt' | sed -n '45,65p'; nl -ba '$src22/libcxxabi/CMakeLists.txt' | sed -n '469,509p'; rg -n -C 5 'LIBCXXABI_USE_LLVM_UNWINDER|LIBCXXABI_ENABLE_STATIC_UNWINDER' '$src22/libcxxabi/src/CMakeLists.txt'; } > '$out/22_CMake_unwinder_focused.raw.txt'" || exit 104
run_recorded SOURCE_BUILD_PERSONALITY_SHA bash -o pipefail -c \
    "sha256sum '$src21/libcxxabi/src/cxa_personality.cpp' '$root21/home/abuild/rpmbuild/BUILD/libcxx-runtimes-21.1.1/libcxxabi/src/cxa_personality.cpp' '$src22/libcxxabi/src/cxa_personality.cpp' > '$out/source_vs_build_personality_sha256.tsv'" || exit 105
exit 0
