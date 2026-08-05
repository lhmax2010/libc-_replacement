#!/usr/bin/env bash
set -u
set -o pipefail
REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R8"
RAW="$OUT/raw"
LEDGER="$OUT/commands/04_finalize_r8.log"
: >"$LEDGER"

record_to() {
  local label=$1 outfile=$2
  shift 2
  { printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"; printf '%q ' "$@"; printf '\n'; } >>"$LEDGER"
  "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

cd "$REPO" || exit 125
overall=0
record_to assert_final_statuses "$RAW/assert_final_statuses.stdout.txt" bash -o pipefail -c '
  grep -Fxq "PLATFORM_BASELINE=PASS_PRISTINE_PLUS_PATCHES_WITH_PARTIAL_TIZEN_BASE_CORRESPONDENCE" "$1" &&
  grep -Fxq "RUNTIME_TREE_DIFF=PASS_ONLY_EHABI_BACKPORT" "$2" &&
  grep -Fxq "BUILD_INTERSECTION=PASS_ONLY_EHABI_BACKPORT;NONRUNTIME_TIZEN_DELTA=NO_INTERSECTION" "$3" &&
  grep -Fxq "PAYLOAD_PROVENANCE_AUDIT=PASS_WITH_DECLARED_ARM_PROVENANCE_MISMATCHES" "$4"
' _ "$OUT/platform_baseline.status" "$OUT/runtime_tree_diff.status" "$OUT/build_intersection.status" "$OUT/payload_provenance.status" || overall=1
record_to assert_report_shape "$RAW/assert_report_shape.stdout.txt" bash -o pipefail -c '
  rg -n "^## (1|2|3|4|5|6)\\." "$1" &&
  rg -q "PLATFORM_BUILDS_FROM_PRISTINE_PLUS_PATCHES" "$1" &&
  rg -q "NO_INTERSECTION" "$1" &&
  rg -q "不包含处置建议" "$1"
' _ "$OUT/R8_report.md" || overall=1
record_to final_codes_llvm_identity "$RAW/final_codes_llvm_identity.stdout.txt" bash -o pipefail -c '
  printf "BRANCH="; git -C "$1" branch --show-current
  printf "HEAD="; git -C "$1" rev-parse HEAD
  printf "STATUS_BEGIN\n"; git -C "$1" status --porcelain; printf "STATUS_END\n"
' _ "$REPO/codes/llvm" || overall=1
record_to assert_codes_llvm_unchanged "$RAW/assert_codes_llvm_unchanged.stdout.txt" bash -o pipefail -c '
  test "$(git -C "$1" branch --show-current)" = tizen_base &&
  test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b &&
  test -z "$(git -C "$1" status --porcelain)"
' _ "$REPO/codes/llvm" || overall=1
record_to final_parent_worktree_status "$RAW/final_parent_worktree_status.stdout.txt" git status --porcelain || overall=1
record_to assert_no_tracked_input_changes "$RAW/assert_no_tracked_input_changes.stdout.txt" git diff --quiet -- packaging artifacts || overall=1
record_to final_artifact_sha256 "$OUT/final_artifact_sha256.tsv" bash -o pipefail -c '
  printf "arch\trpm\tsha256\n"
  for arch in x86_64 aarch64 armv7l; do
    for f in "$1/$arch"/*.rpm; do printf "%s\t%s\t%s\n" "$arch" "$(basename "$f")" "$(sha256sum "$f" | cut -d" " -f1)"; done
  done | LC_ALL=C sort
' _ "$REPO/artifacts/rebuild_2218" || overall=1

cat >"$OUT/evidence_index.tsv" <<'EOF'
topic	primary_evidence
platform_spec_full	progress/R8/raw/platform_spec_numbered.stdout.txt
platform_source_patch_entries	progress/R8/platform_source_patch_entries.tsv
platform_patch_full	progress/R8/raw/platform_patch_numbered.stdout.txt
patch_correspondence	progress/R8/platform_patch_correspondence.tsv
runtime_tree_diff	progress/R8/raw/{libcxx,libcxxabi,libunwind,runtimes,cmake,third_party}.diff_qr.txt
all_source_differences	progress/R8/source_difference_files.tsv
r6_actual_tu	progress/R8/r6_actual_compiled_tu.tsv
r6_actual_headers	progress/R8/r6_actual_object_dependencies.tsv
r6_actual_cmake	progress/R8/r6_cmake_consumed_inputs.tsv
build_intersection	progress/R8/difference_build_intersection.tsv
rpm_payload_inventory	progress/R8/rpm_payload_inventory.tsv
payload_path_differences	progress/R8/payload_path_differences.tsv
payload_source_attribution	progress/R8/payload_source_attribution.tsv
actual_source0	progress/R8/actual_source0_identity.tsv
provenance_raw	progress/R8/SOURCE_PROVENANCE.{x86_64,aarch64,armv7l}.txt
provenance_audit	progress/R8/provenance_field_audit.tsv
report	progress/R8/R8_report.md
commands	progress/R8/commands/*.log
EOF

if [[ $overall -eq 0 ]]; then
  printf 'R8=COMPLETE_READ_ONLY;PLATFORM=PRISTINE_PLUS_PATCHES;RUNTIME_DIFF=ONLY_EHABI_BACKPORT;NONRUNTIME_BUILD_INTERSECTION=NONE;ARM_PROVENANCE_MISMATCH_FIELDS=2\n' >"$OUT/R8_completion.status"
  printf 'task\tstatus\ttimestamp\tnote\nR8\tDONE\t%s\tread_only_baseline_attribution_complete\n' "$(date --iso-8601=seconds)" >"$REPO/progress/state/R8.tsv"
else
  printf 'R8=FAIL_OR_UNDETERMINED\n' >"$OUT/R8_completion.status"
fi
record_to r8_output_sha256_manifest "$OUT/R8_output_sha256.tsv" bash -o pipefail -c '
  find "$1" -type f ! -name R8_output_sha256.tsv ! -path "*/commands/04_finalize_r8.log" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum
' _ "$OUT" || overall=1
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
