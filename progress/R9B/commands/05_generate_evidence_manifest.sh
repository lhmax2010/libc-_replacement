#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LEDGER="$OUT/commands/05_generate_evidence_manifest.log"
MANIFEST="$OUT/evidence_manifest.tsv"

: >"$LEDGER"
{
    printf 'LABEL=generate_evidence_manifest\nPWD=%s\nCOMMAND=' "$REPO"
    printf '%q ' bash -o pipefail -c 'find "$1" -type f ! -path "$2" ! -path "$3" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum | sed "s#  $1/#\t#"' _ "$OUT" "$MANIFEST" "$LEDGER"
    printf '\n'
} >>"$LEDGER"
cd "$REPO" || exit 125
{
    printf 'sha256\tpath\n'
    find "$OUT" -type f ! -path "$MANIFEST" ! -path "$LEDGER" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum | sed "s#  $OUT/#\t#"
} >"$MANIFEST"
rc=$?
printf 'EXIT_CODE=%s\n' "$rc" >>"$LEDGER"
{
    printf '\nLABEL=verify_evidence_manifest\nPWD=%s\nCOMMAND=' "$REPO"
    printf '%q ' bash -o pipefail -c 'tail -n +2 "$1" | while IFS=$'"'"'\t'"'"' read -r expected rel; do test "$(sha256sum "$2/$rel" | cut -d" " -f1)" = "$expected" || exit 1; done' _ "$MANIFEST" "$OUT"
    printf '\n'
} >>"$LEDGER"
verify_rc=0
tail -n +2 "$MANIFEST" | while IFS=$'\t' read -r expected rel; do
    test "$(sha256sum "$OUT/$rel" | cut -d" " -f1)" = "$expected" || exit 1
done || verify_rc=$?
printf 'EXIT_CODE=%s\n' "$verify_rc" >>"$LEDGER"
[[ $rc -eq 0 && $verify_rc -eq 0 ]]
