#!/usr/bin/env bash
set -u
set -o pipefail

ARCH=${1:-}
case "$ARCH" in
    armv7l|aarch64|x86_64) ;;
    *) printf 'usage: %s armv7l|aarch64|x86_64\n' "$0" >&2; exit 64 ;;
esac

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/raw"
ARCH_OUT="$OUT/architectures/$ARCH"
BUILD="$OUT/build"
LEDGER="$OUT/commands/03_build_${ARCH}.log"
LOG="$BUILD/${ARCH}.full.log"
STATUS="$ARCH_OUT/status.tsv"
WORKTREE="$REPO/tmp/R9C/seg2/gbs-source-c2"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-${ARCH}-20260806-c2"
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
BUILDCONF=/var/tmp/toolchain-gbs/tizen_unified_standard.conf
ARTIFACT="$REPO/artifacts/rebuild_2218_c2/$ARCH"
EXPECTED_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
EXPECTED_SOURCE0=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2

mkdir -p "$ARCH_OUT"
bootstrap_rc=$?
{
    printf 'LABEL=create_architecture_evidence_directory\nPWD=%s\nCOMMAND=' "$PWD"
    printf '%q ' mkdir -p "$ARCH_OUT"
    printf '\nEXIT_CODE=%s\n\n' "$bootstrap_rc"
} >"$LEDGER"
[[ $bootstrap_rc -eq 0 ]] || exit "$bootstrap_rc"

record_to() {
    local label=$1 outfile=$2
    shift 2
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >"$outfile" 2>"$RAW/${ARCH}_${label}.stderr.txt"
    local rc=$?
    printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
    return "$rc"
}

cd "$WORKTREE" || exit 125
preflight_fail=0
record_to assert_common_export_preflight "$RAW/${ARCH}_assert_common_export_preflight.stdout.txt" bash -o pipefail -c \
    'rg -qx "STANDARD_EXPORT_PREFLIGHT=PASS_SINGLE_PACKAGE_SOURCE0_R9C_6_PATCHES" "$1" && rg -qx "STATUS=PASS_SINGLE_PACKAGE_PREFLIGHT" "$2"' _ \
    "$OUT/preflight.status" "$OUT/identity/prebuild_single_target_assertion.txt" || preflight_fail=1
record_to assert_fresh_arch_root_and_artifact "$RAW/${ARCH}_assert_fresh_root_and_artifact.stdout.txt" bash -o pipefail -c \
    'test ! -e "$1" && test ! -e "$2"' _ "$ROOT" "$ARTIFACT" || preflight_fail=1
record_to assert_single_package_context "$RAW/${ARCH}_assert_single_package_context.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1/packaging" -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1 && rg -q "^Name:[[:space:]]+libcxx-runtimes$" "$1/packaging/libcxx-runtimes.spec" && test "$(git -C "$1" diff --name-only | sed "/^packaging\\//d;/^$/d" | wc -l)" -eq 0' _ "$WORKTREE" || preflight_fail=1
record_to assert_frozen_source_and_configs "$RAW/${ARCH}_assert_frozen_source_and_configs.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)" && test "$(sha256sum "$3" | cut -d" " -f1)" = 8fab16c1ca9db28d45f6b269881af47a71ef3d111d93527b602f57e1e37d49cf && test "$(sha256sum "$4" | cut -d" " -f1)" = 9eac0e23968b9bd7dabd463d400f0281d2464815fc2299431052f7b8cfb736a5 && cmp -s "$5" "$6"' _ \
    "$LLVM" "$EXPECTED_HEAD" "$CONFIG" "$BUILDCONF" "$WORKTREE/packaging/SOURCE_PROVENANCE" "$REPO/packaging/SOURCE_PROVENANCE" || preflight_fail=1
if [[ $preflight_fail -ne 0 ]]; then
    printf 'architecture\tstatus\tclassification\troot\n%s\tRED_STOP_PREBUILD\tPREBUILD_ASSERTION_FAILED\t%s\n' "$ARCH" "$ROOT" >"$STATUS"
    printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
    exit 90
fi

