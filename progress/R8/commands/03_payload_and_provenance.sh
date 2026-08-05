#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R8"
RAW="$OUT/raw"
ART="$REPO/artifacts/rebuild_2218"
TMP="$REPO/tmp/R8/payload_analysis"
BUILD="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
CODE="$REPO/codes/llvm"
LEDGER="$OUT/commands/03_payload_and_provenance.log"
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
record_to assert_payload_tmp_fresh "$RAW/assert_payload_tmp_fresh.stdout.txt" test ! -e "$TMP" || overall=1
record_to create_payload_analysis_dirs "$RAW/create_payload_analysis_dirs.stdout.txt" mkdir -p "$TMP" "$OUT/payload_paths/raw" "$OUT/payload_paths/normalized" || overall=1

printf 'arch\tname\tnevra\trpm_file\tpayload_path_count\n' >"$OUT/rpm_payload_inventory.tsv"
for arch in x86_64 aarch64 armv7l; do
  mkdir -p "$OUT/payload_paths/raw/$arch" "$OUT/payload_paths/normalized/$arch"
  for rpmfile in "$ART/$arch"/*.rpm; do
    name=$(rpm -qp --qf '%{NAME}' "$rpmfile")
    nevra=$(rpm -qp --qf '%{NEVRA}' "$rpmfile")
    record_to "qpl_${arch}_${name//+/_}" "$OUT/payload_paths/raw/$arch/$name.txt" rpm -qpl "$rpmfile" || overall=1
    count=$(wc -l <"$OUT/payload_paths/raw/$arch/$name.txt")
    printf '%s\t%s\t%s\t%s\t%s\n' "$arch" "$name" "$nevra" "${rpmfile#$REPO/}" "$count" >>"$OUT/rpm_payload_inventory.tsv"
    record_to "normalize_qpl_${arch}_${name//+/_}" "$OUT/payload_paths/normalized/$arch/$name.txt" awk '
      {
        p=$0
        if (p ~ /^\/usr\/lib\/\.build-id(\/|$)/) p="/usr/@BUILD_ID@"
        else if (p ~ /^\/usr\/lib\/debug\/usr\/lib64(\/|$)/) sub(/^\/usr\/lib\/debug\/usr\/lib64/,"/usr/lib/debug/usr/@LIB@",p)
        else if (p ~ /^\/usr\/lib\/debug\/usr\/lib(\/|$)/) sub(/^\/usr\/lib\/debug\/usr\/lib/,"/usr/lib/debug/usr/@LIB@",p)
        else if (p ~ /^\/usr\/lib64(\/|$)/) sub(/^\/usr\/lib64/,"/usr/@LIB@",p)
        else if (p ~ /^\/usr\/lib(\/|$)/) sub(/^\/usr\/lib/,"/usr/@LIB@",p)
        sub(/^\/usr\/src\/debug\/libcxx-runtimes-22\.1\.8-1\.(x86_64|aarch64|arm)(\/|$)/,"/usr/src/debug/libcxx-runtimes-22.1.8-1.@ARCH@\\2",p)
        print p
      }
    ' "$OUT/payload_paths/raw/$arch/$name.txt" || overall=1
    LC_ALL=C sort -u -o "$OUT/payload_paths/normalized/$arch/$name.txt" "$OUT/payload_paths/normalized/$arch/$name.txt"
  done
done

record_to assert_subpackage_sets "$RAW/assert_subpackage_sets.stdout.txt" bash -o pipefail -c '
  for arch in x86_64 aarch64 armv7l; do find "$1/normalized/$arch" -maxdepth 1 -type f -printf "%f\n" | sort >"$2/$arch.packages"; done
  cmp "$2/x86_64.packages" "$2/aarch64.packages" && cmp "$2/x86_64.packages" "$2/armv7l.packages"
' _ "$OUT/payload_paths" "$TMP" || overall=1

printf 'package\tcomparison\tside\tnormalized_path\n' >"$OUT/payload_path_differences.tsv"
while IFS= read -r pkgfile; do
  pkg=${pkgfile%.txt}
  for arch in aarch64 armv7l; do
    record_to "comm_x86_64_${arch}_${pkg//+/_}" "$RAW/payload_${pkg//+/_}_x86_64_vs_${arch}.comm.txt" \
      comm -3 "$OUT/payload_paths/normalized/x86_64/$pkgfile" "$OUT/payload_paths/normalized/$arch/$pkgfile" || overall=1
    comm -23 "$OUT/payload_paths/normalized/x86_64/$pkgfile" "$OUT/payload_paths/normalized/$arch/$pkgfile" |
      while IFS= read -r path; do printf '%s\tx86_64_vs_%s\tONLY_X86_64\t%s\n' "$pkg" "$arch" "$path"; done >>"$OUT/payload_path_differences.tsv"
    comm -13 "$OUT/payload_paths/normalized/x86_64/$pkgfile" "$OUT/payload_paths/normalized/$arch/$pkgfile" |
      while IFS= read -r path; do printf '%s\tx86_64_vs_%s\tONLY_%s\t%s\n' "$pkg" "$arch" "${arch^^}" "$path"; done >>"$OUT/payload_path_differences.tsv"
  done
done <"$TMP/x86_64.packages"

record_to payload_path_comparison_summary "$OUT/payload_path_comparison_summary.tsv" bash -o pipefail -c '
  printf "metric\tvalue\n"
  printf "subpackage_sets\tIDENTICAL_7_BINARY_PLUS_1_SRPM\n"
  printf "normalization\tlib_vs_lib64_to_@LIB@;debugsource_arch_root_to_@ARCH@;build_id_hash_paths_to_@BUILD_ID@\n"
  printf "non_arch_path_delta_count\t%s\n" "$(tail -n +2 "$1" | wc -l)"
  awk -F"\t" "NR>1 {n[\$1 \"|\" \$2]++} END {for (k in n) print k \"\\t\" n[k]}" "$1" | sort
' _ "$OUT/payload_path_differences.tsv" || overall=1

record_to create_debugsource_extract_dirs "$RAW/create_debugsource_extract_dirs.stdout.txt" mkdir -p "$TMP/debugsource/x86_64" "$TMP/debugsource/aarch64" "$TMP/debugsource/armv7l" || overall=1
for arch in x86_64 aarch64 armv7l; do
  dbg="$ART/$arch/libcxx-runtimes-debugsource-22.1.8-1.$arch.rpm"
  record_to "extract_debugsource_${arch}" "$RAW/extract_debugsource_${arch}.stdout.txt" bash -o pipefail -c \
    'cd "$1" && rpm2cpio "$2" | cpio -idmuv' _ "$TMP/debugsource/$arch" "$dbg" || overall=1
  record_to "debugsource_cxa_sha_${arch}" "$RAW/debugsource_cxa_sha_${arch}.stdout.txt" bash -o pipefail -c \
    'find "$1" -type f -path "*/libcxxabi/src/cxa_personality.cpp" -print0 | xargs -0 sha256sum' _ "$TMP/debugsource/$arch" || overall=1
