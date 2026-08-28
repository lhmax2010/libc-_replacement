#!/usr/bin/env bash
set -euo pipefail

root=/home/toolchain/development/libc++_replacement/progress/R76

rg -q '^result\tSUBMITTED_BUILD_PENDING$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^patch_identity\tPASS_4_OF_4$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^apply_preflight\tPASS_NO_CONFLICT$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^series_validation\tPASS$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^gerrit_push\tPASS_EXIT_0$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^quickbuild_result\tNOT_OBSERVED_REQUIRES_HUMAN_PLATFORM_CHECK$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^formal_branch_push\tNO$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^llvm_upstream_submission\tNO$' "${root}/SUBMISSION_STATUS.tsv"

test "$(awk -F '\t' 'NR>1 && $4==$5 && $6=="YES" {n++} END{print n+0}' "${root}/PATCH_IDENTITY.tsv")" -eq 4
test "$(awk -F '\t' 'NR>1 {n++} END{print n+0}' "${root}/SUBMISSION_COMMITS.tsv")" -eq 4
rg -q '^R76_APPLY_PREFLIGHT=PASS$' "${root}/raw/apply_preflight/STATUS"
rg -q '^R76_FINAL_SERIES_VALIDATION=PASS$' "${root}/raw/final_series_validation/STATUS"
rg -q '^EXIT_CODE: 0$' "${root}/commands/086_gerrit_sandbox_push.log"
rg -q 'HEAD -> sandbox/lhmax2025/libcxx-forced-unwind' "${root}/raw/086_gerrit_sandbox_push.combined"
rg -q '74900f3fbb2969f7f127449f9b1d20df72b9eefa[[:space:]]+refs/heads/sandbox/lhmax2025/libcxx-forced-unwind' "${root}/raw/087_gerrit_post_push_verify.txt"
rg -q '8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c[[:space:]]+refs/heads/tizen_base' "${root}/raw/087_gerrit_post_push_verify.txt"
rg -q 'SUBMITTED_BUILD_PENDING' "${root}/REPORT.md"
rg -q 'NOT_OBSERVED' "${root}/REPORT.md"
rg -q '不得只应用或验证其中一部分' "${root}/code/layer_a_series_dependency.txt"

(cd /home/toolchain/development/libc++_replacement && sha256sum -c progress/R69/patches/SHA256SUMS) > "${root}/raw/092_final_patch_identity.log"
printf 'R76_SUBMITTED_PENDING_REPORT_VALIDATION=PASS\n'