CMD=(gbs -c "$CONFIG" build
    -A "$ARCH"
    -B "$ROOT"
    -C
    --debug
    --style git
    --include-all
    --packaging-dir packaging
    --spec libcxx-runtimes.spec
    "$WORKTREE")
{
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'SOURCE_MODE=DETACHED_GIT_WORKTREE_STANDARD_PATCH_EXPORT\n'
    printf 'SOURCE_COMMIT=%s\n' "$(git rev-parse HEAD)"
    printf 'PACKAGING_DIRECTORY=packaging\n'
    printf 'BUILDROOT=%s\n' "$ROOT"
    printf 'COMMAND_BEGIN\n'
    printf '%q ' "${CMD[@]}"
    printf '\nCOMMAND_END\nOUTPUT_BEGIN\n'
} >"$LOG"
"${CMD[@]}" 2>&1 | tee -a "$LOG"
gbs_rc=${PIPESTATUS[0]}
printf 'OUTPUT_END\nEXIT_CODE=%s\n' "$gbs_rc" | tee -a "$LOG"
{
    printf 'LABEL=gbs_standard_export_build_%s\nPWD=%s\nCOMMAND=' "$ARCH" "$PWD"
    printf '%q ' "${CMD[@]}"
    printf '\nEXIT_CODE=%s\n\n' "$gbs_rc"
} >>"$LEDGER"

classification=SUCCESS
if rg -q 'circle found in [0-9]+/[0-9]+ packages' "$LOG"; then
    classification=FULL_REPOSITORY_SCAN_OR_CIRCLE
elif [[ $gbs_rc -ne 0 ]] && rg -qi 'Installed \(but unpackaged\)|File not found:|unpackaged file|missing file' "$LOG"; then
    classification=EXPECTED_GAP_C10
elif [[ $gbs_rc -ne 0 ]] && rg -qi 'nothing provides|nothing provides requested|unresolvable|BuildRequires' "$LOG"; then
    classification=BUILDREQ_VERSION_FORM
elif [[ $gbs_rc -ne 0 ]]; then
    classification=UNCLASSIFIED
fi

record_to target_resolution_evidence "$ARCH_OUT/target_resolution.log" rg -n \
    'start building packages from:|Checking dependencies for libcxx-runtimes|there is no circle in 1 packages|\*\*\* \[1/1\] building libcxx-runtimes|circle found in|/usr/bin/build' "$LOG" || true
record_to repository_set "$ARCH_OUT/repository_set.log" rg -n \
    'depanneur .*--repository=|init_buildsystem .*--repository' "$LOG" || true

if [[ $gbs_rc -ne 0 || $classification != SUCCESS ]]; then
    record_to failure_context "$ARCH_OUT/failure_context.log" rg -n -C 12 \
        'error:|ERROR|Error|FAILED:|ninja: build stopped|Installed \(but unpackaged\)|File not found:|unpackaged file|missing file|nothing provides|unresolvable|circle found in' "$LOG" || true
    if [[ $classification == EXPECTED_GAP_C10 ]]; then
        record_to c10_full_file_errors "$ARCH_OUT/c10_file_errors.log" rg -n \
            'Installed \(but unpackaged\)|File not found:|unpackaged file|missing file|RPM build errors:|^error:' "$LOG" || true
        scratch="$ROOT/local/BUILD-ROOTS/scratch.${ARCH}.0"
        if [[ -d "$scratch/home/abuild/rpmbuild/BUILDROOT" ]]; then
            record_to c10_actual_install_tree "$ARCH_OUT/c10_actual_install_tree.tsv" find \
                "$scratch/home/abuild/rpmbuild/BUILDROOT" -printf '%y\t%p\t%s\n' || true
        fi
    elif [[ $classification == BUILDREQ_VERSION_FORM ]]; then
        record_to buildreq_solver_evidence "$ARCH_OUT/buildreq_solver_evidence.log" rg -n -C 10 \
            'nothing provides|unresolvable|BuildRequires|clang-22\.1\.8|clang = 22\.1\.8' "$LOG" || true
    fi
    if [[ -d "$ROOT" ]]; then
        record_to retire_failed_root "$ARCH_OUT/root_retirement.txt" bash -o pipefail -c \
            'printf "RETIRED_SINGLE_USE\\narch=%s\\nclassification=%s\\ngbs_exit_code=%s\\nroot=%s\\n" "$2" "$3" "$4" "$1" > "$1/RETIRED_SINGLE_USE"; cat "$1/RETIRED_SINGLE_USE"' _ \
            "$ROOT" "$ARCH" "$classification" "$gbs_rc" || true
    fi
    printf 'architecture\tstatus\tclassification\tgbs_exit\troot\n%s\tRED\t%s\t%s\t%s\n' "$ARCH" "$classification" "$gbs_rc" "$ROOT" >"$STATUS"
    printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
    exit 1
