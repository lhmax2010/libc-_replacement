#!/usr/bin/env bash
set -euo pipefail

root=/home/toolchain/development/libc++_replacement/progress/R76

rg -q '^result\tBLOCKED$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^resource_gate\tPASS$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^gerrit_push\tNOT_ATTEMPTED$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^quickbuild\tNOT_OBSERVED$' "${root}/SUBMISSION_STATUS.tsv"
test "$(awk -F '\t' 'NR>1 && $4==$5 && $6=="YES" {n++} END{print n+0}' "${root}/PATCH_IDENTITY.tsv")" -eq 4
rg -q '^EXIT_CODE: 1$' "${root}/raw/apply_preflight/004_patch_identity.log"
test "$(find "${root}/raw/apply_preflight" -maxdepth 1 -type f | wc -l)" -eq 4
test ! -s "${root}/raw/020_remote_baseline_and_sandbox_before.stderr"
test "$(wc -l < "${root}/raw/020_remote_baseline_and_sandbox_before.stdout")" -eq 1

printf 'R76_BLOCKED_REPORT_VALIDATION=PASS\n'
