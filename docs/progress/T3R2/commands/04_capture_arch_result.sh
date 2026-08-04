#!/usr/bin/env bash
set -u

arch=${1:-}
case "$arch" in
    armv7l|aarch64) ;;
    *) printf 'USAGE: %s armv7l|aarch64\n' "$0" >&2; exit 64 ;;
esac

repo=/home/toolchain/development/libc++_replacement
root="$repo/tmp/GBS-ROOT/LIBCXX-2218-${arch}-20260804-r2"
scratch="$root/local/BUILD-ROOTS/scratch.${arch}.0"
log="$repo/progress/T3R2/build_${arch}.full.log"
out="$repo/progress/T3R2/$arch"
artifact="$repo/artifacts/rebuild_2218/$arch"
commands="$repo/progress/T3R2/commands/04_capture_${arch}.log"
mkdir -p "$out"

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
    } >>"$commands"
    "$@" >>"$commands" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n' "$rc" >>"$commands"
    printf '%s\t%s\n' "$label" "$rc"
    return "$rc"
}

: >"$commands"
cd "$repo" || exit 125

run_recorded TARGET_ASSERTION bash -o pipefail -c \
    "grep -nE 'start building packages from:|Checking dependencies for libcxx-runtimes|there is no circle in 1 packages|\*\*\* \[1/1\] building libcxx-runtimes|circle found in|/usr/bin/build' '$log' > '$out/target_assertion.raw.log'"
run_recorded REPOSITORY_SET bash -o pipefail -c \
    "grep -nE 'depanneur .*--repository=|init_buildsystem .*--repository' '$log' > '$out/repository_set.raw.log'"
run_recorded ERROR_EXCERPT bash -o pipefail -c \
    "grep -nEi 'Installed \(but unpackaged\)|File not found|RPM build errors|error:|unpackaged|missing file|undefined reference|No such file|nothing provides|unresolvable|failed' '$log' > '$out/error_index.raw.log'; sed -n '2360,2450p' '$log' > '$out/error_context.raw.log'"
run_recorded FULL_INSTALLED_NEVRA bash -o pipefail -c \
    "rpm --root '$scratch' --dbpath /var/lib/rpm -qa --qf '%{NAME}\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\t%{ARCH}\n' | LC_ALL=C sort > '$out/installed_nevra.tsv'"
run_recorded TOOLCHAIN_NEVRA bash -o pipefail -c \
    "grep -E '^(binutils|clang|clang-accel|cmake|cpp|gcc|gcc-c\+\+|libgcc|libllvm|libstdc\+\+|libstdc\+\+-devel|lldb|llvm|llvm-devel|make|ninja|rpm-build)[[:space:]]' '$out/installed_nevra.tsv' > '$out/toolchain_nevra.tsv'"
run_recorded COPY_RPMBUILD_LOG bash -o pipefail -c \
    "cp --no-clobber '$root/local/repos/tizen_unified_standard/$arch/logs/fail/libcxx-runtimes-22.1.8-1/log.txt' '$out/rpmbuild_fail.full.log'"

classification=UNCLASSIFIED
if grep -Eq 'Installed \(but unpackaged\) file\(s\) found|File not found:' "$log"; then
    classification=EXPECTED_GAP_C10
    run_recorded C10_LISTS bash -o pipefail -c \
        "grep -nE 'Installed \(but unpackaged\) file\(s\) found|File not found:|RPM build errors:' '$log' > '$out/c10_unpacked_missing.raw.log'; find '$scratch/home/abuild/rpmbuild/BUILDROOT' \( -type f -o -type l \) -printf '%p\t%s\n' | LC_ALL=C sort > '$out/c10_install_tree_paths_sizes.tsv'"
elif grep -Eqi 'nothing provides|unresolvable|expansion error|have choice for' "$log"; then
    classification=BUILDREQ_VERSION_FORM
    run_recorded SOLVER_ERROR bash -o pipefail -c \
        "grep -nEi 'nothing provides|unresolvable|expansion error|have choice for|clang-[0-9].*${arch}' '$log' > '$out/solver.raw.log'"
fi
printf 'arch\tclassification\tevidence\taction\n%s\t%s\t%s\t%s\n' \
    "$arch" "$classification" "error_index.raw.log; error_context.raw.log" "STOP_ARCH_ROOT_RETIRED_NO_RETRY" \
    >"$out/red_item.tsv"

run_recorded ASSERT_NEW_ARCHIVE_NAMES test ! -e "$artifact/gbs_llvm.r2.conf"
run_recorded ASSERT_NEW_BUILDCONF_NAME test ! -e "$artifact/tizen_unified_standard.r2.conf"
run_recorded ARCHIVE_GBS_CONF cp --no-clobber "$repo/progress/T3R/config/gbs_llvm.conf" "$artifact/gbs_llvm.r2.conf"
run_recorded ARCHIVE_BUILDCONF cp --no-clobber /var/tmp/toolchain-gbs/tizen_unified_standard.conf "$artifact/tizen_unified_standard.r2.conf"
run_recorded ARCHIVE_CONFIG_HASHES bash -o pipefail -c \
    "sha256sum '$artifact/gbs_llvm.r2.conf' '$artifact/tizen_unified_standard.r2.conf' > '$out/archived_config_sha256.tsv'"
run_recorded RETIRE_ROOT bash -o pipefail -c \
    "printf 'RETIRED_SINGLE_USE\narch=%s\nclassification=%s\nroot=%s\n' '$arch' '$classification' '$root' > '$root/RETIRED_SINGLE_USE'"

printf 'ARCH=%s\nCLASSIFICATION=%s\nROOT=%s\n' "$arch" "$classification" "$root"
exit 0