fi

post_fail=0
SCRATCH="$ROOT/local/BUILD-ROOTS/scratch.${ARCH}.0"
LOCAL_REPO="$ROOT/local/repos/tizen_unified_standard/$ARCH"
SOURCE_EXPORT="$ROOT/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1"
record_to assert_single_target_success "$RAW/${ARCH}_assert_single_target_success.stdout.txt" bash -o pipefail -c \
    'rg -q "there is no circle in 1 packages" "$1" && rg -q "\\*\\*\\* \\[1/1\\] building libcxx-runtimes" "$1" && ! rg -q "circle found in [0-9]+/[0-9]+ packages" "$1"' _ "$LOG" || post_fail=1
record_to assert_build_success "$RAW/${ARCH}_assert_build_success.stdout.txt" bash -o pipefail -c \
    'rg -q "Checking for unpackaged file\\(s\\)" "$1" && rg -q "finished building libcxx-runtimes" "$1" && rg -q "Total succeeded built packages: \\(1\\)" "$1" && rg -q "^EXIT_CODE=0$" "$1"' _ "$LOG" || post_fail=1
record_to exported_source0_sha256 "$ARCH_OUT/source0_sha256.txt" sha256sum "$SOURCE_EXPORT/llvm-22.1.8.tar.gz" || post_fail=1
record_to assert_source0_anchor "$RAW/${ARCH}_assert_source0_anchor.stdout.txt" bash -o pipefail -c \
    'test "$(sha256sum "$1" | cut -d" " -f1)" = "$2"' _ "$SOURCE_EXPORT/llvm-22.1.8.tar.gz" "$EXPECTED_SOURCE0" || post_fail=1
record_to exported_patch_inventory "$ARCH_OUT/export_patch_inventory.tsv" bash -o pipefail -c \
    'printf "patch_file\\tsha256\\tpatch_id\\n"; for p in "$1"/0*.patch; do printf "%s\\t%s\\t%s\\n" "$(basename "$p")" "$(sha256sum "$p" | cut -d" " -f1)" "$(git patch-id --stable < "$p" | cut -d" " -f1)"; done' _ "$SOURCE_EXPORT" || post_fail=1
record_to assert_patch_identity "$RAW/${ARCH}_assert_patch_identity.stdout.txt" bash -o pipefail -c \
    'cmp -s <(cut -f1-3 "$1") <(cut -f1-3 "$2") && rg -q "^Patch5:[[:space:]]+0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch$" "$3" && rg -q "^%patch5 -p1$" "$3"' _ \
    "$ARCH_OUT/export_patch_inventory.tsv" "$OUT/identity/provenance_patch_inventory.tsv" "$SOURCE_EXPORT/libcxx-runtimes.spec" || post_fail=1
record_to assert_source_provenance_identity "$RAW/${ARCH}_assert_source_provenance_identity.stdout.txt" cmp -s \
    "$SOURCE_EXPORT/SOURCE_PROVENANCE" "$REPO/packaging/SOURCE_PROVENANCE" || post_fail=1
if [[ $ARCH == armv7l ]]; then
    record_to cxa_personality_object_success "$ARCH_OUT/cxa_personality_object_success.log" rg -n -C 3 \
        'Building CXX object libcxxabi/src/CMakeFiles/cxxabi_shared_objects.dir/cxa_personality.cpp.o|cxa_personality.cpp.o' "$LOG" || post_fail=1
    record_to assert_cxa_error_absent "$RAW/${ARCH}_assert_cxa_error_absent.stdout.txt" bash -o pipefail -c \
        '! rg -q "use of undeclared identifier .__gnu_unwind_frame.|ninja: build stopped" "$1"' _ "$LOG" || post_fail=1
fi
record_to assert_rpm_counts "$RAW/${ARCH}_assert_rpm_counts.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1/RPMS" -maxdepth 1 -type f -name "*.rpm" | wc -l)" -eq 7 && test "$(find "$1/SRPMS" -maxdepth 1 -type f -name "*.rpm" | wc -l)" -eq 1' _ "$LOCAL_REPO" || post_fail=1

