#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
META="$OUT/rpm_meta"
RAW="$OUT/raw"
LEDGER="$OUT/commands/10b_compare_three_arch_rpm_metadata.log"
: >"$LEDGER"
: >"$OUT/three_arch_metadata_comparison.tsv"
: >"$OUT/three_arch_raw_metadata_differences.diff"
printf 'package\tdimension\tarm_vs_aarch64\tarm_vs_x86_64\tcomparison_scope\n' >>"$OUT/three_arch_metadata_comparison.tsv"

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
while IFS= read -r pkg; do
  for arch in armv7l aarch64 x86_64; do
    pkgdir="$META/$arch/$pkg"
    record_to "normalize_files_${arch}_${pkg}" "$pkgdir/files.normalized.txt" sed -E \
      -e 's#/usr/lib64#/usr/<LIBDIR>#g' \
      -e 's#/usr/lib/#/usr/<LIBDIR>/#g' \
      -e 's#libcxx-runtimes-22\.1\.8-1\.(arm|aarch64|x86_64)#libcxx-runtimes-22.1.8-1.<ARCH>#g' \
      "$pkgdir/files.txt" || overall=1
    LC_ALL=C sort -o "$pkgdir/files.normalized.txt" "$pkgdir/files.normalized.txt"

    record_to "normalize_provides_${arch}_${pkg}" "$pkgdir/provides.normalized.txt" sed -E \
      -e 's/\((armv7l-32|aarch-64|x86-64)\)/(<ARCH-CAP>)/g' \
      -e 's/\(\)\(64bit\)//g' \
      -e 's/\(64bit\)//g' \
      "$pkgdir/provides.txt" || overall=1
    LC_ALL=C sort -o "$pkgdir/provides.normalized.txt" "$pkgdir/provides.normalized.txt"

    record_to "normalize_nonabi_requires_${arch}_${pkg}" "$pkgdir/requires.nonabi.normalized.txt" bash -o pipefail -c \
      'grep -Ev "^(ld-linux|libatomic\\.so|libc\\.so|libgcc_s\\.so|libm\\.so|libpthread\\.so|rtld\\()" "$1" | sed -E "s/\\((armv7l-32|aarch-64|x86-64)\\)/(<ARCH-CAP>)/g; s/\\(64bit\\)//g" | LC_ALL=C sort' _ "$pkgdir/requires.txt" || overall=1
  done

  files_aa=MATCH
  files_ax=MATCH
  prov_aa=MATCH
  prov_ax=MATCH
  req_aa=MATCH
  req_ax=MATCH
  cmp -s "$META/armv7l/$pkg/files.normalized.txt" "$META/aarch64/$pkg/files.normalized.txt" || files_aa=DIFF
  cmp -s "$META/armv7l/$pkg/files.normalized.txt" "$META/x86_64/$pkg/files.normalized.txt" || files_ax=DIFF
  cmp -s "$META/armv7l/$pkg/provides.normalized.txt" "$META/aarch64/$pkg/provides.normalized.txt" || prov_aa=DIFF
  cmp -s "$META/armv7l/$pkg/provides.normalized.txt" "$META/x86_64/$pkg/provides.normalized.txt" || prov_ax=DIFF
  cmp -s "$META/armv7l/$pkg/requires.nonabi.normalized.txt" "$META/aarch64/$pkg/requires.nonabi.normalized.txt" || req_aa=DIFF
  cmp -s "$META/armv7l/$pkg/requires.nonabi.normalized.txt" "$META/x86_64/$pkg/requires.nonabi.normalized.txt" || req_ax=DIFF
  printf '%s\tfiles\t%s\t%s\tlibdir_and_debug_build_arch_normalized\n' "$pkg" "$files_aa" "$files_ax" >>"$OUT/three_arch_metadata_comparison.tsv"
  printf '%s\tprovides\t%s\t%s\trpm_arch_capability_and_64bit_suffix_normalized\n' "$pkg" "$prov_aa" "$prov_ax" >>"$OUT/three_arch_metadata_comparison.tsv"
  printf '%s\trequires_nonabi\t%s\t%s\tELF_loader_libc_libgcc_libatomic_libm_libpthread_rtld_requirements_excluded_as_arch_generated\n' "$pkg" "$req_aa" "$req_ax" >>"$OUT/three_arch_metadata_comparison.tsv"
  if [[ $files_aa == DIFF || $files_ax == DIFF || $prov_aa == DIFF || $prov_ax == DIFF || $req_aa == DIFF || $req_ax == DIFF ]]; then
    overall=1
  fi

  {
    printf '### PACKAGE=%s ARM_VS_AARCH64 FILES\n' "$pkg"
    diff -u "$META/armv7l/$pkg/files.txt" "$META/aarch64/$pkg/files.txt" || true
    printf '### PACKAGE=%s ARM_VS_X86_64 FILES\n' "$pkg"
    diff -u "$META/armv7l/$pkg/files.txt" "$META/x86_64/$pkg/files.txt" || true
    printf '### PACKAGE=%s ARM_VS_AARCH64 PROVIDES\n' "$pkg"
    diff -u "$META/armv7l/$pkg/provides.txt" "$META/aarch64/$pkg/provides.txt" || true
    printf '### PACKAGE=%s ARM_VS_X86_64 PROVIDES\n' "$pkg"
    diff -u "$META/armv7l/$pkg/provides.txt" "$META/x86_64/$pkg/provides.txt" || true
    printf '### PACKAGE=%s ARM_VS_AARCH64 REQUIRES\n' "$pkg"
    diff -u "$META/armv7l/$pkg/requires.txt" "$META/aarch64/$pkg/requires.txt" || true
    printf '### PACKAGE=%s ARM_VS_X86_64 REQUIRES\n' "$pkg"
    diff -u "$META/armv7l/$pkg/requires.txt" "$META/x86_64/$pkg/requires.txt" || true
  } >>"$OUT/three_arch_raw_metadata_differences.diff"
done <"$META/armv7l/package_names.txt"

record_to assert_all_normalized_metadata_match "$RAW/assert_all_normalized_metadata_match.stdout.txt" awk -F '\t' 'NR > 1 && ($3 != "MATCH" || $4 != "MATCH") {print; bad=1} END {exit bad}' "$OUT/three_arch_metadata_comparison.tsv" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
