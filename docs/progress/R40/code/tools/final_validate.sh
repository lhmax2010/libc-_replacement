#!/bin/bash
set -euo pipefail

repo=/home/toolchain/development/libc++_replacement
progress="$repo/progress/R40"
docs="$repo/docs/progress/R40"

printf 'COMMAND=(cd %q && sha256sum -c MANIFEST.sha256)\n' "$docs"
(cd "$docs" && sha256sum -c MANIFEST.sha256)

printf 'COMMAND=cmp progress report/INDEX with curated copies\n'
cmp "$progress/R40_report.md" "$docs/R40_report.md"
cmp "$progress/code/INDEX.tsv" "$docs/code/INDEX.tsv"

printf 'COMMAND=find curated files larger than 5 MiB\n'
over=$(find "$docs" -type f -size +5M -print)
printf '%s' "$over"
test -z "$over"

printf 'COMMAND=compare curated implementation files with experiment sources\n'
for variant in diagnostic strong visibility; do
  cmp "$repo/tmp/R40/source-$variant/libcxxabi/include/cxxabi.h" \
    "$docs/code/implementation/$variant/current/libcxxabi/include/cxxabi.h"
  cmp "$repo/tmp/R40/source-$variant/libcxxabi/src/cxa_exception.cpp" \
    "$docs/code/implementation/$variant/current/libcxxabi/src/cxa_exception.cpp"
  cmp "$repo/tmp/R40/source-$variant/libcxxabi/src/cxa_personality.cpp" \
    "$docs/code/implementation/$variant/current/libcxxabi/src/cxa_personality.cpp"
done

printf 'COMMAND=assert matrix cardinality and consumer link mode\n'
test "$(($(wc -l < "$docs/tables/typeinfo_matrix.tsv") - 1))" -eq 4
test "$(rg -l '^EXPORT_MODE=none$' "$docs/code/evidence/commands"/078_build_probe_*.log | wc -l)" -eq 4
test "$(rg -l 'cancel:forced-handler' "$docs/commands"/103_run_*_arm_cancel_valid.log | wc -l)" -eq 2
test "$(rg -l 'CASE_EXIT_CODE=0' "$docs/commands"/080_run_*_x86_cancel.log | wc -l)" -eq 2

printf 'COMMAND=assert board cleanup and platform source cleanliness\n'
rg -q 'PATH_GONE_EXIT_CODE=0' "$docs/commands/107_board_cleanup_files_and_processes.log"
test -z "$(git -C "$repo/codes/llvm" status --porcelain)"

printf 'FINAL_VALIDATION=PASS\n'
