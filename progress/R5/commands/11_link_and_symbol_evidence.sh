#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5"
root21=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
root22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0"
build21="$root21/home/abuild/rpmbuild/BUILD/libcxx-runtimes-21.1.1/build"
build22="$root22/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
ledger="$out/commands/11_link_and_symbol_evidence.log"
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
run_recorded LINK_STANZA_21 bash -o pipefail -c \
    "rg -n -A 11 '^build lib/libc\\+\\+abi\\.so\\.1\\.0:' '$build21/build.ninja' > '$out/21_libcxxabi_link_stanza.raw.txt'" || exit 90
run_recorded LINK_STANZA_22 bash -o pipefail -c \
    "rg -n -A 11 '^build lib/libc\\+\\+abi\\.so\\.1\\.0:' '$build22/build.ninja' > '$out/22_libcxxabi_link_stanza.raw.txt'" || exit 91
run_recorded LIBGCC_SYMBOL_21 bash -o pipefail -c \
    "readelf -Ws '$root21/usr/lib/libgcc_s.so.1' | rg '__gnu_unwind_frame' > '$out/21_libgcc_s_gnu_unwind_frame.raw.txt'" || exit 92
run_recorded LIBGCC_SYMBOL_22 bash -o pipefail -c \
    "readelf -Ws '$root22/usr/lib/libgcc_s.so.1' | rg '__gnu_unwind_frame' > '$out/22_libgcc_s_gnu_unwind_frame.raw.txt'" || exit 93
run_recorded LIBGCC_SHA bash -o pipefail -c \
    "sha256sum '$root21/usr/lib/libgcc_s.so.1' '$root22/usr/lib/libgcc_s.so.1' > '$out/libgcc_s_sha256.tsv'" || exit 94
run_recorded HEADER_OWNERS_21 bash -o pipefail -c \
    "rpm --root '$root21' --dbpath /var/lib/rpm -qf /usr/lib/clang/21/include/unwind.h /usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h /usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h > '$out/21_unwind_header_owners.raw.txt'" || exit 95
run_recorded HEADER_OWNERS_22 bash -o pipefail -c \
    "rpm --root '$root22' --dbpath /var/lib/rpm -qf /usr/lib/clang/22/include/unwind.h /usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h /usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind-arm-common.h > '$out/22_unwind_header_owners.raw.txt'" || exit 96
run_recorded CACHE_EXTENDED_21 bash -o pipefail -c \
    "rg -n '^(LIBCXXABI_(USE_LLVM_UNWINDER|ENABLE_STATIC_UNWINDER|USE_COMPILER_RT|LIBUNWIND_INCLUDES|HAS_GCC_S_LIB)|LLVM_ENABLE_RUNTIMES):' '$build21/CMakeCache.txt' > '$out/21_CMakeCache_unwinder_extended.raw.txt'" || exit 97
run_recorded CACHE_EXTENDED_22 bash -o pipefail -c \
    "rg -n '^(LIBCXXABI_(USE_LLVM_UNWINDER|ENABLE_STATIC_UNWINDER|USE_COMPILER_RT|LIBUNWIND_INCLUDES|HAS_GCC_S_LIB)|LLVM_ENABLE_RUNTIMES):' '$build22/CMakeCache.txt' > '$out/22_CMakeCache_unwinder_extended.raw.txt'" || exit 98
exit 0
