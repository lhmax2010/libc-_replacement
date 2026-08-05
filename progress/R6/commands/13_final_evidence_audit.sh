#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
LEDGER="$OUT/commands/13_final_evidence_audit.log"
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
record_to assert_report_sections "$RAW/assert_report_sections.stdout.txt" rg -n \
  'SOURCE_IDENTICAL|3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b|7 个二进制 RPM|强未解析符号|不进入 T4' \
  "$OUT/repair_validation_report.md" || overall=1
record_to assert_source_delivery_identity "$RAW/assert_source_delivery_identity_final.stdout.txt" \
  bash -o pipefail -c \
  'test "$(git branch --show-current)" = tizen_base && test "$(git rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git status --porcelain=v1)"' \
  || overall=1
record_to assert_frozen_input_hashes "$RAW/assert_frozen_input_hashes_final.stdout.txt" \
  bash -o pipefail -c \
  'test "$(sha256sum "$1" | cut -d" " -f1)" = a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74 && test "$(sha256sum "$2" | cut -d" " -f1)" = e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d && test "$(sha256sum "$3" | cut -d" " -f1)" = db262cd0f01ca668a019b79bf1bb1a9731a100205ff4d4b256d071b3d5304dc3 && test "$(sha256sum "$4" | cut -d" " -f1)" = 1ab73051dc45ee72e681ec0393889bdf527c4d7407df13eb3effa79c7dd4be74' \
  _ "$REPO/packaging/libcxx-runtimes.spec" "$REPO/packaging/libcxx-llvm22.map" \
  "$REPO/packaging/libcxx-runtimes.manifest" "$REPO/packaging/SOURCE_PROVENANCE" || overall=1
record_to verify_artifact_hashes "$RAW/verify_artifact_hashes_final.stdout.txt" \
  sha256sum -c "$OUT/armv7l/artifact_sha256.tsv" || overall=1
record_to assert_artifact_count "$RAW/assert_artifact_count_final.stdout.txt" \
  bash -o pipefail -c \
  'test "$(find "$1" -maxdepth 1 -type f -name "*.rpm" ! -name "*.src.rpm" | wc -l)" -eq 7 && test "$(find "$1" -maxdepth 1 -type f -name "*.src.rpm" | wc -l)" -eq 1' \
  _ "$REPO/artifacts/rebuild_2218/armv7l" || overall=1
record_to assert_retired_single_use "$RAW/assert_retired_single_use_final.stdout.txt" test -f \
  "$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/RETIRED_SINGLE_USE" || overall=1
record_to assert_provenance_final "$RAW/assert_provenance_final.stdout.txt" \
  bash -o pipefail -c \
  'test -s "$1" && cmp -s "$1" "$2" && cmp -s "$1" "$3"' \
  _ "$OUT/rpm_meta/armv7l/SOURCE_PROVENANCE" "$OUT/rpm_meta/aarch64/SOURCE_PROVENANCE" \
  "$OUT/rpm_meta/x86_64/SOURCE_PROVENANCE" || overall=1
record_to assert_elf_summary "$RAW/assert_elf_summary_final.stdout.txt" \
  bash -o pipefail -c \
  'test "$(awk -F"\t" "NR>1 && \$3 != 0 {n++} END {print n+0}" "$1")" -eq 0 && test "$(rg -c "PASS" "$1")" -eq 2' \
  _ "$OUT/armv7l_elf_check_summary.tsv" || overall=1
record_to assert_no_external_push_command "$RAW/assert_no_external_push_command_final.stdout.txt" \
  bash -o pipefail -c '! rg -n "^COMMAND=git push" "$1"' _ "$OUT/commands" || overall=1
record_to manifest_without_self_and_live_ledger "$OUT/MANIFEST.sha256" \
  bash -o pipefail -c \
  'find "$1" -type f ! -path "$1/MANIFEST.sha256" ! -path "$1/commands/13_final_evidence_audit.log" -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' \
  _ "$OUT" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
