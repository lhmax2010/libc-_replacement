#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
tree="${workspace}/tmp/R76/ehabi-backport-series"
check_tree="${workspace}/tmp/R76/ehabi-backport-apply-check"
reference_tree="${workspace}/tmp/R76/series-final"
patch_root="${workspace}/progress/R69/patches"
out="${workspace}/progress/R76/raw/existing_branch_series_validation"
base=5ed6c77278dfa7a470667cf1a137723d3c96fe60
mkdir -p "${out}"

patches=(
  0001-Layer-A-fix-foreign-forced-unwind-resume.patch
  0002-Layer-B-add-GNU-forced-unwind-marker.patch
  0003-Layer-C-header-batch-20-sites.patch
  0004-Layer-C-library-batch-30-sites.patch
)
reference_commits=(
  a4d590eb5a2165a65e82eec6ff9dcf73f778242c
  c92ab20e624234394804bac7c39bb753bf0ed874
  9765bca48c0498b755ba8f1460abf1ef5a949cea
  74900f3fbb2969f7f127449f9b1d20df72b9eefa
)

mapfile -t commits < <(git -C "${tree}" rev-list --reverse "${base}..HEAD")
test "${#commits[@]}" -eq 4

printf 'order\tcommit\tparent\tauthor\tauthor_email\tcommitter\tcommitter_email\tpatch_id\tmessage_status\n' > "${out}/commits.tsv"
previous="${base}"
for index in "${!patches[@]}"; do
  order=$((index + 1))
  patch="${patch_root}/${patches[$index]}"
  commit="${commits[$index]}"
  reference_commit="${reference_commits[$index]}"
  parent=$(git -C "${tree}" rev-parse "${commit}^")
  test "${parent}" = "${previous}"

  author=$(git -C "${tree}" show -s --format=%an "${commit}")
  author_email=$(git -C "${tree}" show -s --format=%ae "${commit}")
  committer=$(git -C "${tree}" show -s --format=%cn "${commit}")
  committer_email=$(git -C "${tree}" show -s --format=%ce "${commit}")
  test "${committer}" = hao.lin
  test "${committer_email}" = hao.lin@samsung.com

  case "${order}" in
    1|2) test "${author}" = 'R51 Evidence'; test "${author_email}" = r51@example.invalid ;;
    3) test "${author}" = NOT_AVAILABLE; test "${author_email}" = not-available@invalid ;;
    4) test "${author}" = 'He Fangyu'; test "${author_email}" = fangyu.he@samsung.com ;;
  esac

  original_patch_id=$(git patch-id --stable < "${patch}")
  original_patch_id=${original_patch_id%% *}
  commit_patch_id=$(git -C "${tree}" show --format=email --patch "${commit}" | git patch-id --stable)
  commit_patch_id=${commit_patch_id%% *}
  test "${commit_patch_id}" = "${original_patch_id}"

  per_commit="${out}/commit${order}"
  mkdir -p "${per_commit}"
  git -C "${reference_tree}" cat-file commit "${reference_commit}" | sed '1,/^$/d' > "${per_commit}/expected.message"
  git -C "${tree}" cat-file commit "${commit}" | sed '1,/^$/d' > "${per_commit}/actual.message"
  cmp "${per_commit}/expected.message" "${per_commit}/actual.message"
  if [[ "${order}" -eq 1 ]]; then
    test "$(awk '/^Change-Id: I[0-9a-f]{40}$/ {n++} END {print n+0}' "${per_commit}/actual.message")" -eq 1
    rg -q '^Series dependency:$' "${per_commit}/actual.message"
    rg -q '^系列依赖说明：$' "${per_commit}/actual.message"
    message_status=APPROVED_DEPENDENCY_AND_CHANGE_ID_PRESERVED
  else
    test "$(awk '/^Change-Id:/ {n++} END {print n+0}' "${per_commit}/actual.message")" -eq 0
    message_status=UNCHANGED
  fi

  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
    "${order}" "${commit}" "${parent}" "${author}" "${author_email}" \
    "${committer}" "${committer_email}" "${commit_patch_id}" "${message_status}" >> "${out}/commits.tsv"
  previous="${commit}"
done

git -C "${tree}" diff --check "${base}..HEAD"
test -z "$(git -C "${tree}" status --short)"
(cd "${workspace}" && sha256sum -c progress/R69/patches/SHA256SUMS) > "${out}/patch_identity.log"

git -C "${tree}" diff --binary "${base}..HEAD" > "${out}/series.diff"
git -C "${check_tree}" diff --cached --binary > "${out}/apply_check.diff"
cmp "${out}/series.diff" "${out}/apply_check.diff"

git -C "${tree}" log --reverse --format=fuller "${base}..HEAD" > "${out}/commits.full"
git -C "${tree}" log --reverse --format='%H%x09%P%x09%s' "${base}..HEAD" > "${out}/commit_chain.tsv"
printf 'R76_EXISTING_BRANCH_SERIES_VALIDATION=PASS\n' > "${out}/STATUS"
