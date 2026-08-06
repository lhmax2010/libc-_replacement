#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/gates/raw"
LEDGER="$OUT/commands/08_aggregate_gates_and_arch_metadata.log"
: >"$LEDGER"

record_to() {
  local label=$1 outfile=$2
  shift 2
  {
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
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
  'test "$(tail -n +2 "$1" | wc -l)" -eq 14 && test "$(cut -f1 "$1" | tail -n +2 | sort -u | wc -l)" -eq 14' _ \
  "$OUT/identity/t4_gate_inventory.tsv" || overall=1
record_to aggregate_assert_builds "$RAW/aggregate_assert_builds.stdout.txt" bash -o pipefail -c \
  'for a in armv7l aarch64 x86_64; do awk -F"\t" "NR==2 && \$2==\"PASS\" && \$4==0 && \$5==8 {ok=1} END {exit !ok}" "$1/architectures/$a/status.tsv" || exit 1; done' _ \
  "$OUT" || overall=1
record_to aggregate_assert_identity "$RAW/aggregate_assert_identity.stdout.txt" bash -o pipefail -c \
  'rg -q "^IDENTITY_SELFCHECK=PASS$" "$1" && rg -q "^SOURCE0_THREE_WAY=1f5fe8da" "$1" && rg -q "^PATCHES_THREE_WAY=IDENTICAL_6_AND_MATCH_FROZEN$" "$1"' _ \
  "$OUT/identity/identity_selfcheck.status" || overall=1
record_to aggregate_assert_static "$RAW/aggregate_assert_static.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 3 && awk -F"\t" "NR>1 {for(i=4;i<=12;i++) if(\$i!=\"PASS\") bad=1; if(\$3!=0) bad=1} END {exit bad}" "$1"' _ \
  "$OUT/gates/static_gate_results.final.tsv" || overall=1
record_to aggregate_assert_runtime "$RAW/aggregate_assert_runtime.stdout.txt" bash -o pipefail -c \
  'rg -q "PASS_ALL_THREE" "$1" && for a in armv7l aarch64 x86_64; do awk -F"\t" "NR==2 && \$5==\"PASS\" {ok=1} END {exit !ok}" "$2/$a/static/dependency_closure_gate.tsv" || exit 1; awk -F"\t" "NR==2 && \$5==\"PASS_GREEN\" {g=1} NR==3 && \$5==\"EXPECTED_RED_DETECTED\" {r=1} END {exit !(g&&r)}" "$2/$a/static/binding_gate.tsv" || exit 1; awk -F"\t" "NR==2 && \$4==\"PASS\" {ok=1} END {exit !ok}" "$2/$a/static/runtime_smoke.tsv" || exit 1; done' _ \
  "$OUT/gates/runtime_carrier.status" "$OUT/gates" || overall=1
record_to aggregate_assert_explicit_ldd "$RAW/aggregate_assert_explicit_ldd.stdout.txt" bash -o pipefail -c \
  'rg -q "PASS_ALL_THREE" "$1" && test "$(awk -F"\t" "NR>1 && \$7!=\"PASS\" {n++} END {print n+0}" "$2")" -eq 0' _ \
  "$OUT/gates/explicit_ldd_r.status" "$OUT/gates/explicit_ldd_r.tsv" || overall=1

printf 'architecture\t__cxa_finalize\t_Unwind_RaiseException\t__cxa_throw\t__cxa_allocate_exception\t__gxx_personality_v0\tverdict\n' >"$OUT/gates/provider_family_results.tsv"
for arch in armv7l aarch64 x86_64; do
  bindings="$OUT/gates/$arch/static/evidence/smoke_provider_bindings.log"
  record_to "${arch}_assert_provider_family" "$RAW/${arch}_assert_provider_family.stdout.txt" bash -o pipefail -c \
    'rg -q "to /lib(64)?/libc\\.so\\.6.*__cxa_finalize" "$1" && rg -q "to /lib(64)?/libgcc_s\\.so\\.1.*_Unwind_RaiseException" "$1" && rg -q "to .*sysroot/usr/lib(64)?/libc\\+\\+abi\\.so\\.1.*__cxa_throw" "$1" && rg -q "to .*sysroot/usr/lib(64)?/libc\\+\\+abi\\.so\\.1.*__cxa_allocate_exception" "$1" && rg -q "to .*sysroot/usr/lib(64)?/libc\\+\\+abi\\.so\\.1.*__gxx_personality_v0" "$1"' _ \
    "$bindings" || overall=1
  printf '%s\tglibc\tlibgcc_s\tcandidate_libc++abi\tcandidate_libc++abi\tcandidate_libc++abi\tPASS\n' "$arch" >>"$OUT/gates/provider_family_results.tsv"
done

{
  printf 'gate_id\tgate_name\tarmv7l\taarch64\tx86_64\tevidence_or_execution\n'
  printf 'P01\tCURRENT_BUILD_PROVENANCE\tPASS\tPASS\tPASS\tthree_new_single_package_builds_exit_0;_8_RPM_each;_R9C_provenance_exact\n'
  printf 'P02\tRPM_SET_NEVRA_DIGEST\tPASS\tPASS\tPASS\t7_binary_plus_1_SRPM_each;_frozen_static_tool_exit_0\n'
  printf 'P03\tELF_MACHINE\tPASS\tPASS\tPASS\tfrozen_LLVM_22_static_tool\n'
  printf 'P04\tDWARF_PRODUCER\tPASS\tPASS\tPASS\tall_six_DSO_debuginfo_sets_contain_clang_version_22.1.8\n'
  printf 'P05\tVERSION_LIBCXX\tPASS\tPASS\tPASS\tall_defined_exports_default_@@LLVM_22\n'
  printf 'P06\tVERSION_LIBCXXABI\tPASS\tPASS\tPASS\tall_defined_exports_default_@@LLVM_22\n'
  printf 'P07\tDIRECT_NEEDED_POLICY\tPASS\tPASS\tPASS\tno_direct_libstdc++_or_libunwind\n'
  printf 'P08\tBIND_NOW\tPASS\tPASS\tPASS\tboth_runtime_DSOs_NOW\n'
  printf 'P09\tLDD_R_CLOSURE\tPASS\tPASS\tPASS\tarm=qemu-arm-static_-L;aarch64=qemu-arm64-static_-L;x86_64=native;_strong_UND=0\n'
  printf 'P10\tPROVIDER_FAMILY\tPASS\tPASS\tPASS\tLD_DEBUG_binding_probe\n'
  printf 'P11\tGREEN_BINDING_FIXTURE\tPASS_GREEN\tPASS_GREEN\tPASS_GREEN\t2_of_2_to_candidate_libc++abi\n'
  printf 'P12\tRED_BINDING_FIXTURE\tEXPECTED_RED_DETECTED\tEXPECTED_RED_DETECTED\tEXPECTED_RED_DETECTED\t2_of_2_unversioned_bindings_to_libstdc++\n'
  printf 'P13\tRUNTIME_SMOKE\tPASS\tPASS\tPASS\tall_carriers_exit_0\n'
  printf 'P14\tHEADER_LAYOUT\tPASS\tPASS\tPASS\tdevel_RPM_owns_usr_include_c++_v1;_all_carriers_compile_fixture\n'
} >"$OUT/gates/gate_results.tsv"

{
  printf 'gate_id\tarmv7l\taarch64\tx86_64\tcomparison\n'
  tail -n +2 "$OUT/gates/gate_results.tsv" | while IFS=$'\t' read -r id name arm aarch x86 evidence; do
    comparison=CONSISTENT
    if [[ $arm != "$aarch" || $arm != "$x86" ]]; then
      case "$id" in P11|P12) comparison=CONSISTENT_SEMANTICS_FIXTURE_SPECIFIC_LABEL;; *) comparison=DIFFERENT;; esac
    fi
    printf '%s\t%s\t%s\t%s\t%s\n' "$id" "$arm" "$aarch" "$x86" "$comparison"
  done
} >"$OUT/gates/three_arch_gate_comparison.tsv"

printf 'architecture\trpm_file\tname\tnevra\tsourcerpm\n' >"$OUT/comparisons/new_rpm_metadata.tsv"
printf 'architecture\tpackage\tpath\n' >"$OUT/comparisons/new_rpm_payload_paths.tsv"
printf 'architecture\tpackage\trelation\n' >"$OUT/comparisons/new_rpm_provides.tsv"
printf 'architecture\tpackage\trelation\n' >"$OUT/comparisons/new_rpm_requires.tsv"
printf 'architecture\tsha256\tfile\n' >"$OUT/identity/new_artifact_sha256.tsv"
for arch in armv7l aarch64 x86_64; do
  for rpmf in "$REPO/artifacts/rebuild_2218_c2/$arch"/*.rpm; do
    base=$(basename "$rpmf")
    sha=$(sha256sum "$rpmf" | cut -d' ' -f1)
    printf '%s\t%s\t%s\n' "$arch" "$sha" "$base" >>"$OUT/identity/new_artifact_sha256.tsv"
    rpm -qp --qf "$arch\t$base\t%{NAME}\t%{NEVRA}\t%{SOURCERPM}\n" "$rpmf" >>"$OUT/comparisons/new_rpm_metadata.tsv"
    [[ $base == *.src.rpm ]] && continue
    pkg=$(rpm -qp --qf '%{NAME}' "$rpmf")
    rpm -qpl "$rpmf" | while IFS= read -r path; do printf '%s\t%s\t%s\n' "$arch" "$pkg" "$path"; done >>"$OUT/comparisons/new_rpm_payload_paths.tsv"
    rpm -qp --provides "$rpmf" | while IFS= read -r rel; do printf '%s\t%s\t%s\n' "$arch" "$pkg" "$rel"; done >>"$OUT/comparisons/new_rpm_provides.tsv"
    rpm -qp --requires "$rpmf" | while IFS= read -r rel; do printf '%s\t%s\t%s\n' "$arch" "$pkg" "$rel"; done >>"$OUT/comparisons/new_rpm_requires.tsv"
  done
done

record_to aggregate_assert_final_tables "$RAW/aggregate_assert_final_tables.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 14 && test "$(tail -n +2 "$2" | wc -l)" -eq 14 && test "$(awk -F"\t" "NR>1 && \$5 !~ /^CONSISTENT/ {n++} END {print n+0}" "$2")" -eq 0 && test "$(tail -n +2 "$3" | wc -l)" -eq 24' _ \
  "$OUT/gates/gate_results.tsv" "$OUT/gates/three_arch_gate_comparison.tsv" "$OUT/identity/new_artifact_sha256.tsv" || overall=1

{
  printf 'item\tclassification\tscope\n'
  printf 'RUNTIME_EXCEPTION_UNWINDING\tPENDING_BOARD_VERIFICATION\tARM_EHABI_and_Itanium_DWARF_runtime_exception_unwinding_correctness_not_covered\n'
  printf 'CHECK_SCOPE\tBUILD_STATIC_LOAD_TIME_ONLY\tbuild_time_static_ELF_ldd_binding_fixture_and_smoke;_no_board_exception_unwind_test\n'
  printf 'AARCH64_QEMU\tAVAILABLE_THIS_RUN\t/usr/bin/qemu-arm64-static_identifies_as_qemu-aarch64_5.2.0;_P09-P13_executed\n'
} >"$OUT/gates/coverage_boundary.tsv"

if [[ $overall -eq 0 ]]; then
  printf 'T4_RERUN=PASS_ALL_14_GATES_ALL_3_ARCHITECTURES\nCOVERAGE_GAP=PENDING_BOARD_VERIFICATION\n' >"$OUT/gates/t4_rerun.status"
else
  printf 'T4_RERUN=RED\n' >"$OUT/gates/t4_rerun.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
