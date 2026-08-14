#!/bin/bash
set -euo pipefail

printf 'MATRIX_DATA_ROWS='
tail -n +2 progress/R38/tables/matrix_results.tsv | wc -l
printf 'PATCHED_ROWS='
awk -F '\t' 'NR > 1 && $1 == "patched_libcxxabi" {n++} END {print n+0}' progress/R38/tables/matrix_results.tsv
printf 'LIBSTDCXX_ROWS='
awk -F '\t' 'NR > 1 && $1 == "libstdcxx" {n++} END {print n+0}' progress/R38/tables/matrix_results.tsv
printf 'UNPATCHED_ROWS='
awk -F '\t' 'NR > 1 && $1 == "unpatched_libcxxabi" {n++} END {print n+0}' progress/R38/tables/matrix_results.tsv
printf 'LIBSTDCXX_NONZERO='
awk -F '\t' 'NR > 1 && $1 == "libstdcxx" && $3 != 0 {n++} END {print n+0}' progress/R38/tables/matrix_results.tsv
printf 'PATCHED_RED='
awk -F '\t' 'NR > 1 && $1 == "patched_libcxxabi" && $8 == "RED" {n++} END {print n+0}' progress/R38/tables/matrix_results.tsv

cmp -s progress/R38/code/tests/s1_rethrow_probe.cpp progress/R34B/src/s1_rethrow_probe.cpp
printf 'STRICT_SOURCE_CMP_RC=%d\n' "$?"

max=$(find progress/R38 -type f -printf '%s\n' | sort -nr | head -n1)
printf 'MAX_LOCAL_FILE_BYTES=%s\n' "$max"
test "$max" -le 5000000

grep -q '^EXIT_CODE=0$' progress/R38/commands/040_board_matrix.log
grep -q '^EXIT_CODE=0$' progress/R38/commands/041_cleanup_board_artifacts.log
grep -q '^EXIT_CODE=0$' progress/R38/commands/043_verify_board_cleanup_readonly.log
grep -q '^EXIT_CODE=1$' progress/R38/commands/045_board_postcleanup_process_match.log
printf 'EVIDENCE_ASSERTIONS=PASS\n'
