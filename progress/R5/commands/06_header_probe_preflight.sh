#!/usr/bin/env bash
set -u

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5/raw"
root21=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
root22="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0"

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
run_recorded 30_compiler_paths bash -o pipefail -c \
    "for p in '$root21/bin/armv7l-tizen-linux-gnueabi-clang++' '$root22/bin/armv7l-tizen-linux-gnueabi-clang++' '$root21/usr/bin/clang++' '$root22/usr/bin/clang++'; do ls -l \"\$p\"; readlink -f \"\$p\"; file -L \"\$p\"; done"
run_recorded 31_rpm_unwind_files_21 bash -o pipefail -c \
    "for pkg in clang gcc libgcc llvm-devel; do printf 'PACKAGE=%s\\n' \"\$pkg\"; rpm --root '$root21' --dbpath /var/lib/rpm -ql \"\$pkg\" 2>&1 | rg '/(unwind|unwind-arm|libunwind)[^/]*\\.h$|/unwind\\.h$' || true; done"
run_recorded 32_rpm_unwind_files_22 bash -o pipefail -c \
    "for pkg in clang gcc libgcc llvm-devel; do printf 'PACKAGE=%s\\n' \"\$pkg\"; rpm --root '$root22' --dbpath /var/lib/rpm -ql \"\$pkg\" 2>&1 | rg '/(unwind|unwind-arm|libunwind)[^/]*\\.h$|/unwind\\.h$' || true; done"
run_recorded 33_known_headers bash -o pipefail -c \
    "for root in '$root21' '$root22'; do for rel in /usr/lib/clang/21/include/unwind.h /usr/lib/clang/22/include/unwind.h /usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h /usr/include/unwind.h; do if test -f \"\$root\$rel\"; then printf 'FOUND\\t%s\\n' \"\$root\$rel\"; fi; done; done"
run_recorded 34_compiler_versions bash -o pipefail -c \
    "'$root21/bin/armv7l-tizen-linux-gnueabi-clang++' --version; '$root22/bin/armv7l-tizen-linux-gnueabi-clang++' --version"
run_recorded 35_header_symbol_scan bash -o pipefail -c \
    "for f in '$root21/usr/lib/clang/21/include/unwind.h' '$root22/usr/lib/clang/22/include/unwind.h' '$root21/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h' '$root22/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/unwind.h'; do if test -f \"\$f\"; then printf 'FILE=%s\\n' \"\$f\"; rg -n -C 8 '__gnu_unwind_frame|ARM_EABI_UNWINDER|include_next|_LIBUNWIND_H' \"\$f\" || true; fi; done"
exit 0
