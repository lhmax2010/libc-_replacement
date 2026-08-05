#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
root21=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
root22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0"
src21="$root21/home/abuild/rpmbuild/BUILD/libcxx-runtimes-21.1.1"
src22="$root22/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
qemu=/usr/bin/qemu-arm-static
ledger="$out/commands/13_ehabi_macro_and_compact_commands.log"
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
run_recorded CONFIG_MACROS_21 bash -o pipefail -c \
    "printf '#include \"__cxxabi_config.h\"\\n' | '$qemu' -L '$root21' '$root21/usr/bin/clang++' --target=armv7l-tizen-linux-gnueabi --sysroot='$root21' -resource-dir '$root21/usr/lib/clang/21' -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -I'$src21/libcxxabi/include' -dM -E -x c++ - > '$out/21___cxxabi_config_clang_dM_E.full.txt'" || exit 90
run_recorded CONFIG_MACROS_22 bash -o pipefail -c \
    "printf '#include \"__cxxabi_config.h\"\\n' | '$qemu' -L '$root22' '$root22/usr/bin/clang++' --target=armv7l-tizen-linux-gnueabi --sysroot='$root22' -resource-dir '$root22/usr/lib/clang/22' -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -I'$src22/libcxxabi/include' -dM -E -x c++ - > '$out/22___cxxabi_config_clang_dM_E.full.txt'" || exit 91
run_recorded CONFIG_MACRO_FILTER bash -o pipefail -c \
    "for v in 21 22; do printf 'VERSION=%s\n' \"\$v\"; rg '^#define (_LIBCXXABI_ARM_EHABI|__ARM_EABI_UNWINDER__)' '$out/'\"\$v\"'___cxxabi_config_clang_dM_E.full.txt' || true; done > '$out/ehabi_macro_after_cxxabi_config.raw.txt'" || exit 92
run_recorded COMPACT_COMMAND_21 bash -o pipefail -c \
    "rg '^/bin/.*clang\\+\\+.*cxa_personality\\.cpp$' '$out/21_cxa_personality_resolved_compile_command.raw.txt' > '$out/21_cxa_personality_compile_command.only.txt'" || exit 93
run_recorded COMPACT_COMMAND_22 bash -o pipefail -c \
    "rg '^/bin/.*clang\\+\\+.*cxa_personality\\.cpp$' '$out/22_cxa_personality_resolved_compile_command.raw.txt' > '$out/22_cxa_personality_compile_command.only.txt'" || exit 94
exit 0
