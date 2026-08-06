#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
LEDGER="$OUT/commands/10_final_audit_and_manifest.log"
: >"$LEDGER"

record() {
  local label=$1
  shift
  {
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >>"$LEDGER" 2>&1
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

cd "$REPO" || exit 125
overall=0
record assert_final_statuses bash -o pipefail -c \
  'rg -q "^IDENTITY_SELFCHECK=PASS$" "$1" && rg -q "^T4_RERUN=PASS_ALL_14_GATES_ALL_3_ARCHITECTURES$" "$2" && rg -q "^QUARANTINE_STATUS=PASS_MOVE_ONLY$" "$3"' _ \
  "$OUT/identity/identity_selfcheck.status" "$OUT/gates/t4_rerun.status" "$OUT/comparisons/quarantine.status" || overall=1
record assert_artifacts bash -o pipefail -c \
  'test "$(find "$1" -mindepth 2 -maxdepth 2 -type f -name "*.rpm" | wc -l)" -eq 24 && test "$(tail -n +2 "$2" | wc -l)" -eq 24 && while IFS=$'\''\t'\'' read -r arch sha file; do test "$arch" = architecture && continue; test "$(sha256sum "$1/$arch/$file" | cut -d" " -f1)" = "$sha" || exit 1; done <"$2"' _ \
  "$REPO/artifacts/rebuild_2218_c2" "$OUT/identity/new_artifact_sha256.tsv" || overall=1
record assert_provenance_and_source bash -o pipefail -c \
  'test "$(sha256sum "$1" | cut -d" " -f1)" = 1cd5a45ea027d8f22b8dc5812eebbcf56f6c52e42446ad531729ce80289e7a37 && test "$(git -C "$2" rev-parse --abbrev-ref HEAD)" = tizen_base && test "$(git -C "$2" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git -C "$2" status --porcelain=v1)"' _ \
  "$REPO/packaging/SOURCE_PROVENANCE" "$REPO/codes/llvm" || overall=1
record assert_quarantine_and_boundary bash -o pipefail -c \
  'test -d "$1/artifacts/quarantine/20260806_pre_c2/rebuild_2218" && test ! -e "$1/artifacts/rebuild_2218" && rg -q "SUPERSEDED_BY_R9C" "$1/artifacts/quarantine/20260806_pre_c2/SUPERSEDED_BY_R9C" && rg -q "PENDING_BOARD_VERIFICATION" "$2"' _ \
  "$REPO" "$OUT/gates/coverage_boundary.tsv" || overall=1
record assert_report_sections bash -o pipefail -c \
  'for x in SOURCE_PROVENANCE 三架构构建 身份自检 T4 新旧制品 覆盖边界 PENDING_BOARD_VERIFICATION; do rg -q "$x" "$1" || exit 1; done' _ \
  "$OUT/R9C_segment2_report.md" || overall=1

record write_curated_manifest bash -o pipefail -c \
  'find "$1" -type f ! -path "*/gates/*/static/artifacts/*" ! -path "*/gates/*/static/extracted/*" ! -path "*/probes/*.bin" ! -path "*/commands/10_final_audit_and_manifest.log" ! -name final_audit.status ! -name evidence_manifest.tsv -print0 | LC_ALL=C sort -z | while IFS= read -r -d "" f; do printf "%s\t%s\t%s\n" "$(sha256sum "$f" | cut -d" " -f1)" "$(stat -c %s "$f")" "${f#"$2"/}"; done >"$1/evidence_manifest.tsv"' _ \
  "$OUT" "$REPO" || overall=1
record assert_manifest_publishable bash -o pipefail -c \
  'test -s "$1" && test "$(awk -F"\t" "\$2>=100000000 {n++} END {print n+0}" "$1")" -eq 0 && ! rg -q "/static/artifacts/|/static/extracted/|/probes/.*\\.bin$" "$1"' _ \
  "$OUT/evidence_manifest.tsv" || overall=1
record final_project_status git status --short || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'FINAL_AUDIT=PASS\nPUBLISH_SET=CURATED_NO_RPM_OR_EXTRACTED_PAYLOAD\n' >"$OUT/final_audit.status"
else
  printf 'FINAL_AUDIT=RED\n' >"$OUT/final_audit.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
