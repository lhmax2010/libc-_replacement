#!/usr/bin/env bash
set -euo pipefail
root=/home/toolchain/development/libc++_replacement
cd "$root"

missing=0
for command in progress/R59/commands/*.command.txt; do
  exit_file=${command%.command.txt}.exit.txt
  if [[ ! -f $exit_file ]]; then
    if [[ ${R59_SKIP_PENDING_COMMAND:-} == "${command##*/}" ]]; then
      printf 'PENDING_CURRENT_COMMAND_SKIPPED=%s\n' "$command"
      continue
    fi
    printf 'MISSING_EXIT=%s\n' "$exit_file"
    missing=1
  fi
done
printf 'COMMAND_EXIT_PAIR_STATUS=%s\n' "$([[ $missing -eq 0 ]] && printf PASS || printf FAIL)"
[[ $missing -eq 0 ]]

awk -F '\t' 'NR > 1 {print $4 "  " $3}' docs/progress/R59/code/INDEX.tsv | sha256sum -c -

diff -u progress/R59/src/r59_probe.cpp docs/progress/R59/code/src/r59_probe.cpp
diff -u progress/R59/tables/reproduction_matrix.tsv docs/progress/R59/tables/reproduction_matrix.tsv

awk -F '\t' '
  NR == 1 {next}
  $1 == "F1" && $2 == "current" && $3 == 86 {ok[1]=1}
  $1 == "F1" && $2 == "baseline" && $3 == 0 {ok[2]=1}
  $1 == "F1" && $2 == "reference" && $3 == 0 {ok[3]=1}
  $1 == "F2" && $2 == "current" && $3 == 5 {ok[4]=1}
  $1 == "F2" && $2 == "baseline" && $3 == 134 {ok[5]=1}
  $1 == "F2" && $2 == "reference" && $3 == 0 {ok[6]=1}
  $1 == "M1" && $2 == "current" && $3 == 6 {ok[7]=1}
  $1 == "M1" && $2 == "baseline" && $3 == 134 {ok[8]=1}
  $1 == "M1" && $2 == "reference" && $3 == 0 {ok[9]=1}
  $1 == "M2" && $2 == "current" && $3 == 86 {ok[10]=1}
  $1 == "M2" && $2 == "baseline" && $3 == 86 {ok[11]=1}
  $1 == "M2" && $2 == "reference" && $3 == 0 {ok[12]=1}
  END {for (i=1;i<=12;i++) if (!ok[i]) exit 1; print "MATRIX_EXPECTED_ROWS=12 PASS"}
' progress/R59/tables/reproduction_matrix.tsv

if git status --porcelain -- codes docs/patches | grep -v '^?? docs/progress/R59/' | grep -q .; then
  printf 'FORBIDDEN_SOURCE_OR_PATCH_CHANGE=FOUND\n'
  exit 1
fi
printf 'FORBIDDEN_SOURCE_OR_PATCH_CHANGE=NONE\n'

find progress/R59 docs/progress/R59 -type f -size +5M -print | tee /tmp/r59_oversize_check.txt
[[ ! -s /tmp/r59_oversize_check.txt ]]
printf 'FILE_SIZE_LIMIT=PASS\n'
