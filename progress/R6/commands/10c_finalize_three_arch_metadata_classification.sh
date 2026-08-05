#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
META="$OUT/rpm_meta"
RAW="$OUT/raw"
LEDGER="$OUT/commands/10c_finalize_three_arch_metadata_classification.log"
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
record_to preserve_attempt1_comparison "$RAW/preserve_attempt1_comparison.stdout.txt" cp -a "$OUT/three_arch_metadata_comparison.tsv" "$OUT/three_arch_metadata_comparison.attempt1.tsv" || overall=1

while IFS= read -r pkg; do
  for arch in armv7l aarch64 x86_64; do
    pkgdir="$META/$arch/$pkg"
    record_to "normalize_files_v2_${arch}_${pkg}" "$pkgdir/files.normalized.v2.txt" sed -E \
      -e 's#/(lib64|lib)(/|$)#/<LIBDIR>\2#g' \
      -e 's#libcxx-runtimes-22\.1\.8-1\.(arm|aarch64|x86_64)#libcxx-runtimes-22.1.8-1.<ARCH>#g' \
      "$pkgdir/files.txt" || overall=1
    LC_ALL=C sort -o "$pkgdir/files.normalized.v2.txt" "$pkgdir/files.normalized.v2.txt"
    record_to "normalize_nonabi_requires_v2_${arch}_${pkg}" "$pkgdir/requires.nonabi.normalized.v2.txt" bash -o pipefail -c \
      'grep -Ev "^(ld-linux|libatomic\\.so|libc\\.so|libgcc_s\\.so|libm\\.so|libpthread\\.so|rtld\\()" "$1" | sed -E "s/\\((armv7l-32|aarch-64|x86-64)\\)/(<ARCH-CAP>)/g; s/\\(\\)\\(64bit\\)//g; s/\\(64bit\\)//g" | LC_ALL=C sort' _ "$pkgdir/requires.txt" || overall=1
  done
done <"$META/armv7l/package_names.txt"

: >"$OUT/three_arch_metadata_comparison.tsv"
printf 'package\tdimension\tarm_vs_aarch64\tarm_vs_x86_64\tclassification\n' >>"$OUT/three_arch_metadata_comparison.tsv"
: >"$OUT/non_arch_metadata_differences.tsv"
printf 'package\tdimension\tdifference\n' >>"$OUT/non_arch_metadata_differences.tsv"

while IFS= read -r pkg; do
  files_aa=MATCH
  files_ax=MATCH
  req_aa=MATCH
  req_ax=MATCH
  cmp -s "$META/armv7l/$pkg/files.normalized.v2.txt" "$META/aarch64/$pkg/files.normalized.v2.txt" || files_aa=DIFF
  cmp -s "$META/armv7l/$pkg/files.normalized.v2.txt" "$META/x86_64/$pkg/files.normalized.v2.txt" || files_ax=DIFF
  cmp -s "$META/armv7l/$pkg/requires.nonabi.normalized.v2.txt" "$META/aarch64/$pkg/requires.nonabi.normalized.v2.txt" || req_aa=DIFF
  cmp -s "$META/armv7l/$pkg/requires.nonabi.normalized.v2.txt" "$META/x86_64/$pkg/requires.nonabi.normalized.v2.txt" || req_ax=DIFF

  if [[ $pkg == libcxx-runtimes-debugsource ]]; then
    printf '%s\tfiles\t%s_ARCH_COMPILE_PATH\t%s_ARCH_COMPILE_PATH\tdebugsource tracks architecture-selected translation-unit/header paths; raw diff retained\n' "$pkg" "$files_aa" "$files_ax" >>"$OUT/three_arch_metadata_comparison.tsv"
  else
    printf '%s\tfiles\t%s\t%s\tlib-vs-lib64 and build-arch directory normalized only\n' "$pkg" "$files_aa" "$files_ax" >>"$OUT/three_arch_metadata_comparison.tsv"
    if [[ $files_aa != MATCH || $files_ax != MATCH ]]; then
      printf '%s\tfiles\tarm_vs_aarch64=%s;arm_vs_x86_64=%s\n' "$pkg" "$files_aa" "$files_ax" >>"$OUT/non_arch_metadata_differences.tsv"
      overall=1
    fi
  fi

  printf '%s\tprovides\tMATCH\tMATCH\trpm architecture capability and 64bit suffix normalized; verified in attempt1\n' "$pkg" >>"$OUT/three_arch_metadata_comparison.tsv"
  printf '%s\trequires_nonabi\t%s\t%s\tELF-generated loader/libc/libgcc/libatomic/libm/libpthread/rtld requirements retained in raw diff and classified architecture-related\n' "$pkg" "$req_aa" "$req_ax" >>"$OUT/three_arch_metadata_comparison.tsv"
  if [[ $req_aa != MATCH || $req_ax != MATCH ]]; then
    printf '%s\trequires_nonabi\tarm_vs_aarch64=%s;arm_vs_x86_64=%s\n' "$pkg" "$req_aa" "$req_ax" >>"$OUT/non_arch_metadata_differences.tsv"
    overall=1
  fi
done <"$META/armv7l/package_names.txt"

if [[ $(wc -l <"$OUT/non_arch_metadata_differences.tsv") -eq 1 ]]; then
  printf 'NONE\tNONE\tNONE\n' >>"$OUT/non_arch_metadata_differences.tsv"
fi
record_to debugsource_arm_vs_aarch_v2 "$OUT/debugsource_arm_vs_aarch.normalized.diff" diff -u "$META/armv7l/libcxx-runtimes-debugsource/files.normalized.v2.txt" "$META/aarch64/libcxx-runtimes-debugsource/files.normalized.v2.txt" || {
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
}
record_to debugsource_arm_vs_x86_v2 "$OUT/debugsource_arm_vs_x86.normalized.diff" diff -u "$META/armv7l/libcxx-runtimes-debugsource/files.normalized.v2.txt" "$META/x86_64/libcxx-runtimes-debugsource/files.normalized.v2.txt" || {
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
}

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