if [[ $post_fail -eq 0 ]]; then
    record_to create_artifact_directory "$RAW/${ARCH}_create_artifact_directory.stdout.txt" mkdir -p "$ARTIFACT" || post_fail=1
    record_to archive_binary_rpms "$RAW/${ARCH}_archive_binary_rpms.stdout.txt" bash -o pipefail -c \
        'find "$1/RPMS" -maxdepth 1 -type f -name "*.rpm" -exec cp --no-clobber -a -t "$2" -- {} +' _ "$LOCAL_REPO" "$ARTIFACT" || post_fail=1
    record_to archive_source_rpm "$RAW/${ARCH}_archive_source_rpm.stdout.txt" bash -o pipefail -c \
        'find "$1/SRPMS" -maxdepth 1 -type f -name "*.rpm" -exec cp --no-clobber -a -t "$2" -- {} +' _ "$LOCAL_REPO" "$ARTIFACT" || post_fail=1
    record_to archive_gbs_conf "$RAW/${ARCH}_archive_gbs_conf.stdout.txt" cp --no-clobber -a "$CONFIG" "$ARTIFACT/gbs_llvm.conf" || post_fail=1
    record_to archive_buildconf "$RAW/${ARCH}_archive_buildconf.stdout.txt" cp --no-clobber -a "$BUILDCONF" "$ARTIFACT/tizen_unified_standard.conf" || post_fail=1
fi
record_to installed_all_nevra "$ARCH_OUT/installed_all_nevra.tsv" rpm --root "$SCRATCH" --dbpath /var/lib/rpm -qa \
    --qf '%{NAME}\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\t%{ARCH}\n' || post_fail=1
record_to toolchain_nevra "$ARCH_OUT/toolchain_nevra.tsv" bash -o pipefail -c \
    'LC_ALL=C sort "$1" | rg "^(binutils|clang|clang-accel|cmake|cpp|gcc|gcc-c\\+\\+|libgcc|libllvm|libstdc\\+\\+|libstdc\\+\\+-devel|lldb|llvm|llvm-devel|make|ninja|rpm-build)[[:space:]]"' _ "$ARCH_OUT/installed_all_nevra.tsv" || post_fail=1
record_to artifact_sha256 "$ARCH_OUT/artifact_sha256.txt" bash -o pipefail -c \
    'find "$1" -maxdepth 1 -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$ARTIFACT" || post_fail=1
record_to rpm_identities "$ARCH_OUT/rpm_identities.tsv" bash -o pipefail -c \
    'printf "file\\tname\\tnevra\\tsourcerpm\\tpayload_size\\n"; for f in "$1"/*.rpm; do rpm -qp --qf "%{NAME}\\t%{NEVRA}\\t%{SOURCERPM}\\t%{SIZE}\\n" "$f" | sed "s#^#$(basename "$f")\\t#"; done | LC_ALL=C sort' _ "$ARTIFACT" || post_fail=1
record_to retire_success_root "$ARCH_OUT/root_retirement.txt" bash -o pipefail -c \
    'printf "RETIRED_SINGLE_USE\\narch=%s\\nclassification=SUCCESS\\ngbs_exit_code=0\\nroot=%s\\n" "$2" "$1" > "$1/RETIRED_SINGLE_USE"; cat "$1/RETIRED_SINGLE_USE"' _ "$ROOT" "$ARCH" || post_fail=1
record_to assert_authoritative_source_postbuild "$RAW/${ARCH}_assert_authoritative_source_postbuild.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" "$EXPECTED_HEAD" || post_fail=1

if [[ $post_fail -eq 0 ]]; then
    printf 'architecture\tstatus\tclassification\tgbs_exit\trpm_count\troot\n%s\tPASS\tSUCCESS\t0\t8\t%s\n' "$ARCH" "$ROOT" >"$STATUS"
    printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
    exit 0
fi
printf 'architecture\tstatus\tclassification\tgbs_exit\trpm_count\troot\n%s\tRED\tPOST_BUILD_IDENTITY_FAILURE\t0\tUNKNOWN\t%s\n' "$ARCH" "$ROOT" >"$STATUS"
printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
exit 1
