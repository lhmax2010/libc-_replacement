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
ledger="$out/commands/10_translation_unit_preprocess_probe.attempt2.log"
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
common=(-DLIBCXX_BUILDING_LIBCXXABI -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LIBCPP_BUILDING_LIBRARY -D_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER -D_LIBCXXABI_BUILDING_LIBRARY -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -march=armv7-a -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb -D__SOFTFP__ -std=c++23 -nostdinc++ -fexceptions)

run_recorded PREPROCESS_TU_21 bash -o pipefail -c \
    "'$qemu' -L '$root21' '$root21/usr/bin/clang++' --target=armv7l-tizen-linux-gnueabi --sysroot='$root21' -resource-dir '$root21/usr/lib/clang/21' ${common[*]} -I'$src21/libcxx/src' -I'$src21/libcxxabi/include' -I'$src21/build/include/c++/v1' -H -E '$src21/libcxxabi/src/cxa_personality.cpp' > '$out/21_cxa_personality.preprocessed.ii' 2> '$out/21_cxa_personality.translation_unit_include_chain.full.txt'" || exit 90
run_recorded PREPROCESS_TU_22 bash -o pipefail -c \
    "'$qemu' -L '$root22' '$root22/usr/bin/clang++' --target=armv7l-tizen-linux-gnueabi --sysroot='$root22' -resource-dir '$root22/usr/lib/clang/22' ${common[*]} -D_LIBCPP_AVAILABILITY_MINIMUM_HEADER_VERSION=2 -I'$src22/libcxx/src' -I'$src22/libcxxabi/include' -I'$src22/build/include/c++/v1' -H -E '$src22/libcxxabi/src/cxa_personality.cpp' > '$out/22_cxa_personality.preprocessed.ii' 2> '$out/22_cxa_personality.translation_unit_include_chain.full.txt'" || exit 91
run_recorded TU_UNWIND_PATHS bash -o pipefail -c \
    "for v in 21 22; do printf 'VERSION=%s\n' \"\$v\"; rg -n 'unwind\\.h|libunwind\\.h' '$out/'\"\$v\"'_cxa_personality.translation_unit_include_chain.full.txt' || true; done > '$out/translation_unit_unwind_header_paths.raw.txt'" || exit 92
run_recorded TU_SYMBOL_21 bash -o pipefail -c \
    "rg -n -C 2 '__gnu_unwind_frame' '$out/21_cxa_personality.preprocessed.ii' > '$out/21_preprocessed_tu_gnu_unwind_frame.raw.txt'" || exit 93
run_recorded TU_SYMBOL_22 bash -o pipefail -c \
    "rg -n -C 2 '__gnu_unwind_frame' '$out/22_cxa_personality.preprocessed.ii' > '$out/22_preprocessed_tu_gnu_unwind_frame.raw.txt'" || exit 94
run_recorded TU_DECLARATION_21 rg -n 'extern "C" _Unwind_Reason_Code __gnu_unwind_frame' "$out/21_cxa_personality.preprocessed.ii" || exit 95
run_recorded TU_DECLARATION_22 rg -n 'extern "C" _Unwind_Reason_Code __gnu_unwind_frame' "$out/22_cxa_personality.preprocessed.ii"
tu22_decl_rc=$?
printf 'EXPECTED_RG_EXIT=1\tACTUAL_RG_EXIT=%s\tINTERPRETATION=%s\n' "$tu22_decl_rc" "$([ "$tu22_decl_rc" -eq 1 ] && printf DECLARATION_NOT_VISIBLE || printf UNEXPECTED)" >"$out/22_preprocessed_tu_declaration_assertion.tsv"
test "$tu22_decl_rc" -eq 1 || exit 96
run_recorded PREPROCESSED_SHA_SIZE bash -o pipefail -c \
    "sha256sum '$out/21_cxa_personality.preprocessed.ii' '$out/22_cxa_personality.preprocessed.ii'; wc -c '$out/21_cxa_personality.preprocessed.ii' '$out/22_cxa_personality.preprocessed.ii'" || exit 97
exit 0
