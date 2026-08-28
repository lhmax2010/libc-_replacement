#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
repo="${workspace}/codes/llvm"
out="${workspace}/progress/R76/raw/mistaken_branch_disposal_gate"
formal_base=8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c
old_head=74900f3fbb2969f7f127449f9b1d20df72b9eefa
new_base=5ed6c77278dfa7a470667cf1a137723d3c96fe60
new_head=08a64b6eb95ed74339f1d862a6c22a2f93fa78f9
mkdir -p "${out}"

mapfile -t old_commits < <(git -C "${repo}" rev-list --reverse "${formal_base}..${old_head}")
mapfile -t new_commits < <(git -C "${repo}" rev-list --reverse "${new_base}..${new_head}")
test "${#old_commits[@]}" -eq 4
test "${#new_commits[@]}" -eq 4
test "$(git -C "${repo}" rev-parse "${old_commits[0]}^")" = "${formal_base}"
test "$(git -C "${repo}" rev-parse "${new_commits[0]}^")" = "${new_base}"

printf 'order\told_commit\tnew_commit\told_patch_id\tnew_patch_id\tmessage\tauthor\n' > "${out}/equivalence.tsv"
for index in 0 1 2 3; do
  old_commit="${old_commits[$index]}"
  new_commit="${new_commits[$index]}"
  old_patch_id=$(git -C "${repo}" show --format=email --patch "${old_commit}" | git patch-id --stable)
  old_patch_id=${old_patch_id%% *}
  new_patch_id=$(git -C "${repo}" show --format=email --patch "${new_commit}" | git patch-id --stable)
  new_patch_id=${new_patch_id%% *}
  test "${old_patch_id}" = "${new_patch_id}"

  git -C "${repo}" cat-file commit "${old_commit}" | sed '1,/^$/d' > "${out}/old_message_$((index + 1)).txt"
  git -C "${repo}" cat-file commit "${new_commit}" | sed '1,/^$/d' > "${out}/new_message_$((index + 1)).txt"
  cmp "${out}/old_message_$((index + 1)).txt" "${out}/new_message_$((index + 1)).txt"
  test "$(git -C "${repo}" show -s --format='%an <%ae>' "${old_commit}")" = "$(git -C "${repo}" show -s --format='%an <%ae>' "${new_commit}")"

  printf '%s\t%s\t%s\t%s\t%s\tIDENTICAL\tIDENTICAL\n' \
    "$((index + 1))" "${old_commit}" "${new_commit}" "${old_patch_id}" "${new_patch_id}" >> "${out}/equivalence.tsv"
done

git -C "${repo}" rev-list --reverse "${formal_base}..${old_head}" > "${out}/old_branch_task_commits.txt"
git -C "${repo}" rev-list --reverse "${new_base}..${new_head}" > "${out}/new_branch_task_commits.txt"
git -C "${repo}" log --reverse --format=fuller "${formal_base}..${old_head}" > "${out}/old_branch_task_commits.full"
git -C "${repo}" log --reverse --format=fuller "${new_base}..${new_head}" > "${out}/new_branch_task_commits.full"
printf 'R76_MISTAKEN_BRANCH_DISPOSAL_GATE=PASS\n' > "${out}/STATUS"
