#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
META="$OUT/rpm_meta"
RAW="$OUT/raw"
LEDGER="$OUT/commands/10a_compare_package_names_and_provenance_retry.log"
: >"$LEDGER"

record_to() {
  local label=$1
  local outfile=$2
  shift 2
  {
    printf 'LABEL=%s\n' "$label"
    printf 'PWD=%s\n' "$PWD"
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

printf 'PREVIOUS_ATTEMPT=10_collect_three_arch_rpm_metadata.sh\nPREVIOUS_RESULT=COMPARISON_HELPER_ARGUMENT_ERROR;RPM_METADATA_COLLECTION_SUCCEEDED\n' >"$OUT/rpm_metadata_compare_retry_disclosure.txt"
overall=0
for arch in armv7l aarch64 x86_64; do
  record_to "${arch}_package_names_retry" "$META/$arch/package_names.txt" find "$META/$arch" -mindepth 1 -maxdepth 1 -type d -printf '%f\n' || overall=1
  LC_ALL=C sort -o "$META/$arch/package_names.txt" "$META/$arch/package_names.txt"
done
record_to package_names_arm_vs_aarch_retry "$OUT/package_names_arm_vs_aarch.diff" diff -u "$META/armv7l/package_names.txt" "$META/aarch64/package_names.txt" || {
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
}
record_to package_names_arm_vs_x86_retry "$OUT/package_names_arm_vs_x86.diff" diff -u "$META/armv7l/package_names.txt" "$META/x86_64/package_names.txt" || {
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
}
record_to assert_package_name_sets_identical_retry "$RAW/assert_package_name_sets_identical_retry.stdout.txt" bash -o pipefail -c 'cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ "$META/armv7l/package_names.txt" "$META/aarch64/package_names.txt" "$META/x86_64/package_names.txt" || overall=1
record_to assert_provenance_identical_retry "$RAW/assert_provenance_identical_retry.stdout.txt" bash -o pipefail -c 'cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ "$META/armv7l/SOURCE_PROVENANCE" "$META/aarch64/SOURCE_PROVENANCE" "$META/x86_64/SOURCE_PROVENANCE" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