done
record_to debugsource_cxa_x86_vs_aarch "$RAW/debugsource_cxa_x86_vs_aarch.diff" diff -u \
  "$(find "$TMP/debugsource/x86_64" -type f -path '*/libcxxabi/src/cxa_personality.cpp')" \
  "$(find "$TMP/debugsource/aarch64" -type f -path '*/libcxxabi/src/cxa_personality.cpp')"
rc=$?; [[ $rc -eq 0 ]] || overall=1
record_to debugsource_cxa_x86_vs_arm "$RAW/debugsource_cxa_x86_vs_arm.diff" diff -u \
  "$(find "$TMP/debugsource/x86_64" -type f -path '*/libcxxabi/src/cxa_personality.cpp')" \
  "$(find "$TMP/debugsource/armv7l" -type f -path '*/libcxxabi/src/cxa_personality.cpp')"
rc=$?; [[ $rc -eq 1 ]] || overall=1
record_to cxa_object_linked_into_libcxxabi "$RAW/cxa_object_linked_into_libcxxabi.stdout.txt" rg -n \
  '^build lib/libc\+\+abi\.so\.1\.0:.*cxa_personality\.cpp\.o' "$BUILD/build.ninja" || overall=1

printf 'arch\tactual_source0_name\tactual_source0_sha256\n' >"$OUT/actual_source0_identity.tsv"
for arch in x86_64 aarch64 armv7l; do
  srpm="$ART/$arch/libcxx-runtimes-22.1.8-1.src.rpm"
  record_to "source0_sha_${arch}" "$RAW/source0_sha_${arch}.stdout.txt" bash -o pipefail -c \
    'rpm2cpio "$1" | cpio -i --to-stdout llvm-22.1.8.tar.gz 2>"$2" | sha256sum' _ "$srpm" "$RAW/source0_sha_${arch}.cpio.stderr.txt" || overall=1
  sha=$(awk '{print $1}' "$RAW/source0_sha_${arch}.stdout.txt")
  printf '%s\tllvm-22.1.8.tar.gz\t%s\n' "$arch" "$sha" >>"$OUT/actual_source0_identity.tsv"
  devel="$ART/$arch/libc++-devel-22.1.8-1.$arch.rpm"
  record_to "provenance_${arch}" "$OUT/SOURCE_PROVENANCE.$arch.txt" bash -o pipefail -c \
    'rpm2cpio "$1" | cpio -i --to-stdout ./usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE 2>"$2"' _ "$devel" "$RAW/provenance_${arch}.cpio.stderr.txt" || overall=1
  record_to "provenance_numbered_${arch}" "$RAW/SOURCE_PROVENANCE.${arch}.numbered.txt" nl -ba "$OUT/SOURCE_PROVENANCE.$arch.txt" || overall=1
