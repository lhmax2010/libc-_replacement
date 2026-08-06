#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
TARGET="$REPO/artifacts/quarantine/20260806_pre_c2"
LEDGER="$OUT/commands/09_quarantine_superseded_artifacts.log"
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
record assert_new_artifacts_gated bash -o pipefail -c \
  'rg -q "^IDENTITY_SELFCHECK=PASS$" "$1" && rg -q "^T4_RERUN=PASS_ALL_14_GATES_ALL_3_ARCHITECTURES$" "$2" && test "$(find "$3" -mindepth 2 -maxdepth 2 -type f -name "*.rpm" | wc -l)" -eq 24' _ \
  "$OUT/identity/identity_selfcheck.status" "$OUT/gates/t4_rerun.status" "$REPO/artifacts/rebuild_2218_c2" || exit 90
record assert_quarantine_target_absent test ! -e "$TARGET" || exit 91
record inventory_old_artifacts bash -o pipefail -c \
  'exec >"$5"; printf "source\ttype\tpath\tsize\tsha256\n"; for d in "$1" "$2" "$3"; do if test -e "$d"; then find "$d" -type f -print0 | LC_ALL=C sort -z | while IFS= read -r -d "" f; do printf "%s\tfile\t%s\t%s\t%s\n" "$(basename "$d")" "${f#"$4"/}" "$(stat -c %s "$f")" "$(sha256sum "$f" | cut -d" " -f1)"; done; else printf "%s\tNOT_FOUND\t%s\tNA\tNA\n" "$(basename "$d")" "${d#"$4"/}"; fi; done' _ \
  "$REPO/artifacts/rebuild_2218" "$REPO/artifacts/rebuild_2218_std" "$REPO/artifacts/rebuild_2218_p0" "$REPO" "$OUT/comparisons/quarantine_source_inventory.tsv" || exit 92
record create_quarantine_target mkdir -p "$TARGET" || exit 93
record move_rebuild_2218 mv "$REPO/artifacts/rebuild_2218" "$TARGET/rebuild_2218" || exit 94
if [[ -e "$REPO/artifacts/rebuild_2218_std" ]]; then
  record move_rebuild_2218_std mv "$REPO/artifacts/rebuild_2218_std" "$TARGET/rebuild_2218_std" || exit 95
else
  record rebuild_2218_std_not_found test ! -e "$REPO/artifacts/rebuild_2218_std" || exit 95
fi
if [[ -e "$REPO/artifacts/rebuild_2218_p0" ]]; then
  record move_rebuild_2218_p0 mv "$REPO/artifacts/rebuild_2218_p0" "$TARGET/rebuild_2218_p0" || exit 96
else
  record rebuild_2218_p0_not_found test ! -e "$REPO/artifacts/rebuild_2218_p0" || exit 96
fi
record write_supersession_marker bash -o pipefail -c \
  'printf "classification=SUPERSEDED_BY_R9C\nreplacement=%s\nquarantine=%s\naction=MOVE_ONLY_NO_DELETE\n" "$2" "$1" >"$1/SUPERSEDED_BY_R9C"' _ \
  "$TARGET" "$REPO/artifacts/rebuild_2218_c2" || exit 97
record verify_quarantine bash -o pipefail -c \
  'test ! -e "$1/artifacts/rebuild_2218" && test ! -e "$1/artifacts/rebuild_2218_std" && test -d "$2/rebuild_2218" && test -d "$2/rebuild_2218_std" && rg -q "SUPERSEDED_BY_R9C" "$2/SUPERSEDED_BY_R9C" && test "$(find "$1/artifacts/rebuild_2218_c2" -mindepth 2 -maxdepth 2 -type f -name "*.rpm" | wc -l)" -eq 24' _ \
  "$REPO" "$TARGET" || exit 98
record quarantine_final_inventory bash -o pipefail -c \
  'find "$1" -printf "%y\t%P\t%s\n" | LC_ALL=C sort >"$2"' _ "$TARGET" "$OUT/comparisons/quarantine_final_inventory.tsv" || exit 99

printf 'QUARANTINE_STATUS=PASS_MOVE_ONLY\nCLASSIFICATION=SUPERSEDED_BY_R9C\nTARGET=%s\n' "$TARGET" >"$OUT/comparisons/quarantine.status"
printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
exit 0
