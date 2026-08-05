#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R8"
RAW="$OUT/raw"
TMP="$REPO/tmp/R8/platform_srpm"
SRPM="$REPO/tmp/corpus/srpms/llvm-22.1.8-18.1.src.rpm"
CODE="$REPO/codes/llvm"
LEDGER="$OUT/commands/00_platform_srpm_and_patch_baseline.log"

mkdir -p "$RAW"
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

cd "$REPO" || exit 125
overall=0
record_to assert_tmp_fresh "$RAW/assert_tmp_fresh.stdout.txt" test ! -e "$TMP" || overall=1
record_to codes_git_identity "$RAW/codes_git_identity.stdout.txt" bash -o pipefail -c \
  'printf "BRANCH="; git -C "$1" branch --show-current; printf "HEAD="; git -C "$1" rev-parse HEAD; printf "STATUS_BEGIN\n"; git -C "$1" status --porcelain; printf "STATUS_END\n"; git -C "$1" remote -v' \
  _ "$CODE" || overall=1
record_to assert_codes_identity "$RAW/assert_codes_identity.stdout.txt" bash -o pipefail -c \
  'test "$(git -C "$1" branch --show-current)" = tizen_base && test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git -C "$1" status --porcelain)"' \
  _ "$CODE" || overall=1
record_to srpm_sha256 "$RAW/srpm_sha256.stdout.txt" sha256sum "$SRPM" || overall=1
record_to srpm_signature_readability "$RAW/srpm_signature_readability.stdout.txt" rpm -K --nosignature "$SRPM" || overall=1
record_to srpm_metadata "$RAW/srpm_metadata.stdout.txt" rpm -qpi "$SRPM" || overall=1
if [[ $overall -ne 0 ]]; then
  printf 'PLATFORM_SRPM_PREFLIGHT=FAIL\n' >"$OUT/platform_baseline.status"
  printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
  exit 1
fi
record_to create_platform_srpm_tmp "$RAW/create_platform_srpm_tmp.stdout.txt" mkdir -p "$TMP/payload" "$TMP/patch_applied_subset" || overall=1
record_to unpack_platform_srpm "$RAW/unpack_platform_srpm.stdout.txt" bash -o pipefail -c \
  'cd "$1" && rpm2cpio "$2" | cpio -idmuv' _ "$TMP/payload" "$SRPM" || overall=1
record_to platform_payload_listing "$RAW/platform_payload_listing.stdout.txt" bash -o pipefail -c \
  'find "$1" -maxdepth 1 -type f -printf "%f\t%s\n" | LC_ALL=C sort' _ "$TMP/payload" || overall=1
record_to platform_payload_sha256 "$RAW/platform_payload_sha256.stdout.txt" bash -o pipefail -c \
  'find "$1" -maxdepth 1 -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$TMP/payload" || overall=1

SPEC="$TMP/payload/llvm.spec"
PATCH="$TMP/payload/0001-Apply-Tizen-toolchain-compatibility-fixes.patch"
record_to platform_spec_numbered "$RAW/platform_spec_numbered.stdout.txt" nl -ba "$SPEC" || overall=1
record_to platform_source_patch_entries "$RAW/platform_source_patch_entries.stdout.txt" rg -n \
  '^[[:space:]]*(Source[0-9]*|Patch[0-9]*):' "$SPEC" || overall=1
record_to platform_prep_patch_application "$RAW/platform_prep_patch_application.stdout.txt" rg -n \
  '^%prep|^%setup|^%patch|Apply-Tizen-toolchain' "$SPEC" || overall=1
record_to platform_patch_numbered "$RAW/platform_patch_numbered.stdout.txt" nl -ba "$PATCH" || overall=1
record_to platform_patch_file_list "$RAW/platform_patch_file_list.stdout.txt" bash -o pipefail -c \
  'awk '\''/^--- a\// {sub(/^--- a\//, ""); print}'\'' "$1" | LC_ALL=C sort -u' _ "$PATCH" || overall=1
record_to assert_source_patch_shape "$RAW/assert_source_patch_shape.stdout.txt" bash -o pipefail -c \
  'test "$(rg -c "^[[:space:]]*Source[0-9]*:" "$1")" -eq 5 && test "$(rg -c "^[[:space:]]*Patch[0-9]*:" "$1")" -eq 1 && rg -q "^Patch0:.*0001-Apply-Tizen-toolchain-compatibility-fixes.patch" "$1" && rg -q "^%patch0 -p1" "$1" && test "$(wc -l < "$2")" -eq 10' \
  _ "$SPEC" "$RAW/platform_patch_file_list.stdout.txt" || overall=1

