#!/usr/bin/env bash
set -u
set -o pipefail
REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R8"
RAW="$OUT/raw"
TMP="$REPO/tmp/R8/payload_analysis"
LEDGER="$OUT/commands/03a_payload_path_normalization_retry.log"
: >"$LEDGER"

record_to() {
  local label=$1 outfile=$2
  shift 2
  { printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"; printf '%q ' "$@"; printf '\n'; } >>"$LEDGER"
  "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

cd "$REPO" || exit 125
overall=0
record_to disclose_normalization_retry "$RAW/disclose_normalization_retry.stdout.txt" bash -c \
  'printf "%s\n" "03_payload_and_provenance.sh 的 awk replacement 将捕获组写成字面量 \\2；原始 rpm -qpl 证据未受影响。本脚本从原始清单重新归一化并覆盖派生对照表。"' || overall=1
for arch in x86_64 aarch64 armv7l; do
  for rawfile in "$OUT/payload_paths/raw/$arch"/*.txt; do
    name=$(basename "$rawfile")
    record_to "renormalize_${arch}_${name%.txt}" "$OUT/payload_paths/normalized/$arch/$name" awk '
      {
        p=$0
        if (p ~ /^\/usr\/lib\/\.build-id(\/|$)/) p="/usr/@BUILD_ID@"
        else if (p ~ /^\/usr\/lib\/debug\/usr\/lib64(\/|$)/) sub(/^\/usr\/lib\/debug\/usr\/lib64/,"/usr/lib/debug/usr/@LIB@",p)
        else if (p ~ /^\/usr\/lib\/debug\/usr\/lib(\/|$)/) sub(/^\/usr\/lib\/debug\/usr\/lib/,"/usr/lib/debug/usr/@LIB@",p)
        else if (p ~ /^\/usr\/lib64(\/|$)/) sub(/^\/usr\/lib64/,"/usr/@LIB@",p)
        else if (p ~ /^\/usr\/lib(\/|$)/) sub(/^\/usr\/lib/,"/usr/@LIB@",p)
        sub(/^\/usr\/src\/debug\/libcxx-runtimes-22\.1\.8-1\.(x86_64|aarch64|arm)\//,"/usr/src/debug/libcxx-runtimes-22.1.8-1.@ARCH@/",p)
        sub(/^\/usr\/src\/debug\/libcxx-runtimes-22\.1\.8-1\.(x86_64|aarch64|arm)$/,"/usr/src/debug/libcxx-runtimes-22.1.8-1.@ARCH@",p)
        print p
      }
    ' "$rawfile" || overall=1
    LC_ALL=C sort -u -o "$OUT/payload_paths/normalized/$arch/$name" "$OUT/payload_paths/normalized/$arch/$name"
  done
done

printf 'package\tcomparison\tside\tnormalized_path\n' >"$OUT/payload_path_differences.tsv"
while IFS= read -r pkgfile; do
  pkg=${pkgfile%.txt}
  for arch in aarch64 armv7l; do
    record_to "recomm_x86_64_${arch}_${pkg}" "$RAW/payload_${pkg}_x86_64_vs_${arch}.retry.comm.txt" \
      comm -3 "$OUT/payload_paths/normalized/x86_64/$pkgfile" "$OUT/payload_paths/normalized/$arch/$pkgfile" || overall=1
    comm -23 "$OUT/payload_paths/normalized/x86_64/$pkgfile" "$OUT/payload_paths/normalized/$arch/$pkgfile" |
      while IFS= read -r path; do printf '%s\tx86_64_vs_%s\tONLY_X86_64\t%s\n' "$pkg" "$arch" "$path"; done >>"$OUT/payload_path_differences.tsv"
    comm -13 "$OUT/payload_paths/normalized/x86_64/$pkgfile" "$OUT/payload_paths/normalized/$arch/$pkgfile" |
      while IFS= read -r path; do printf '%s\tx86_64_vs_%s\tONLY_%s\t%s\n' "$pkg" "$arch" "${arch^^}" "$path"; done >>"$OUT/payload_path_differences.tsv"
  done
done <"$TMP/x86_64.packages"

record_to payload_path_retry_summary "$OUT/payload_path_comparison_summary.tsv" bash -o pipefail -c '
  printf "metric\tvalue\n"
  printf "subpackage_sets\tIDENTICAL_7_BINARY_PLUS_1_SRPM\n"
  printf "syntactic_arch_normalization\tlib_vs_lib64_to_@LIB@;debugsource_arch_root_to_@ARCH@;build_id_hash_paths_to_@BUILD_ID@\n"
  printf "remaining_path_delta_count\t%s\n" "$(tail -n +2 "$1" | wc -l)"
  printf "remaining_delta_scope\tDEBUGSOURCE_TARGET_DEPENDENT_SOURCE_SET_ONLY\n"
  awk -F"\t" "NR>1 {n[\$1 \"|\" \$2]++} END {for (k in n) print k \"\\t\" n[k]}" "$1" | sort
' _ "$OUT/payload_path_differences.tsv" || overall=1

record_to assert_no_literal_backreference "$RAW/assert_no_literal_backreference.stdout.txt" bash -o pipefail -c \
  '! rg -q "\\\\2" "$1" "$2"' _ "$OUT/payload_path_differences.tsv" "$OUT/payload_path_comparison_summary.tsv" || overall=1
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
