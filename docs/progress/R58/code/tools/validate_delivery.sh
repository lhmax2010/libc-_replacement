#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
dst="$workspace/docs/progress/R58"
test -f "$dst/REPORT.md"
test -f "$dst/RESULT.txt"
test -f "$dst/code/patches/0001-Layer-A-libc-abi-preserve-foreign-forced-unwind-stat.patch"
test -f "$dst/code/patches/0002-Layer-B-libc-abi-expose-GNU-forced-unwind-marker-typ.patch"
test -f "$dst/code/patches/0001-Layer-C-header-preserve-forced-unwind-and-ready-async.patch"
test -f "$dst/code/patches/0001-libcxx-preserve-forced-unwind-in-library-catch-sites.patch"
test ! -s "$dst/OVER_5MB.txt"
test -z "$(find "$dst" -type f -size +5M -print -quit)"
test "$(sha256sum "$dst/code/patches/0001-Layer-C-header-preserve-forced-unwind-and-ready-async.patch" | awk '{print $1}')" = 5562639277ff71921d18be41a1ed80755e11c0b70378e45567eadc99a57335c5
test "$(rg -l 'R40_TRACE|R41_TRACE|R42_TRACE' "$dst/code/current" | wc -l)" -eq 0
test "$(awk -F '\t' 'NR>1 && $2=="full" {n++; if($4==20 && $6==20) ok++} END{print n":"ok}' "$dst/tables/async_20_run_summary.tsv")" = 2:2
test "$(awk -F '\t' 'NR>1 {n+=$3; ok+=$4} END{print n":"ok}' "$dst/tables/regression_summary.tsv")" = 76:76
test "$(awk -F= '/TARGET_PROCESS_COUNT=/{v=$2} END{print v}' "$workspace/progress/R58/raw/board_cleanup.log" | tr -d '\r')" = 0
printf 'VALIDATION=PASS\n'