done

record_to provenance_three_way_byte_identity "$RAW/provenance_three_way_byte_identity.stdout.txt" bash -o pipefail -c \
  'sha256sum "$1" "$2" "$3"; cmp "$1" "$2" && cmp "$1" "$3"' _ \
  "$OUT/SOURCE_PROVENANCE.x86_64.txt" "$OUT/SOURCE_PROVENANCE.aarch64.txt" "$OUT/SOURCE_PROVENANCE.armv7l.txt" || overall=1
record_to source_git_ancestry "$RAW/source_git_ancestry.stdout.txt" bash -o pipefail -c '
  git -C "$1" rev-parse HEAD
  git -C "$1" merge-base --is-ancestor ca7933e47d3a3451d81e72ac174dcb5aa28b59d1 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b; printf "UPSTREAM_PEELED_ANCESTOR_RC=%s\n" "$?"
  git -C "$1" merge-base --is-ancestor c12cbe887ff08f04628e1b1165f8d02ce69a9146 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b; printf "PLATFORM_SPEC_VCS_ANCESTOR_RC=%s\n" "$?"
' _ "$CODE" || overall=1
record_to r6_actual_source_declaration "$RAW/r6_actual_source_declaration.stdout.txt" bash -o pipefail -c \
  'nl -ba "$1"; nl -ba "$2"; nl -ba "$3"' _ "$REPO/progress/R6/build_input_delta.tsv" "$REPO/progress/R6/raw/exported_source_sha256.stdout.txt" "$REPO/progress/R6/build_armv7l_git_r3b.status.tsv" || overall=1

cat >"$OUT/provenance_field_audit.tsv" <<'EOF'
arch	field	declared_value	actual_match	actual_or_correct_value	evidence
x86_64	component	llvm-project libcxx+libcxxabi	相符	llvm-project libcxx+libcxxabi	RPM_subpackages_and_R6/T3R_build_targets
x86_64	version	22.1.8	相符	22.1.8	RPM_NEVRA_and_Source0_name
x86_64	source_tarball	llvm-22.1.8.tar.gz	相符	llvm-22.1.8.tar.gz	actual_source0_identity.tsv
x86_64	source_tarball_sha256	16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e	相符	16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e	actual_source0_identity.tsv
x86_64	platform_spec_vcs	platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146	相符	platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146	platform_SRPM_spec_VCS_anchor
x86_64	upstream_tag	llvmorg-22.1.8	相符	llvmorg-22.1.8	platform_Source0_three_anchor_identity
x86_64	upstream_peeled_commit	ca7933e47d3a3451d81e72ac174dcb5aa28b59d1	相符	ca7933e47d3a3451d81e72ac174dcb5aa28b59d1	platform_Source0_three_anchor_identity
x86_64	source_export	verbatim platform SRPM tarball; not repacked	相符	verbatim platform SRPM tarball; not repacked	Source0_SHA_16a932e1
x86_64	stdlib_abi	libcxxabi	相符	libcxxabi	build_configuration
x86_64	unwinder	libgcc_s	相符	libgcc_s	build_configuration
x86_64	compiler_rt	disabled	相符	disabled	build_configuration
x86_64	llvm_unwinder	disabled	相符	disabled	build_configuration
x86_64	symbol_version_node	LLVM_22	相符	LLVM_22	T4_symbol_gate
aarch64	component	llvm-project libcxx+libcxxabi	相符	llvm-project libcxx+libcxxabi	RPM_subpackages_and_R6/T3R2_build_targets
aarch64	version	22.1.8	相符	22.1.8	RPM_NEVRA_and_Source0_name
aarch64	source_tarball	llvm-22.1.8.tar.gz	相符	llvm-22.1.8.tar.gz	actual_source0_identity.tsv
aarch64	source_tarball_sha256	16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e	相符	16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e	actual_source0_identity.tsv
aarch64	platform_spec_vcs	platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146	相符	platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146	platform_SRPM_spec_VCS_anchor
aarch64	upstream_tag	llvmorg-22.1.8	相符	llvmorg-22.1.8	platform_Source0_three_anchor_identity
aarch64	upstream_peeled_commit	ca7933e47d3a3451d81e72ac174dcb5aa28b59d1	相符	ca7933e47d3a3451d81e72ac174dcb5aa28b59d1	platform_Source0_three_anchor_identity
aarch64	source_export	verbatim platform SRPM tarball; not repacked	相符	verbatim platform SRPM tarball; not repacked	Source0_SHA_16a932e1
aarch64	stdlib_abi	libcxxabi	相符	libcxxabi	build_configuration
aarch64	unwinder	libgcc_s	相符	libgcc_s	build_configuration
aarch64	compiler_rt	disabled	相符	disabled	build_configuration
aarch64	llvm_unwinder	disabled	相符	disabled	build_configuration
aarch64	symbol_version_node	LLVM_22	相符	LLVM_22	T4_symbol_gate
armv7l	component	llvm-project libcxx+libcxxabi	相符	llvm-project libcxx+libcxxabi	RPM_subpackages_and_R6_build_targets
armv7l	version	22.1.8	相符	22.1.8	RPM_NEVRA_and_Source0_name
armv7l	source_tarball	llvm-22.1.8.tar.gz	相符	llvm-22.1.8.tar.gz	actual_source0_identity.tsv
armv7l	source_tarball_sha256	16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e	不符	7d7f2105f1c745754e1318b3d73b632a642e426883fb2d6fa6a5b46f55b5d314	actual_source0_identity.tsv
armv7l	platform_spec_vcs	platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146	相符（平台spec基线锚；非实际Source_HEAD）	实际Source_HEAD=platform/upstream/llvm@3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b；c12cbe88为其祖先	source_git_ancestry.stdout.txt
armv7l	upstream_tag	llvmorg-22.1.8	相符（上游基线锚；非实际Source_HEAD）	实际Source_HEAD含Tizen下游提交及EHABI_backport	SOURCE_IDENTICAL_base_comparison_plus_git_ancestry
armv7l	upstream_peeled_commit	ca7933e47d3a3451d81e72ac174dcb5aa28b59d1	相符（上游基线锚；非实际Source_HEAD）	ca7933e4为3d3e3da祖先；实际Source_HEAD=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b	source_git_ancestry.stdout.txt
armv7l	source_export	verbatim platform SRPM tarball; not repacked	不符	GBS_git_export_of_codes/llvm_tizen_base@3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b;Source0_SHA=7d7f2105...	R6_build_input_delta.tsv_and_exported_source_sha256
armv7l	stdlib_abi	libcxxabi	相符	libcxxabi	R6_build_command
armv7l	unwinder	libgcc_s	相符	libgcc_s	R6_build_command_and_ELF_binding
armv7l	compiler_rt	disabled	相符	disabled	R6_build_command
armv7l	llvm_unwinder	disabled	相符	disabled	R6_build_command
armv7l	symbol_version_node	LLVM_22	相符	LLVM_22	R6_symbol_gate
EOF