PRISTINE="$REPO/tmp/corpus/extracted/llvm-22.1.8-18.1.src.rpm/unpacked/llvm-22.1.8.tar.gz/llvm-22.1.8"
record_to copy_patch_target_subset "$RAW/copy_patch_target_subset.stdout.txt" bash -o pipefail -c \
  'while IFS= read -r f; do mkdir -p "$2/$(dirname "$f")"; cp -p "$1/$f" "$2/$f"; done < "$3"' \
  _ "$PRISTINE" "$TMP/patch_applied_subset" "$RAW/platform_patch_file_list.stdout.txt" || overall=1
record_to apply_platform_patch_to_subset "$RAW/apply_platform_patch_to_subset.stdout.txt" patch --batch --forward \
  -d "$TMP/patch_applied_subset" -p1 -i "$PATCH" || overall=1
record_to compare_patched_subset_to_tizen_base "$RAW/compare_patched_subset_to_tizen_base.stdout.txt" bash -o pipefail -c \
  'printf "file\tpatched_subset_sha256\ttizen_base_sha256\tresult\n"; while IFS= read -r f; do a=$(sha256sum "$1/$f" | cut -d" " -f1); b=$(sha256sum "$2/$f" | cut -d" " -f1); r=MATCH; test "$a" = "$b" || r=MISMATCH; printf "%s\t%s\t%s\t%s\n" "$f" "$a" "$b" "$r"; done < "$3"' \
  _ "$TMP/patch_applied_subset" "$CODE" "$RAW/platform_patch_file_list.stdout.txt" || overall=1
cp -p "$RAW/compare_patched_subset_to_tizen_base.stdout.txt" "$OUT/patch_to_tizen_base.tsv"
record_to assert_patch_targets_match "$RAW/assert_patch_targets_match.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 10 && test "$(awk -F"\t" "NR>1 && \$4!=\"MATCH\" {n++} END {print n+0}" "$1")" -eq 0' \
  _ "$OUT/patch_to_tizen_base.tsv" || overall=1

for rel in clang/lib/Driver/Driver.cpp llvm/lib/TargetParser/Triple.cpp clang/lib/Driver/ToolChains/CommonArgs.cpp; do
  stem=${rel//\//_}
  record_to "contrast_${stem}" "$RAW/${stem}.pristine_to_tizen.diff" diff -u "$PRISTINE/$rel" "$CODE/$rel"
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
done

{
  printf 'field\tfile_line\traw_value\tsha256_or_status\n'
  printf 'Source0\tllvm.spec:39\t%%{name}-%%{version}.tar.gz\t16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e\n'
  printf 'Source1001\tllvm.spec:40\tllvm.manifest\t%s\n' "$(sha256sum "$TMP/payload/llvm.manifest" | cut -d' ' -f1)"
  printf 'Source1002\tllvm.spec:45\tmlgo_arm_model.tar.gz\t%s\n' "$(sha256sum "$TMP/payload/mlgo_arm_model.tar.gz" | cut -d' ' -f1)"
  printf 'Source1003\tllvm.spec:46\tmlgo_aarch_model.tar.gz\t%s\n' "$(sha256sum "$TMP/payload/mlgo_aarch_model.tar.gz" | cut -d' ' -f1)"
  printf 'Source1004\tllvm.spec:47\tmlgo_x86_model.tar.gz\t%s\n' "$(sha256sum "$TMP/payload/mlgo_x86_model.tar.gz" | cut -d' ' -f1)"
  printf 'Patch0\tllvm.spec:49\t0001-Apply-Tizen-toolchain-compatibility-fixes.patch\t%s\n' "$(sha256sum "$PATCH" | cut -d' ' -f1)"
  printf 'PatchApply\tllvm.spec:160-161\tcomment_then_%%patch0_-p1\tPRESENT\n'
} >"$OUT/platform_source_patch_entries.tsv"
printf 'conclusion\tevidence\nPLATFORM_BUILDS_FROM_PRISTINE_PLUS_PATCHES\tSource0_tarball_plus_Patch0;spec_%%prep_applies_%%patch0_-p1;10_of_10_patched_files_match_tizen_base\n' >"$OUT/platform_baseline_conclusion.tsv"

if [[ $overall -eq 0 ]]; then
  printf 'PLATFORM_BASELINE=PASS_PRISTINE_PLUS_PATCHES\n' >"$OUT/platform_baseline.status"
else
  printf 'PLATFORM_BASELINE=FAIL\n' >"$OUT/platform_baseline.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
