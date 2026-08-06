#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
python3 "$workspace/progress/R14/tools/parse_matrix.py"
for table in "$workspace"/progress/R14/tables/*.tsv; do
  printf '%s\t' "$table"
  wc -l < "$table"
done
