#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
LEDGER="$OUT/commands/00_inventory_and_freeze_preflight.log"
OLD_REPORT=/home/toolchain/development/libc++_replacement_bak/libc-_replacement/packaging/multiarch_packaging_report.md
OLD_MULTI=/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch

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

overall=0
cd "$REPO" || exit 125
record_to disk_guard "$RAW/disk_guard.stdout.txt" df -BG "$REPO" || overall=1
record_to assert_disk_guard "$RAW/assert_disk_guard.stdout.txt" bash -o pipefail -c \
  'test "$(df --output=avail -B1 "$1" | tail -1)" -ge 80000000000' _ "$REPO" || overall=1
record_to old_report_sha256 "$RAW/old_report_sha256.stdout.txt" sha256sum "$OLD_REPORT" || overall=1
record_to old_report_numbered "$RAW/old_report_numbered.stdout.txt" nl -ba "$OLD_REPORT" || overall=1
record_to old_gate_verdicts_numbered "$RAW/old_gate_verdicts_numbered.stdout.txt" bash -o pipefail -c \
  'for arch in armv7l aarch64 x86_64; do printf "===== %s =====\n" "$arch"; nl -ba "$1/$arch/gate_verdict.tsv"; done' \
  _ "$OLD_MULTI" || overall=1
record_to old_static_tool_numbered "$RAW/old_static_tool_numbered.stdout.txt" nl -ba "$OLD_MULTI/run_static_gates.sh" || overall=1
record_to old_runtime_tool_numbered "$RAW/old_runtime_tool_numbered.stdout.txt" nl -ba "$OLD_MULTI/run_runtime_gates.sh" || overall=1
record_to old_reference_sha256 "$RAW/old_reference_sha256.stdout.txt" sha256sum \
  "$OLD_MULTI/run_static_gates.sh" "$OLD_MULTI/run_runtime_gates.sh" \
  "$OLD_MULTI/armv7l/gate_verdict.tsv" "$OLD_MULTI/aarch64/gate_verdict.tsv" \
  "$OLD_MULTI/x86_64/gate_verdict.tsv" || overall=1
record_to t5_freeze_entry_count "$RAW/t5_freeze_entry_count.stdout.txt" wc -l gates/FREEZE_MANIFEST.sha256 || overall=1
record_to t5_freeze_verify "$RAW/t5_freeze_verify.stdout.txt" bash -o pipefail -c \
  'cd gates && sha256sum -c FREEZE_MANIFEST.sha256' || overall=1
record_to active_static_tool_sha256 "$RAW/active_static_tool_sha256.stdout.txt" sha256sum gates/tools/run_static_gates.sh || overall=1
record_to assert_t5_freeze "$RAW/assert_t5_freeze.stdout.txt" bash -o pipefail -c \
  'test "$(wc -l < gates/FREEZE_MANIFEST.sha256)" -eq 145 && test "$(sha256sum gates/tools/run_static_gates.sh | cut -d" " -f1)" = d24f7c234d266bf970dbfd9c24939052681eba2105c186d0002009891c119885' \
  || overall=1
record_to gate_inventory "$RAW/gate_inventory.stdout.txt" cat "$OUT/gate_inventory.tsv" || overall=1
record_to assert_gate_inventory_count "$RAW/assert_gate_inventory_count.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 14 && test "$(cut -f1 "$1" | tail -n +2 | sort -u | wc -l)" -eq 14' \
  _ "$OUT/gate_inventory.tsv" || overall=1
record_to no_tool_mapping_list "$OUT/no_tool_mapping.tsv" bash -o pipefail -c \
  'awk -F"\t" "NR==1 || \$5 ~ /^NO_TOOL_MAPPING/" "$1"' _ "$OUT/gate_inventory.tsv" || overall=1
record_to fixture_sha256 "$RAW/fixture_sha256.stdout.txt" sha256sum \
  /home/toolchain/plan_evaluation/p1/libcxx_packaging/fixtures/smoke.cpp \
  /home/toolchain/plan_evaluation/p1/libcxx_packaging/fixtures/mixed_throw.cpp \
  /home/toolchain/plan_evaluation/p1/libcxx_packaging/fixtures/dlopen_driver.c || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'INVENTORY_AND_FREEZE_PREFLIGHT=PASS\n' >"$OUT/inventory_preflight.status"
else
  printf 'INVENTORY_AND_FREEZE_PREFLIGHT=FAIL\n' >"$OUT/inventory_preflight.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
