#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
dst="$repo/docs/progress/T3R2"
log="$dst/commands/08_validate_delivery.log"
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
run_recorded COPY_VALIDATOR cp --no-clobber progress/T3R2/commands/08_validate_delivery.sh docs/progress/T3R2/commands/08_validate_delivery.sh || exit 90
run_recorded REQUIRED_FILES test -s docs/progress/T3R2/T3R2_report.md -a -s docs/progress/T3R2/invocation_delta.tsv -a -s docs/progress/T3R2/build_status.tsv -a -s docs/progress/T3R2/red_items.tsv -a -s docs/progress/T3R2/artifact_sha256.tsv -a -s docs/progress/T3R2/toolchain_nevra.tsv || exit 91
run_recorded AARCH_LOCAL_RPM_COUNT bash -o pipefail -c "test \"\$(find artifacts/rebuild_2218/aarch64 -maxdepth 1 -type f -name '*.rpm' | wc -l)\" -eq 8" || exit 92
run_recorded ARM_LOCAL_RPM_COUNT bash -o pipefail -c "test \"\$(find artifacts/rebuild_2218/armv7l -maxdepth 1 -type f -name '*.rpm' | wc -l)\" -eq 0" || exit 93
run_recorded CURATED_RPM_COUNT bash -o pipefail -c "test \"\$(find docs/progress/T3R2 -type f -name '*.rpm' | wc -l)\" -eq 0" || exit 94
run_recorded ASSERT_RED_CLASS grep -F $'armv7l\tUNCLASSIFIED\tcxa_personality.cpp:1135:11: error: use of undeclared identifier __gnu_unwind_frame' docs/progress/T3R2/red_items.tsv || exit 95
run_recorded ASSERT_AARCH_SUCCESS grep -F $'aarch64\t0\tSUCCEEDED\tNONE\t7_BINARY_1_SRPM_ARCHIVED' docs/progress/T3R2/build_status.tsv || exit 96
run_recorded ASSERT_CROSS_NOT_RUN bash -o pipefail -c "test \"\$(grep -c 'NOT_RUN_TWO_ARCH_SUCCESS_REQUIRED' docs/progress/T3R2/cross_arch_comparison_status.tsv)\" -eq 4" || exit 97
run_recorded ASSERT_SPEC_UNCHANGED git diff --exit-code 89bc0b8ad6f0d41c0fcdf4b9c942b08c3f378e72 -- packaging || exit 98
run_recorded GIT_DIFF_CHECK git diff --check || exit 99
run_recorded CURATED_SIZE du -sh docs/progress/T3R2 || exit 100
run_recorded GIT_STATUS git status --short --branch || exit 101

cd "$dst" || exit 125
find . -type f ! -path ./MANIFEST.sha256 ! -path ./commands/07_curate.log ! -path ./commands/08_validate_delivery.log -print0 \
    | LC_ALL=C sort -z \
    | xargs -0 sha256sum >MANIFEST.sha256
manifest_rc=${PIPESTATUS[2]}
printf 'MANIFEST_GENERATE_EXIT_CODE=%s\n' "$manifest_rc" >>"$log"
test "$manifest_rc" -eq 0 || exit "$manifest_rc"
sha256sum -c MANIFEST.sha256 >>"$log" 2>&1
verify_rc=$?
printf 'MANIFEST_VERIFY_EXIT_CODE=%s\n' "$verify_rc" >>"$log"
exit "$verify_rc"
