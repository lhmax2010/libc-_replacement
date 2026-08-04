#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
scratch="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/local/BUILD-ROOTS/scratch.armv7l.0"
out="$repo/progress/T3R2/armv7l"
log="$repo/progress/T3R2/commands/04b_capture_armv7l_nevra.log"

{
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'COMMAND_BEGIN\n'
    printf '%s\n' "rpm --root $scratch --dbpath /var/lib/rpm -qa --qf '%{NAME}\\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\\t%{ARCH}\\n' | LC_ALL=C sort > $out/installed_nevra.tsv"
    printf 'COMMAND_END\n'
} >"$log"
rpm --root "$scratch" --dbpath /var/lib/rpm -qa \
    --qf '%{NAME}\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\t%{ARCH}\n' \
    2>>"$log" | LC_ALL=C sort >"$out/installed_nevra.tsv"
rc=${PIPESTATUS[0]}
printf 'EXIT_CODE=%s\n' "$rc" >>"$log"
test "$rc" -eq 0 || exit "$rc"

{
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'COMMAND_BEGIN\n'
    printf '%s\n' "grep -E '^(binutils|clang|clang-accel|cmake|cpp|gcc|gcc-c\\+\\+|libgcc|libllvm|libstdc\\+\\+|libstdc\\+\\+-devel|lldb|llvm|llvm-devel|make|ninja|rpm-build)[[:space:]]' $out/installed_nevra.tsv > $out/toolchain_nevra.tsv"
    printf 'COMMAND_END\n'
} >>"$log"
grep -E '^(binutils|clang|clang-accel|cmake|cpp|gcc|gcc-c\+\+|libgcc|libllvm|libstdc\+\+|libstdc\+\+-devel|lldb|llvm|llvm-devel|make|ninja|rpm-build)[[:space:]]' \
    "$out/installed_nevra.tsv" >"$out/toolchain_nevra.tsv"
rc=$?
printf 'EXIT_CODE=%s\n' "$rc" >>"$log"
exit "$rc"
