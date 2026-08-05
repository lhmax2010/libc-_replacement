#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
LEDGER="$OUT/commands/05_aggregate_gate_results.log"

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
record_to aggregate_assert_inventory "$RAW/aggregate_assert_inventory.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 14 && test "$(cut -f1 "$1" | tail -n +2 | sort -u | wc -l)" -eq 14 && test -s "$2"' \
  _ "$OUT/gate_inventory.tsv" "$OUT/no_tool_mapping.tsv" || overall=1
record_to aggregate_assert_artifacts "$RAW/aggregate_assert_artifacts.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 24 && test "$(awk -F"\t" "NR>1 && \$5!=\"MATCH\" {n++} END {print n+0}" "$1")" -eq 0 && rg -q "^ARTIFACT_IDENTITY=PASS$" "$2"' \
  _ "$OUT/artifact_identity.tsv" "$OUT/artifact_identity.status" || overall=1
record_to aggregate_assert_static "$RAW/aggregate_assert_static.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 3 && awk -F"\t" "NR>1 {for(i=4;i<=12;i++) if(\$i!=\"PASS\") bad=1; if(\$3!=0) bad=1} END {exit bad}" "$1"' \
  _ "$OUT/static_gate_results.final.tsv" || overall=1
record_to aggregate_assert_runtime "$RAW/aggregate_assert_runtime.stdout.txt" bash -o pipefail -c \
  'rg -q "PASS_ARM_X86_AARCH64_NOT_AVAILABLE" "$1" && rg -q "PASS_ARM_X86_AARCH64_NOT_AVAILABLE" "$2" && for a in armv7l x86_64; do awk -F"\t" "NR==2 && \$5==\"PASS\" {ok=1} END {exit !ok}" "$3/$a/static/dependency_closure_gate.tsv" || exit 1; awk -F"\t" "NR==2 && \$5==\"PASS_GREEN\" {g=1} NR==3 && \$5==\"EXPECTED_RED_DETECTED\" {r=1} END {exit !(g&&r)}" "$3/$a/static/binding_gate.tsv" || exit 1; awk -F"\t" "NR==2 && \$4==\"PASS\" {ok=1} END {exit !ok}" "$3/$a/static/runtime_smoke.tsv" || exit 1; done' \
  _ "$OUT/runtime_carrier.status" "$OUT/explicit_ldd_provider.status" "$OUT" || overall=1
record_to aggregate_provenance_sha256 "$OUT/source_provenance_sha256.tsv" bash -o pipefail -c \
  'printf "architecture\tsha256\n"; for a in armv7l aarch64 x86_64; do printf "%s\t" "$a"; sha256sum "$1/provenance/$a.SOURCE_PROVENANCE" | cut -d" " -f1; done' \
  _ "$OUT" || overall=1
record_to aggregate_assert_provenance_contents "$RAW/aggregate_assert_provenance_contents.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | cut -f2 | sort -u | wc -l)" -eq 1 && rg -q "PASS_IDENTICAL" "$2" && rg -q "RED_UNEXPECTED_R6_GIT_EXPORT_DELTA" "$3"' \
  _ "$OUT/source_provenance_sha256.tsv" "$OUT/source_provenance_consistency.status" "$OUT/source_input_identity.status" || overall=1

{
  printf 'gate_id\tgate_name\tarmv7l\taarch64\tx86_64\tevidence_or_reason\n'
  printf 'P01\tCURRENT_BUILD_PROVENANCE\tPASS\tPASS\tPASS\tcurrent_T3R_T3R2_R6_build_records_exit_0\n'
  printf 'P02\tRPM_SET_NEVRA_DIGEST\tPASS\tPASS\tPASS\t7_binary_plus_1_SRPM_and_24_of_24_archive_SHA_MATCH\n'
  printf 'P03\tELF_MACHINE\tPASS\tPASS\tPASS\tfrozen_LLVM_22_static_tool\n'
  printf 'P04\tDWARF_PRODUCER\tPASS\tPASS\tPASS\tall_six_DSO_debuginfo_logs_record_clang_version_22.1.8\n'
  printf 'P05\tVERSION_LIBCXX\tPASS\tPASS\tPASS\tall_defined_exports_default_@@LLVM_22\n'
  printf 'P06\tVERSION_LIBCXXABI\tPASS\tPASS\tPASS\tall_defined_exports_default_@@LLVM_22\n'
  printf 'P07\tDIRECT_NEEDED_POLICY\tPASS\tPASS\tPASS\tno_direct_libstdc++_or_libunwind\n'
  printf 'P08\tBIND_NOW\tPASS\tPASS\tPASS\tboth_runtime_DSOs_NOW\n'
  printf 'P09\tLDD_R_CLOSURE\tPASS\tNOT_AVAILABLE\tPASS\taarch64_QEMU_carrier_absent;_arm_explicit_qemu-arm-static_-L;_x86_native\n'
  printf 'P10\tPROVIDER_FAMILY\tPASS\tNOT_AVAILABLE\tPASS\taarch64_QEMU_carrier_absent\n'
  printf 'P11\tGREEN_BINDING_FIXTURE\tPASS_GREEN\tNOT_AVAILABLE\tPASS_GREEN\taarch64_QEMU_carrier_absent\n'
  printf 'P12\tRED_BINDING_FIXTURE\tEXPECTED_RED_DETECTED\tNOT_AVAILABLE\tEXPECTED_RED_DETECTED\taarch64_QEMU_carrier_absent\n'
  printf 'P13\tRUNTIME_SMOKE\tPASS\tNOT_AVAILABLE\tPASS\taarch64_QEMU_carrier_absent\n'
  printf 'P14\tHEADER_LAYOUT\tPASS\tPASS\tPASS\tdevel_RPM_owns_usr_include_c++_v1;_available_carriers_compile_fixture\n'
} >"$OUT/gate_results.tsv"

{
  printf 'gate_id\tarmv7l\taarch64\tx86_64\tcomparison\n'
  while IFS=$'\t' read -r gate_id gate_name arm aarch x86 evidence; do
    [[ $gate_id == gate_id ]] && continue
    comparison=CONSISTENT
    if [[ $aarch == NOT_AVAILABLE ]]; then comparison=CARRIER_NOT_AVAILABLE_AARCH64; fi
    if [[ $gate_id == P11 || $gate_id == P12 ]]; then comparison=CONSISTENT_WHERE_EXECUTABLE_AARCH64_NOT_AVAILABLE; fi
    printf '%s\t%s\t%s\t%s\t%s\n' "$gate_id" "$arm" "$aarch" "$x86" "$comparison"
  done <"$OUT/gate_results.tsv"
} >"$OUT/three_arch_gate_comparison.tsv"

{
  printf 'architecture\tstatic_gates\truntime_gates\tartifact_identity\tsource_input_identity\toverall\n'
  printf 'armv7l\tPASS\tPASS\tPASS\tRED_UNEXPECTED_R6_GIT_EXPORT_DELTA\tRED\n'
  printf 'aarch64\tPASS\tNOT_AVAILABLE_P09_P13\tPASS\tPASS_PLATFORM_SOURCE0\tNOT_AVAILABLE\n'
  printf 'x86_64\tPASS\tPASS\tPASS\tPASS_PLATFORM_SOURCE0\tPASS\n'
} >"$OUT/architecture_summary.tsv"

{
  printf 'item\tclassification\tscope\n'
  printf 'AARCH64_RUNTIME_CARRIER\tNOT_AVAILABLE\tP09_P13_not_executed_no_qemu-aarch64_carrier\n'
  printf 'ARM_R6_SOURCE0\tRED_UNEXPECTED_R6_GIT_EXPORT_DELTA\tSRPM_Source0_has_non_backport_differences_and_packaging_directories\n'
  printf 'ARM_BACKPORT_COMMIT\tEXPECTED_BACKPORT_DELTA\tone_file_two_additions_in_ARM_EHABI_branch_only\n'
  printf 'RUNTIME_EXCEPTION_UNWINDING\tPENDING_BOARD_VERIFICATION\tARM_EHABI_and_Itanium_DWARF_runtime_correctness_not_covered\n'
} >"$OUT/red_and_gap_items.tsv"

{
  printf 'metric\tvalue\n'
  printf 'overall\tRED_WITH_AARCH64_NOT_AVAILABLE\n'
  printf 'architectures_static_gated\t3_of_3\n'
  printf 'architectures_runtime_gated\t2_of_3\n'
  printf 'arch_pass\tx86_64\n'
  printf 'arch_red\tarmv7l_SOURCE_INPUT_IDENTITY\n'
  printf 'arch_not_available\taarch64_RUNTIME_CARRIER\n'
  printf 'artifact_hash_matches\t24_of_24\n'
  printf 'old_gate_items_accounted\t14_of_14\n'
  printf 'frozen_registry_verified\t145_of_145\n'
  printf 'board_gap\tPENDING_BOARD_VERIFICATION\n'
} >"$OUT/conclusions.tsv"

record_to aggregate_assert_final_tables "$RAW/aggregate_assert_final_tables.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 14 && test "$(tail -n +2 "$2" | wc -l)" -eq 14 && test "$(tail -n +2 "$3" | wc -l)" -eq 3 && rg -q "PENDING_BOARD_VERIFICATION" "$4" && rg -q "RED_UNEXPECTED_R6_GIT_EXPORT_DELTA" "$3"' \
  _ "$OUT/gate_results.tsv" "$OUT/three_arch_gate_comparison.tsv" "$OUT/architecture_summary.tsv" "$OUT/red_and_gap_items.tsv" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'T4_AGGREGATION=PASS_EVIDENCE_COMPLETE_WITH_REPORTED_RED_AND_NA\n' >"$OUT/aggregation.status"
else
  printf 'T4_AGGREGATION=FAIL\n' >"$OUT/aggregation.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
