#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
build21=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/libcxx-runtimes-21.1.1/build
build22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
target=libcxxabi/src/CMakeFiles/cxxabi_shared_objects.dir/cxa_personality.cpp.o
ledger="$out/commands/12_ninja_command_introspection.log"
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
run_recorded NINJA_TOOL_MODE_ASSERT ninja --version || exit 90
run_recorded PRINT_COMMAND_21 bash -o pipefail -c \
    "ninja -C '$build21' -t commands '$target' > '$out/21_cxa_personality_resolved_compile_command.raw.txt'" || exit 91
run_recorded PRINT_COMMAND_22 bash -o pipefail -c \
    "ninja -C '$build22' -t commands '$target' > '$out/22_cxa_personality_resolved_compile_command.raw.txt'" || exit 92
run_recorded EXTRACT_DEFINES bash -o pipefail -c \
    "for v in 21 22; do printf 'VERSION=%s\n' \"\$v\"; tr ' ' '\n' < '$out/'\"\$v\"'_cxa_personality_resolved_compile_command.raw.txt' | rg '^-D' | LC_ALL=C sort -u; done > '$out/cxa_personality_D_options_comparison.raw.txt'" || exit 93
exit 0
