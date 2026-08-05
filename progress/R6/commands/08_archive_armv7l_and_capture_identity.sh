#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b"
SCRATCH="$ROOT/local/BUILD-ROOTS/scratch.armv7l.0"
LOCAL_REPO="$ROOT/local/repos/tizen_unified_standard/armv7l"
SOURCE_EXPORT="$ROOT/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1"
ARTIFACT="$REPO/artifacts/rebuild_2218/armv7l"
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
BUILDCONF=/var/tmp/toolchain-gbs/tizen_unified_standard.conf
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
ARCH_OUT="$OUT/armv7l"
LEDGER="$OUT/commands/08_archive_armv7l_and_capture_identity.log"
mkdir -p "$ARCH_OUT"
: >"$LEDGER"

run_record() {
  local label=$1
  shift
  {
    printf 'LABEL=%s\n' "$label"
    printf 'PWD=%s\n' "$PWD"
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$RAW/$label.stdout.txt" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

overall=0
cd "$REPO" || exit 125
run_record assert_build_success rg -n -- '^EXIT_CODE=0$|Total succeeded built packages: \(1\)' "$OUT/build_armv7l_git_r3b.full.log" || overall=1
run_record assert_single_target rg -n -- 'there is no circle in 1 packages|\[1/1\] building libcxx-runtimes' "$OUT/build_armv7l_git_r3b.full.log" || overall=1
run_record cxa_object_evidence rg -n -C 2 -- 'cxa_personality\.cpp\.o|cxa_personality\.cpp' "$OUT/build_armv7l_git_r3b.full.log" || overall=1
run_record assert_cxa_old_error_absent bash -o pipefail -c '! rg -n -- "use of undeclared identifier .__gnu_unwind_frame.|ninja: build stopped" "$1"' _ "$OUT/build_armv7l_git_r3b.full.log" || overall=1
run_record init_buildsystem_repositories rg -n -- 'init_buildsystem --configdir.*--repository' "$OUT/build_armv7l_git_r3b.full.log" || overall=1
run_record exported_source_sha256 sha256sum "$SOURCE_EXPORT/llvm-22.1.8.tar.gz" || overall=1
run_record exported_source_patch_occurrence bash -o pipefail -c 'tar -xOf "$1" llvm-22.1.8/libcxxabi/src/cxa_personality.cpp | rg -n -C 2 -- "extern .C. _Unwind_Reason_Code __gnu_unwind_frame|__gnu_unwind_frame\\(exception_object"' _ "$SOURCE_EXPORT/llvm-22.1.8.tar.gz" || overall=1
run_record source_rpms find "$LOCAL_REPO/SRPMS" -maxdepth 1 -type f -name '*.rpm' -print || overall=1
run_record binary_rpms find "$LOCAL_REPO/RPMS" -maxdepth 1 -type f -name '*.rpm' -print || overall=1
run_record assert_rpm_counts bash -o pipefail -c 'test "$(find "$1/RPMS" -maxdepth 1 -type f -name "*.rpm" | wc -l)" -eq 7 && test "$(find "$1/SRPMS" -maxdepth 1 -type f -name "*.rpm" | wc -l)" -eq 1' _ "$LOCAL_REPO" || overall=1
run_record archive_binary_rpms bash -o pipefail -c 'find "$1" -maxdepth 1 -type f -name "*.rpm" -exec cp -a -t "$2" -- {} +' _ "$LOCAL_REPO/RPMS" "$ARTIFACT" || overall=1
run_record archive_source_rpm bash -o pipefail -c 'find "$1" -maxdepth 1 -type f -name "*.rpm" -exec cp -a -t "$2" -- {} +' _ "$LOCAL_REPO/SRPMS" "$ARTIFACT" || overall=1
run_record archive_gbs_conf cp -a "$CONFIG" "$ARTIFACT/gbs_llvm.r3.conf" || overall=1
run_record archive_buildconf cp -a "$BUILDCONF" "$ARTIFACT/tizen_unified_standard.r3.conf" || overall=1
run_record archived_rpm_sha256 bash -o pipefail -c 'find "$1" -maxdepth 1 -type f -name "*.rpm" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$ARTIFACT" || overall=1
run_record archived_config_sha256 sha256sum "$ARTIFACT/gbs_llvm.r3.conf" "$ARTIFACT/tizen_unified_standard.r3.conf" || overall=1
run_record assert_archived_counts bash -o pipefail -c 'test "$(find "$1" -maxdepth 1 -type f -name "*.rpm" | wc -l)" -eq 8' _ "$ARTIFACT" || overall=1
run_record installed_all_nevra rpm --root "$SCRATCH" --dbpath /var/lib/rpm -qa --qf '%{NAME}\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\t%{ARCH}\n' || overall=1
run_record toolchain_nevra grep -E '^(binutils|clang|clang-accel|cmake|cpp|gcc|gcc-c\+\+|libgcc|libllvm|libstdc\+\+|libstdc\+\+-devel|lldb|llvm|llvm-devel|make|ninja|rpm-build)[[:space:]]' "$RAW/installed_all_nevra.stdout.txt" || overall=1
run_record copy_installed_nevra cp -a "$RAW/installed_all_nevra.stdout.txt" "$ARCH_OUT/installed_all_nevra.tsv" || overall=1
run_record copy_toolchain_nevra cp -a "$RAW/toolchain_nevra.stdout.txt" "$ARCH_OUT/toolchain_nevra.tsv" || overall=1
run_record copy_artifact_hashes cp -a "$RAW/archived_rpm_sha256.stdout.txt" "$ARCH_OUT/artifact_sha256.tsv" || overall=1
run_record root_retirement cat "$ROOT/RETIRED_SINGLE_USE" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
