#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
tree="${workspace}/tmp/R76/series-final"
original_tree="${workspace}/tmp/R76/series"
check_tree="${workspace}/tmp/R76/apply-check"
patch_root="${workspace}/progress/R69/patches"
appendix="${workspace}/progress/R76/code/layer_a_series_dependency.txt"
out="${workspace}/progress/R76/raw/final_series_validation"
baseline=8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c
mkdir -p "${out}"

patches=(
  0001-Layer-A-fix-foreign-forced-unwind-resume.patch
  0002-Layer-B-add-GNU-forced-unwind-marker.patch
  0003-Layer-C-header-batch-20-sites.patch
  0004-Layer-C-library-batch-30-sites.patch
)

mapfile -t commits < <(git -C "${tree}" rev-list --reverse "${baseline}..HEAD")
mapfile -t original_commits < <(git -C "${original_tree}" rev-list --reverse "${baseline}..HEAD")
test "${#commits[@]}" -eq 4
test "${#original_commits[@]}" -eq 4

printf 'order\tcommit\tparent\tauthor\tauthor_email\tcommitter\tcommitter_email\tpatch_id\tmessage_status\n' > "${out}/commits.tsv"
previous="${baseline}"
for index in "${!patches[@]}"; do
  order=$((index + 1))
  patch="${patch_root}/${patches[$index]}"
  commit="${commits[$index]}"
  parent=$(git -C "${tree}" rev-parse "${commit}^")
  test "${parent}" = "${previous}"

  author=$(git -C "${tree}" show -s --format=%an "${commit}")
  author_email=$(git -C "${tree}" show -s --format=%ae "${commit}")
  committer=$(git -C "${tree}" show -s --format=%cn "${commit}")
  committer_email=$(git -C "${tree}" show -s --format=%ce "${commit}")
  test "${committer}" = lhmax2025
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
  git -C "${original_tree}" cat-file commit "${original_commits[$index]}" | sed '1,/^$/d' > "${per_commit}/original.message"
  git -C "${tree}" cat-file commit "${commit}" | sed '1,/^$/d' > "${per_commit}/final.message"
  if [[ "${order}" -eq 1 ]]; then
    { cat "${per_commit}/original.message"; printf '\n'; cat "${appendix}"; } > "${per_commit}/expected.message"
    test "$(awk '/^Change-Id: I[0-9a-f]{40}$/ {n++} END {print n+0}' "${per_commit}/final.message")" -eq 1
    sed '/^Change-Id: I[0-9a-f]\{40\}$/d' "${per_commit}/final.message" | sed '$ { /^$/d; }' > "${per_commit}/final.without_change_id"
    cmp "${per_commit}/expected.message" "${per_commit}/final.without_change_id"
    message_status=APPROVED_DEPENDENCY_AND_CHANGE_ID_APPENDED
  else
    test "$(awk '/^Change-Id:/ {n++} END {print n+0}' "${per_commit}/final.message")" -eq 0
    cmp "${per_commit}/original.message" "${per_commit}/final.message"
    message_status=UNCHANGED
  fi

  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
    "${order}" "${commit}" "${parent}" "${author}" "${author_email}" \
    "${committer}" "${committer_email}" "${commit_patch_id}" "${message_status}" >> "${out}/commits.tsv"
  previous="${commit}"
done

git -C "${tree}" diff --check "${baseline}..HEAD"
test -z "$(git -C "${tree}" status --short)"
(cd "${workspace}" && sha256sum -c progress/R69/patches/SHA256SUMS) > "${out}/patch_identity.log"

git -C "${tree}" diff --binary "${baseline}..HEAD" > "${out}/series.diff"
git -C "${check_tree}" diff --cached --binary > "${out}/apply_check.diff"
cmp "${out}/series.diff" "${out}/apply_check.diff"

rg -n '^Change-Id:' "${out}"/commit*/final.message > "${out}/change_ids.txt"
test "$(wc -l < "${out}/change_ids.txt")" -eq 1
change_id_status=LAYER_A_ONLY
printf 'change_id_status\t%s\n' "${change_id_status}" > "${out}/metadata.tsv"
printf 'R76_FINAL_SERIES_VALIDATION=PASS\n' > "${out}/STATUS"
