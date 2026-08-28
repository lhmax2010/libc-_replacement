#!/usr/bin/env bash
set -euo pipefail

root=/home/toolchain/development/libc++_replacement/progress/R76

rg -q '^result\tSUBMITTED_BUILD_PENDING$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^patch_identity\tPASS_4_OF_4$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^apply_preflight\tPASS_NO_CONFLICT$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^series_validation\tPASS$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^gerrit_push\tPASS_EXIT_0_FAST_FORWARD$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^target_ref\trefs/heads/sandbox/lhmax2025/libcxx-ehabi-backport$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^target_head_before\t5ed6c77278dfa7a470667cf1a137723d3c96fe60$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^target_head_after\t08a64b6eb95ed74339f1d862a6c22a2f93fa78f9$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^existing_head_preserved\tYES_ANCESTOR$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^other_sandbox_branches_unchanged\tYES$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^mistaken_branch_cleanup\tPASS_DELETED_AFTER_EQUIVALENCE_GATE$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^quickbuild_result\tNOT_OBSERVED_REQUIRES_HUMAN_PLATFORM_CHECK$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^formal_branch_push\tNO$' "${root}/SUBMISSION_STATUS.tsv"
rg -q '^llvm_upstream_submission\tNO$' "${root}/SUBMISSION_STATUS.tsv"

test "$(awk -F '\t' 'NR>1 && $4==$5 && $6=="YES" {n++} END{print n+0}' "${root}/PATCH_IDENTITY.tsv")" -eq 4
test "$(awk -F '\t' 'NR>1 {n++} END{print n+0}' "${root}/SUBMISSION_COMMITS.tsv")" -eq 4
rg -q '^R76_EXISTING_BRANCH_APPLY_PREFLIGHT=PASS$' "${root}/raw/apply_existing_branch/STATUS"
rg -q '^R76_EXISTING_BRANCH_SERIES_VALIDATION=PASS$' "${root}/raw/existing_branch_series_validation/STATUS"
rg -q '^R76_MISTAKEN_BRANCH_DISPOSAL_GATE=PASS$' "${root}/raw/mistaken_branch_disposal_gate/STATUS"
rg -q '^EXIT_CODE: 0$' "${root}/commands/127_push_existing_sandbox_branch.log"
rg -q '5ed6c77278df..08a64b6eb95e[[:space:]]+HEAD -> sandbox/lhmax2025/libcxx-ehabi-backport' "${root}/raw/127_push_existing_sandbox_branch.stderr"
rg -q '08a64b6eb95ed74339f1d862a6c22a2f93fa78f9[[:space:]]+refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport' "${root}/raw/139_target_branch_after_cleanup.stdout"
rg -q '8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c[[:space:]]+refs/heads/tizen_base' "${root}/raw/140_formal_branch_after_cleanup.stdout"
test ! -s "${root}/raw/141_mistaken_branch_absence.stdout"
rg -q '^EXIT_CODE: 0$' "${root}/commands/137_delete_mistaken_sandbox_branch.log"
rg -q '^EXIT_CODE: 0$' "${root}/commands/142_cleanup_namespace_expected_check.log"
rg -q 'SUBMITTED_BUILD_PENDING' "${root}/REPORT.md"
rg -q 'NOT_OBSERVED' "${root}/REPORT.md"
rg -q '不得只应用或验证其中一部分' "${root}/code/layer_a_series_dependency.txt"

(cd /home/toolchain/development/libc++_replacement && sha256sum -c progress/R69/patches/SHA256SUMS) > "${root}/raw/092_final_patch_identity.log"
printf 'R76_SUBMITTED_PENDING_REPORT_VALIDATION=PASS\n'
