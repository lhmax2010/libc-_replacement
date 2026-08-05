#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
LLVM="$REPO/codes/llvm"
COMMIT=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
PLATFORM_SHA=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e
LEDGER="$OUT/commands/01b_classify_source_deltas.log"

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

overall=0
cd "$REPO" || exit 125
record_to verify_artifact_identity_rows "$RAW/verify_artifact_identity_rows.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 24 && test "$(awk -F"\t" "NR>1 && \$5 != \"MATCH\" {n++} END {print n+0}" "$1")" -eq 0' \
  _ "$OUT/artifact_identity.tsv" || overall=1
record_to backport_commit_show "$RAW/backport_commit_show.stdout.txt" git -C "$LLVM" show --format=fuller --stat --patch "$COMMIT" || overall=1
record_to backport_commit_numstat "$RAW/backport_commit_numstat.stdout.txt" git -C "$LLVM" show --format= --numstat "$COMMIT" || overall=1
record_to verify_backport_scope "$RAW/verify_backport_scope.stdout.txt" bash -o pipefail -c \
  'test "$(wc -l < "$1")" -eq 1 && awk "\$1==2 && \$2==0 && \$3==\"libcxxabi/src/cxa_personality.cpp\" {ok=1} END {exit !ok}" "$1" && test "$(rg -c "^\\+extern \\\"C\\\" _Unwind_Reason_Code __gnu_unwind_frame" "$2")" -eq 1 && rg -q "^ #else$" "$2"' \
  _ "$RAW/backport_commit_numstat.stdout.txt" "$RAW/backport_commit_show.stdout.txt" || overall=1
record_to verify_platform_carriers "$RAW/verify_platform_carriers.stdout.txt" bash -o pipefail -c \
  'awk -F"\t" -v s="$2" "\$1==\"aarch64\" && \$2==s {a=1} \$1==\"x86_64\" && \$2==s {x=1} END {exit !(a&&x)}" "$1"' \
  _ "$OUT/source0_sha256.tsv" "$PLATFORM_SHA" || overall=1
record_to verify_arm_carrier_delta "$RAW/verify_arm_carrier_delta.stdout.txt" bash -o pipefail -c \
  'arm=$(awk -F"\t" "\$1==\"armv7l\" {print \$2}" "$1"); test -n "$arm" && test "$arm" != "$2" && test "$(wc -l < "$3")" -gt 1' \
  _ "$OUT/source0_sha256.tsv" "$PLATFORM_SHA" "$RAW/armv7l_vs_x86_source_tree_retry.diff" || overall=1
record_to provenance_contents_equal "$RAW/provenance_contents_equal.stdout.txt" bash -o pipefail -c \
  'cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ \
  "$OUT/provenance/armv7l.SOURCE_PROVENANCE" "$OUT/provenance/aarch64.SOURCE_PROVENANCE" \
  "$OUT/provenance/x86_64.SOURCE_PROVENANCE" || overall=1
record_to provenance_claim "$RAW/provenance_claim.stdout.txt" rg -n '^source_tarball_sha256=|^source_export=' \
  "$OUT/provenance/armv7l.SOURCE_PROVENANCE" || overall=1

arm_sha=$(awk -F'\t' '$1=="armv7l" {print $2}' "$OUT/source0_sha256.tsv")
{
  printf 'architecture\tidentity_scope\texpected\tobserved\tclassification\timpact\n'
  printf 'x86_64\tSRPM_Source0\t%s\t%s\tMATCH\tplatform_tarball\n' "$PLATFORM_SHA" "$PLATFORM_SHA"
  printf 'aarch64\tSRPM_Source0\t%s\t%s\tMATCH\tplatform_tarball\n' "$PLATFORM_SHA" "$PLATFORM_SHA"
  printf 'armv7l\tbackport_commit\tone_file_two_additions\tone_file_two_additions\tEXPECTED_BACKPORT_DELTA\tARM_EHABI_branch_only\n'
  printf 'armv7l\tSRPM_Source0\tplatform_tarball_plus_declared_backport\t%s\tUNEXPECTED_R6_GIT_EXPORT_DELTA\tfull_diff_in_raw/armv7l_vs_x86_source_tree_retry.diff\n' "$arm_sha"
  printf 'armv7l\tSOURCE_PROVENANCE_claim\tactual_Source0_hash\tdeclares_%s\tMISMATCH\tmetadata_does_not_describe_actual_R6_Source0\n' "$PLATFORM_SHA"
} >"$OUT/source_input_delta.tsv"

if [[ $overall -eq 0 ]]; then
  printf 'ARTIFACT_IDENTITY=PASS\n' >"$OUT/artifact_identity.status"
  printf 'SOURCE_PROVENANCE_CONTENTS=PASS_IDENTICAL\n' >"$OUT/source_provenance_consistency.status"
  printf 'SOURCE_INPUT_IDENTITY=RED_UNEXPECTED_R6_GIT_EXPORT_DELTA\n' >"$OUT/source_input_identity.status"
else
  printf 'ARTIFACT_IDENTITY=FAIL\n' >"$OUT/artifact_identity.status"
  printf 'SOURCE_PROVENANCE_CONTENTS=UNDETERMINED\n' >"$OUT/source_provenance_consistency.status"
  printf 'SOURCE_INPUT_IDENTITY=UNDETERMINED\n' >"$OUT/source_input_identity.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
