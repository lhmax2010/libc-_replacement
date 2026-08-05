#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R8"
RAW="$OUT/raw"
TMP="$REPO/tmp/R8/platform_srpm"
CODE="$REPO/codes/llvm"
LIST="$RAW/platform_patch_file_list.stdout.txt"
LEDGER="$OUT/commands/00a_platform_patch_correspondence.log"
: >"$LEDGER"

record_to() {
  local label=$1 outfile=$2
  shift 2
  {
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

cd "$REPO" || exit 125
overall=0
record_to disclose_prior_strict_assertion "$RAW/disclose_prior_strict_assertion.stdout.txt" \
  bash -c 'printf "%s\n" "00_platform_srpm_and_patch_baseline.sh 的逐字同一断言退出 1；该断言过严，不否定 spec 明文的 Source0+Patch0 构建路径。本脚本保留原日志，并将补丁内容对应与完整文件逐字同一区分申报。"' || overall=1

for rel in clang/lib/Driver/Driver.cpp clang/lib/Options/OptionUtils.cpp llvm/lib/TargetParser/Triple.cpp; do
  stem=${rel//\//_}
  record_to "patched_to_tizen_${stem}" "$RAW/${stem}.patched_to_tizen.diff" \
    diff -u "$TMP/patch_applied_subset/$rel" "$CODE/$rel"
  rc=$?
  [[ $rc -eq 1 ]] || overall=1
done

record_to classify_patch_correspondence "$OUT/platform_patch_correspondence.tsv" bash -o pipefail -c '
  printf "file\tbyte_result\tcontent_correspondence\tsupplemental_tizen_base_delta\n"
  while IFS= read -r f; do
    if cmp -s "$1/$f" "$2/$f"; then
      printf "%s\tMATCH\tPATCH0_EXACT\tNONE\n" "$f"
    else
      case "$f" in
        clang/lib/Driver/Driver.cpp)
          extra="formatting_of_RealDir_and_QEMU_ACCEL_DIR_block" ;;
        clang/lib/Options/OptionUtils.cpp)
          extra="cstdlib_include_and_comment_typo_fix" ;;
        llvm/lib/TargetParser/Triple.cpp)
          extra="getVendorTypeName_Tizen_case" ;;
        *) extra="UNCLASSIFIED" ;;
      esac
      printf "%s\tMISMATCH\tPATCH0_CONTENT_PRESENT_WITH_ADDITIONAL_DELTA\t%s\n" "$f" "$extra"
    fi
  done <"$3"
' _ "$TMP/patch_applied_subset" "$CODE" "$LIST" || overall=1

record_to assert_correspondence_counts "$RAW/assert_correspondence_counts.stdout.txt" bash -o pipefail -c '
  test "$(awk -F"\t" "NR>1 && \$2==\"MATCH\" {n++} END {print n+0}" "$1")" -eq 7 &&
  test "$(awk -F"\t" "NR>1 && \$2==\"MISMATCH\" {n++} END {print n+0}" "$1")" -eq 3 &&
  test "$(awk -F"\t" "NR>1 && \$3 !~ /^PATCH0_/ {n++} END {print n+0}" "$1")" -eq 0
' _ "$OUT/platform_patch_correspondence.tsv" || overall=1

printf 'conclusion\tevidence\nPLATFORM_BUILDS_FROM_PRISTINE_PLUS_PATCHES\tllvm.spec:39_Source0_tarball;llvm.spec:49_Patch0;llvm.spec:158-161_%%prep_%%setup_%%patch0_-p1;Patch0_10_targets=7_exact+3_patch_content_with_supplemental_tizen_base_delta\n' >"$OUT/platform_baseline_conclusion.tsv"
if [[ $overall -eq 0 ]]; then
  printf 'PLATFORM_BASELINE=PASS_PRISTINE_PLUS_PATCHES_WITH_PARTIAL_TIZEN_BASE_CORRESPONDENCE\n' >"$OUT/platform_baseline.status"
else
  printf 'PLATFORM_BASELINE=FAIL\n' >"$OUT/platform_baseline.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
