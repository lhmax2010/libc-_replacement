#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/T3R2"
cmdlog="$out/commands/06_finalize_evidence.log"
x86root="$repo/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260804/local/BUILD-ROOTS/scratch.x86_64.0"
mkdir -p "$out/x86_64" "$repo/progress/state"
: >"$cmdlog"

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
    } >>"$cmdlog"
    "$@" >>"$cmdlog" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n' "$rc" >>"$cmdlog"
    printf '%s\t%s\n' "$label" "$rc"
    return "$rc"
}

cd "$repo" || exit 125
run_recorded INPUT_IDENTITY bash -o pipefail -c \
    "{ printf 'main_head\\t%s\\n' \"\$(git rev-parse HEAD)\"; printf 'build_source_commit\\t89bc0b8ad6f0d41c0fcdf4b9c942b08c3f378e72\\n'; sha256sum packaging/libcxx-runtimes.spec packaging/libcxx-llvm22.map packaging/libcxx-runtimes.manifest packaging/SOURCE_PROVENANCE progress/T3R/config/gbs_llvm.conf /var/tmp/toolchain-gbs/tizen_unified_standard.conf; } > '$out/input_identity.tsv'" || exit 90
run_recorded ASSERT_SPEC_SOURCE_UNCHANGED git diff --exit-code 89bc0b8ad6f0d41c0fcdf4b9c942b08c3f378e72 -- packaging || exit 91
run_recorded X86_CURRENT_SHA bash -o pipefail -c \
    "sha256sum artifacts/rebuild_2218/x86_64/*.rpm | LC_ALL=C sort -k2 > '$out/x86_64/current_rpm_sha256.tsv'" || exit 92
run_recorded X86_UNCHANGED_COMPARE bash -o pipefail -c \
    "awk -F '\\t' 'NR>1 {print \$1 \"\\t\" \$4}' progress/T3R/x86_64/rpm_sha256.tsv | LC_ALL=C sort > '$out/x86_64/expected_rpm_sha256.tsv'; awk '{name=\$2; sub(\".*/\",\"\",name); print name \"\\t\" \$1}' '$out/x86_64/current_rpm_sha256.tsv' | LC_ALL=C sort > '$out/x86_64/actual_rpm_sha256.tsv'; diff -u '$out/x86_64/expected_rpm_sha256.tsv' '$out/x86_64/actual_rpm_sha256.tsv' > '$out/x86_64/unchanged.diff'" || exit 93
run_recorded X86_READONLY_INSTALLED_NEVRA bash -o pipefail -c \
    "rpm --root '$x86root' --dbpath /var/lib/rpm -qa --qf '%{NAME}\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\t%{ARCH}\n' | LC_ALL=C sort > '$out/x86_64/installed_nevra_readonly.tsv'" || exit 94
run_recorded X86_TOOLCHAIN_NEVRA bash -o pipefail -c \
    "grep -E '^(binutils|clang|clang-accel|cmake|cpp|gcc|gcc-c\+\+|libgcc|libllvm|libstdc\+\+|libstdc\+\+-devel|lldb|llvm|llvm-devel|make|ninja|rpm-build)[[:space:]]' '$out/x86_64/installed_nevra_readonly.tsv' > '$out/x86_64/toolchain_nevra_readonly.tsv'" || exit 95
run_recorded ROOT_MARKERS bash -o pipefail -c \
    "for f in tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2/RETIRED_SINGLE_USE tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260804-r2/RETIRED_SINGLE_USE; do printf 'FILE=%s\\n' \"\$f\"; sed -n '1,20p' \"\$f\"; done > '$out/root_retirement.raw.log'" || exit 96
run_recorded FINAL_GIT_STATUS bash -o pipefail -c \
    "git status --short --branch > '$out/git_status_before_curation.raw.log'" || exit 97

printf 'arch\tbuild_exit\tstatus\tred_class\tartifacts\ttoolchain_resolution\tbuildroot\n' >"$out/build_status.tsv"
printf 'x86_64\t0\tSUCCEEDED_EXISTING_UNCHANGED\tNONE\t7_BINARY_1_SRPM_PRESERVED\tclang-22.1.8-18.1.x86_64\ttmp/GBS-ROOT/LIBCXX-2218-x86_64-20260804\n' >>"$out/build_status.tsv"
printf 'armv7l\t1\tRED_STOP\tUNCLASSIFIED\tNO_RPM\tclang-22.1.8-18.1.armv7l\ttmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2\n' >>"$out/build_status.tsv"
printf 'aarch64\t0\tSUCCEEDED\tNONE\t7_BINARY_1_SRPM_ARCHIVED\tclang-22.1.8-18.1.aarch64\ttmp/GBS-ROOT/LIBCXX-2218-aarch64-20260804-r2\n' >>"$out/build_status.tsv"

printf 'arch\tclassification\tevidence\taction\n' >"$out/red_items.tsv"
printf 'armv7l\tUNCLASSIFIED\tcxa_personality.cpp:1135:11: error: use of undeclared identifier __gnu_unwind_frame\tROOT_RETIRED_SINGLE_USE; NO_RETRY; AWAIT_HUMAN_ADJUDICATION\n' >>"$out/red_items.tsv"

printf 'arch\tsha256\tpath\n' >"$out/artifact_sha256.tsv"
while read -r sha path; do printf 'x86_64\t%s\t%s\n' "$sha" "$path"; done < <(sha256sum artifacts/rebuild_2218/x86_64/*.rpm | LC_ALL=C sort -k2) >>"$out/artifact_sha256.tsv"
while read -r sha path; do printf 'aarch64\t%s\t%s\n' "$sha" "$path"; done < <(sha256sum artifacts/rebuild_2218/aarch64/*.rpm | LC_ALL=C sort -k2) >>"$out/artifact_sha256.tsv"

printf 'build_arch\tname\tevr\trpm_arch\n' >"$out/toolchain_nevra.tsv"
awk -v a=x86_64 'BEGIN{FS=OFS="\t"} {print a,$1,$2,$3}' "$out/x86_64/toolchain_nevra_readonly.tsv" >>"$out/toolchain_nevra.tsv"
awk -v a=armv7l 'BEGIN{FS=OFS="\t"} {print a,$1,$2,$3}' "$out/armv7l/toolchain_nevra.tsv" >>"$out/toolchain_nevra.tsv"
awk -v a=aarch64 'BEGIN{FS=OFS="\t"} {print a,$1,$2,$3}' "$out/aarch64/toolchain_nevra.tsv" >>"$out/toolchain_nevra.tsv"

finished=$(date -Iseconds)
printf 'task\tstatus\tfinished_at\tdetail\nT3R2\tRED_STOP\t%s\tarmv7l=UNCLASSIFIED(__gnu_unwind_frame); aarch64=SUCCESS(7 binary + 1 SRPM); cross-arch=NOT_RUN\n' "$finished" >"$repo/progress/state/T3R2.tsv"

exit 0
