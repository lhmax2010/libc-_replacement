#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
python3 "$workspace/progress/R14/tools/run_matrix.py"
wc -l "$workspace/progress/R14/tables/matrix_execution.tsv"
find "$workspace/progress/R14/commands/matrix" -type f | wc -l
find "$workspace/progress/R14/raw/matrix" -name 'ld_debug.*' -type f | wc -l