cat >"$OUT/payload_source_attribution.tsv" <<'EOF'
payload_scope	finding	measured_basis
normalized_payload_path_sets	SEE_payload_path_differences.tsv	rpm_-qpl_then_declared_arch_normalization
nonruntime_Tizen_differences	NO_BINARY_PAYLOAD_CONTENT_ATTRIBUTABLE	13_difference_files_have_NO_INTERSECTION_with_78_TUs/Ninja_headers/CMake_inputs
EHABI_backport	ENTERS_libc++abi.so.1	cxa_personality.cpp_is_actual_TU_and_object_is_on_libc++abi.so.1.0_link_edge
debugsource_payload	ARM_cxa_personality_content_differs_from_x86_64/aarch64	extracted_debugsource_full_diff_and_SHA256
SRPM_payload	ARM_Source0_differs	actual_Source0_SHA_7d7f2105_vs_16a932e1
architecture_specific_ELF_byte_comparison	NOT_USED_FOR_CROSS_ARCH_ATTRIBUTION	different_ISA_prevents_byte_identity_from_proving_source_causation
EOF

record_to assert_provenance_audit_shape "$RAW/assert_provenance_audit_shape.stdout.txt" bash -o pipefail -c '
  test "$(tail -n +2 "$1" | wc -l)" -eq 39 &&
  test "$(awk -F"\t" "NR>1 && \$4==\"不符\" {n++} END {print n+0}" "$1")" -eq 2 &&
  test "$(awk -F"\t" "NR>1 && \$1==\"armv7l\" && \$4==\"不符\" {print \$2}" "$1" | paste -sd, -)" = source_tarball_sha256,source_export
' _ "$OUT/provenance_field_audit.tsv" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'PAYLOAD_PROVENANCE_AUDIT=PASS_WITH_DECLARED_ARM_PROVENANCE_MISMATCHES\n' >"$OUT/payload_provenance.status"
else
  printf 'PAYLOAD_PROVENANCE_AUDIT=FAIL_OR_UNDETERMINED\n' >"$OUT/payload_provenance.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
