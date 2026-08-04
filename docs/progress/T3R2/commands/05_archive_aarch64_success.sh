#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
arch=aarch64
root="$repo/tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260804-r2"
scratch="$root/local/BUILD-ROOTS/scratch.aarch64.0"
repo_out="$root/local/repos/tizen_unified_standard/aarch64"
artifact="$repo/artifacts/rebuild_2218/aarch64"
out="$repo/progress/T3R2/aarch64"
commands="$repo/progress/T3R2/commands/05_archive_aarch64_success.log"
log="$repo/progress/T3R2/build_aarch64.full.log"
mkdir -p "$out"
: >"$commands"

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

cd "$repo" || exit 125
run_recorded TARGET_ASSERTION bash -o pipefail -c \
    "grep -nE 'start building packages from:|Checking dependencies for libcxx-runtimes|there is no circle in 1 packages|\*\*\* \[1/1\] building libcxx-runtimes|circle found in|/usr/bin/build' '$log' > '$out/target_assertion.raw.log'" || exit 90
run_recorded BUILD_SUCCESS bash -o pipefail -c \
    "grep -nE 'Checking for unpackaged file\(s\)|Wrote:|finished building libcxx-runtimes|Total succeeded built packages: \(1\)|EXIT_CODE=0' '$log' > '$out/build_success.raw.log'" || exit 91
run_recorded ASSERT_CHECK_FILES bash -o pipefail -c \
    "grep -q 'Checking for unpackaged file(s)' '$log'" || exit 92
run_recorded REPOSITORY_SET bash -o pipefail -c \
    "grep -nE 'depanneur .*--repository=|init_buildsystem .*--repository' '$log' > '$out/repository_set.raw.log'" || exit 93
run_recorded FULL_INSTALLED_NEVRA bash -o pipefail -c \
    "rpm --root '$scratch' --dbpath /var/lib/rpm -qa --qf '%{NAME}\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\t%{ARCH}\n' | LC_ALL=C sort > '$out/installed_nevra.tsv'" || exit 94
run_recorded TOOLCHAIN_NEVRA bash -o pipefail -c \
    "grep -E '^(binutils|clang|clang-accel|cmake|cpp|gcc|gcc-c\+\+|libgcc|libllvm|libstdc\+\+|libstdc\+\+-devel|lldb|llvm|llvm-devel|make|ninja|rpm-build)[[:space:]]' '$out/installed_nevra.tsv' > '$out/toolchain_nevra.tsv'" || exit 95

mapfile -t rpms < <(find "$repo_out/RPMS" "$repo_out/SRPMS" -maxdepth 1 -type f -name '*.rpm' -print | LC_ALL=C sort)
printf 'RPM_COUNT=%s\n' "${#rpms[@]}" | tee -a "$commands"
test "${#rpms[@]}" -eq 8 || exit 96
test "$(find "$repo_out/RPMS" -maxdepth 1 -type f -name '*.rpm' | wc -l)" -eq 7 || exit 97
test "$(find "$repo_out/SRPMS" -maxdepth 1 -type f -name '*.rpm' | wc -l)" -eq 1 || exit 98
test -z "$(find "$artifact" -maxdepth 1 -type f -name '*.rpm' -print -quit)" || exit 99

for rpm_path in "${rpms[@]}"; do
    run_recorded "ARCHIVE_RPM_$(basename "$rpm_path")" cp --no-clobber "$rpm_path" "$artifact/" || exit 100
done

run_recorded ASSERT_NEW_ARCHIVE_NAMES test ! -e "$artifact/gbs_llvm.r2.conf" || exit 101
run_recorded ASSERT_NEW_BUILDCONF_NAME test ! -e "$artifact/tizen_unified_standard.r2.conf" || exit 102
run_recorded ARCHIVE_GBS_CONF cp --no-clobber "$repo/progress/T3R/config/gbs_llvm.conf" "$artifact/gbs_llvm.r2.conf" || exit 103
run_recorded ARCHIVE_BUILDCONF cp --no-clobber /var/tmp/toolchain-gbs/tizen_unified_standard.conf "$artifact/tizen_unified_standard.r2.conf" || exit 104
run_recorded ARTIFACT_SHA256 bash -o pipefail -c \
    "sha256sum '$artifact'/*.rpm '$artifact/gbs_llvm.r2.conf' '$artifact/tizen_unified_standard.r2.conf' | LC_ALL=C sort -k2 > '$out/artifact_sha256.tsv'" || exit 105
run_recorded RPM_IDENTITIES bash -o pipefail -c \
    "for f in '$artifact'/*.rpm; do rpm -qp --qf '%{NAME}\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\t%{ARCH}\t%{SOURCERPM}\t%{SIZE}\t%{FILENAMES}\n' \"\$f\"; done | LC_ALL=C sort > '$out/rpm_identities.tsv'" || exit 106
run_recorded RETIRE_ROOT bash -o pipefail -c \
    "printf 'RETIRED_SINGLE_USE\narch=aarch64\nclassification=SUCCESS\nroot=%s\n' '$root' > '$root/RETIRED_SINGLE_USE'" || exit 107

printf 'ARCH=aarch64\nSTATUS=SUCCESS\nRPM_COUNT=8\nROOT=%s\n' "$root"
exit 0
