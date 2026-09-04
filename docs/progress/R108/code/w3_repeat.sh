#!/bin/bash
set -euo pipefail
if [ "$#" -ne 1 ]; then
  exit 2
fi
for run in 1 2 3 4 5; do
  printf 'run=%d\n' "$run"
  "$1"
done
