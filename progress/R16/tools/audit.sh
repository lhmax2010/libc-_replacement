#!/usr/bin/env bash
set -euo pipefail
for path in \
  progress/R16/REPORT.md \
  progress/R16/METHODOLOGY.md \
  progress/R16/tables/input_identity.tsv \
  progress/R16/tables/libgcc_export_classification.tsv \
  progress/R16/tables/libgcc_symbol_versions.tsv \
  progress/R16/tables/armv7l_aeabi_symbols.tsv \
  progress/R16/tables/compiler_rt_missing_helpers.tsv \
  progress/R16/tables/compiler_rt_only_symbols.tsv \
  progress/R16/tables/board_followup_carriers.tsv; do
  test -s "$path"
done
for table in progress/R16/tables/*.tsv; do
  awk -F '\t' 'NR==1 { n=NF } NF!=n { print FNR, NF, n; bad=1 } END { exit bad }' "$table"
done
grep -q D106703 progress/R16/REPORT.md
grep -q SDB_RECOVERED_UNWIND_LINK_LOAD_NOT_OBSERVED progress/R16/REPORT.md
grep -q UNWIND_LINK_LOAD_NOT_OBSERVED progress/R16/tables/runtime_observation.tsv
grep -q 'PRESENT@0x' progress/R16/tables/glibc_dependency.tsv
test "$(awk -F '\t' 'NR>1 { n++ } END { print n }' progress/R16/tables/armv7l_aeabi_symbols.tsv)" -eq 69
test "$(awk -F '\t' 'NR>1 && $3=="NO" { n++ } END { print n+0 }' progress/R16/tables/armv7l_aeabi_symbols.tsv)" -eq 7
echo AUDIT=PASS
