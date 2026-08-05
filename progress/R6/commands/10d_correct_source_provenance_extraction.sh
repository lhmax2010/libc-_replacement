#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
META="$OUT/rpm_meta"
RAW="$OUT/raw"
LEDGER="$OUT/commands/10d_correct_source_provenance_extraction.attempt2.log"
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
for arch in armv7l aarch64 x86_64; do
  rpmfile="$REPO/artifacts/rebuild_2218/$arch/libc++-devel-22.1.8-1.$arch.rpm"
  record_to "${arch}_source_provenance_corrected" "$META/$arch/SOURCE_PROVENANCE" \
    bash -o pipefail -c 'rpm2cpio "$1" | cpio -i --quiet --to-stdout ./usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE' _ "$rpmfile" || overall=1
  record_to "${arch}_source_provenance_size" "$RAW/${arch}_source_provenance_size.stdout.txt" \
    stat -c '%n\t%s bytes' "$META/$arch/SOURCE_PROVENANCE" || overall=1
done

record_to provenance_sha256_corrected "$OUT/three_arch_source_provenance_sha256.tsv" \
  sha256sum "$META/armv7l/SOURCE_PROVENANCE" "$META/aarch64/SOURCE_PROVENANCE" "$META/x86_64/SOURCE_PROVENANCE" || overall=1
record_to assert_provenance_nonempty "$RAW/assert_provenance_nonempty.stdout.txt" bash -o pipefail -c \
  'test -s "$1" && test -s "$2" && test -s "$3"' _ \
  "$META/armv7l/SOURCE_PROVENANCE" "$META/aarch64/SOURCE_PROVENANCE" "$META/x86_64/SOURCE_PROVENANCE" || overall=1
record_to assert_provenance_identical_corrected "$RAW/assert_provenance_identical_corrected.stdout.txt" bash -o pipefail -c \
  'cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ \
  "$META/armv7l/SOURCE_PROVENANCE" "$META/aarch64/SOURCE_PROVENANCE" "$META/x86_64/SOURCE_PROVENANCE" || overall=1

printf '%s\n' \
  'PREVIOUS_RESULT=EMPTY_EXTRACTION_FROM_WRONG_PACKAGE_libc++;SOURCE_PROVENANCE_IS_IN_libc++-devel' \
  'CORRECTED_RESULT=NONEMPTY_IDENTICAL_THREE_ARCH' \
  >"$OUT/source_provenance_extraction_correction.txt"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
