#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
root21=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
root22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0"
clang21="$root21/usr/bin/clang++"
clang22="$root22/usr/bin/clang++"
qemu=/usr/bin/qemu-arm-static
log="$out/commands/07_header_and_macro_probe.log"
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
run_recorded ASSERT_QEMU test -x "$qemu" || exit 90
run_recorded GCC_COMMON_SYMBOLS bash -o pipefail -c \
    "for f in '$root21/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h' '$root22/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h'; do printf 'FILE=%s\\n' \"\$f\"; rg -n -C 12 '__gnu_unwind_frame|ARM_EABI_UNWINDER|_Unwind_Control_Block' \"\$f\" || true; done > '$out/gcc_unwind_arm_common_symbols.raw.txt'" || exit 91
run_recorded GCC_COMMON_GUARD_STACK_21 bash -o pipefail -c \
    "awk -v needle='__gnu_unwind_frame' -f '$out/commands/pp_stack.awk' '$root21/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h' > '$out/21_gcc_gnu_unwind_frame_guard_stack.tsv'" || exit 92
run_recorded GCC_COMMON_GUARD_STACK_22 bash -o pipefail -c \
    "awk -v needle='__gnu_unwind_frame' -f '$out/commands/pp_stack.awk' '$root22/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h' > '$out/22_gcc_gnu_unwind_frame_guard_stack.tsv'" || exit 93
run_recorded HEADER_SHA bash -o pipefail -c \
    "sha256sum '$root21/usr/lib/clang/21/include/unwind.h' '$root22/usr/lib/clang/22/include/unwind.h' '$root21/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h' '$root22/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h' '$root21/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h' '$root22/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h' > '$out/unwind_header_sha256.tsv'" || exit 94
run_recorded CLANG_UNWIND_DIFF bash -o pipefail -c \
    "diff -u --label clang-21/include/unwind.h --label clang-22/include/unwind.h '$root21/usr/lib/clang/21/include/unwind.h' '$root22/usr/lib/clang/22/include/unwind.h' > '$out/clang_unwind_21_to_22.full.diff'"
clang_header_diff_rc=$?
test "$clang_header_diff_rc" -eq 0 -o "$clang_header_diff_rc" -eq 1 || exit 95
run_recorded GCC_UNWIND_DIFF bash -o pipefail -c \
    "diff -u --label root-21/gcc/include/unwind.h --label root-22/gcc/include/unwind.h '$root21/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h' '$root22/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h' > '$out/gcc_unwind_header_21root_to_22root.full.diff'"
gcc_header_diff_rc=$?
test "$gcc_header_diff_rc" -eq 0 -o "$gcc_header_diff_rc" -eq 1 || exit 96
run_recorded QEMU_VERSION_21 "$qemu" -L "$root21" "$clang21" --version || true
run_recorded QEMU_VERSION_22 "$qemu" -L "$root22" "$clang22" --version || true
run_recorded CLANG_SEARCH_21 bash -o pipefail -c \
    "'$qemu' -L '$root21' '$clang21' --target=armv7l-tizen-linux-gnueabi --sysroot='$root21' -resource-dir '$root21/usr/lib/clang/21' -print-search-dirs > '$out/21_clang_search_dirs.raw.txt' 2>&1" || true
run_recorded CLANG_SEARCH_22 bash -o pipefail -c \
    "'$qemu' -L '$root22' '$clang22' --target=armv7l-tizen-linux-gnueabi --sysroot='$root22' -resource-dir '$root22/usr/lib/clang/22' -print-search-dirs > '$out/22_clang_search_dirs.raw.txt' 2>&1" || true
run_recorded MACROS_21 bash -o pipefail -c \
    "printf '' | '$qemu' -L '$root21' '$clang21' --target=armv7l-tizen-linux-gnueabi --sysroot='$root21' -resource-dir '$root21/usr/lib/clang/21' -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -dM -E -x c++ - > '$out/21_armv7l_clang_dM_E.full.txt' 2> '$out/21_armv7l_clang_dM_E.stderr.txt'" || true
run_recorded MACROS_22 bash -o pipefail -c \
    "printf '' | '$qemu' -L '$root22' '$clang22' --target=armv7l-tizen-linux-gnueabi --sysroot='$root22' -resource-dir '$root22/usr/lib/clang/22' -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -dM -E -x c++ - > '$out/22_armv7l_clang_dM_E.full.txt' 2> '$out/22_armv7l_clang_dM_E.stderr.txt'" || true
run_recorded MACRO_FILTER bash -o pipefail -c \
    "for v in 21 22; do printf 'VERSION=%s\\n' \"\$v\"; rg 'ARM|EABI|UNWIND|SJLJ' '$out/'\"\$v\"'_armv7l_clang_dM_E.full.txt' || true; done > '$out/armv7l_macro_comparison.raw.txt'" || exit 97
run_recorded INCLUDE_CHAIN_21 bash -o pipefail -c \
    "printf '#include <unwind.h>\\n' | '$qemu' -L '$root21' '$clang21' --target=armv7l-tizen-linux-gnueabi --sysroot='$root21' -resource-dir '$root21/usr/lib/clang/21' -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -v -H -E -x c++ - > '$out/21_unwind_include_preprocessed.full.txt' 2> '$out/21_unwind_include_chain.full.txt'" || true
run_recorded INCLUDE_CHAIN_22 bash -o pipefail -c \
    "printf '#include <unwind.h>\\n' | '$qemu' -L '$root22' '$clang22' --target=armv7l-tizen-linux-gnueabi --sysroot='$root22' -resource-dir '$root22/usr/lib/clang/22' -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -v -H -E -x c++ - > '$out/22_unwind_include_preprocessed.full.txt' 2> '$out/22_unwind_include_chain.full.txt'" || true
run_recorded PREPROCESSED_SYMBOLS bash -o pipefail -c \
    "for v in 21 22; do printf 'VERSION=%s\\n' \"\$v\"; rg -n -C 3 '__gnu_unwind_frame' '$out/'\"\$v\"'_unwind_include_preprocessed.full.txt' || true; done > '$out/preprocessed_unwind_symbol_comparison.raw.txt'" || exit 98
exit 0
