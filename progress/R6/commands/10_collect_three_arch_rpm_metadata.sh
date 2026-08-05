#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
META="$OUT/rpm_meta"
RAW="$OUT/raw"
LEDGER="$OUT/commands/10_collect_three_arch_rpm_metadata.log"
mkdir -p "$META"
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
: >"$OUT/three_arch_package_composition.tsv"
printf 'arch\tname\tepoch\tversion\trelease\trpm_arch\tfilename\n' >>"$OUT/three_arch_package_composition.tsv"

for arch in armv7l aarch64 x86_64; do
  arch_dir="$REPO/artifacts/rebuild_2218/$arch"
  mkdir -p "$META/$arch"
  while IFS= read -r rpmfile; do
    filename=${rpmfile##*/}
    name=$(rpm -qp --qf '%{NAME}' "$rpmfile")
    pkgdir="$META/$arch/$name"
    mkdir -p "$pkgdir"
    record_to "${arch}_${name}_identity" "$pkgdir/identity.tsv" rpm -qp --qf '%{NAME}\t%{EPOCHNUM}\t%{VERSION}\t%{RELEASE}\t%{ARCH}\n' "$rpmfile" || overall=1
    record_to "${arch}_${name}_files" "$pkgdir/files.txt" rpm -qpl "$rpmfile" || overall=1
    record_to "${arch}_${name}_provides" "$pkgdir/provides.txt" rpm -qp --provides "$rpmfile" || overall=1
    record_to "${arch}_${name}_requires" "$pkgdir/requires.txt" rpm -qp --requires "$rpmfile" || overall=1
    printf '%s\t' "$arch" >>"$OUT/three_arch_package_composition.tsv"
    tr '\n' '\t' <"$pkgdir/identity.tsv" | sed 's/\t$//' >>"$OUT/three_arch_package_composition.tsv"
    printf '\t%s\n' "$filename" >>"$OUT/three_arch_package_composition.tsv"
  done < <(find "$arch_dir" -maxdepth 1 -type f -name '*.rpm' ! -name '*.src.rpm' -print | LC_ALL=C sort)
  record_to "${arch}_package_names" "$META/$arch/package_names.txt" bash -o pipefail -c 'cut -f1 "$1" | tail -n +2 | sort -u' _ "$OUT/three_arch_package_composition.tsv" || overall=1
done

record_to package_names_arm_vs_aarch "$OUT/package_names_arm_vs_aarch.diff" diff -u "$META/armv7l/package_names.txt" "$META/aarch64/package_names.txt" || {
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
}
record_to package_names_arm_vs_x86 "$OUT/package_names_arm_vs_x86.diff" diff -u "$META/armv7l/package_names.txt" "$META/x86_64/package_names.txt" || {
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
}
record_to assert_package_name_sets_identical bash -o pipefail -c 'cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ "$META/armv7l/package_names.txt" "$META/aarch64/package_names.txt" "$META/x86_64/package_names.txt" || overall=1

for arch in armv7l aarch64 x86_64; do
  rpmfile="$REPO/artifacts/rebuild_2218/$arch/libc++-22.1.8-1.$arch.rpm"
  record_to "${arch}_source_provenance" "$META/$arch/SOURCE_PROVENANCE" bash -o pipefail -c 'rpm2cpio "$1" | cpio -i --quiet --to-stdout ./usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE' _ "$rpmfile" || overall=1
done
record_to provenance_sha256 "$OUT/three_arch_source_provenance_sha256.tsv" sha256sum "$META/armv7l/SOURCE_PROVENANCE" "$META/aarch64/SOURCE_PROVENANCE" "$META/x86_64/SOURCE_PROVENANCE" || overall=1
record_to assert_provenance_identical bash -o pipefail -c 'cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ "$META/armv7l/SOURCE_PROVENANCE" "$META/aarch64/SOURCE_PROVENANCE" "$META/x86_64/SOURCE_PROVENANCE" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
