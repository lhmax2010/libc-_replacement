#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
tree="${workspace}/tmp/R76/series"
check_tree="${workspace}/tmp/R76/apply-check"
patch_root="${workspace}/progress/R69/patches"
out="${workspace}/progress/R76/raw/series_validation"
baseline=8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c
mkdir -p "${out}"

patches=(
  0001-Layer-A-fix-foreign-forced-unwind-resume.patch
  0002-Layer-B-add-GNU-forced-unwind-marker.patch
  0003-Layer-C-header-batch-20-sites.patch
  0004-Layer-C-library-batch-30-sites.patch
)

mapfile -t commits < <(git -C "${tree}" rev-list --reverse "${baseline}..HEAD")
test "${#commits[@]}" -eq 4

printf 'order\tcommit\tparent\tauthor\tauthor_email\tcommitter\tcommitter_email\tpatch_id\n' > "${out}/commits.tsv"
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
  git mailinfo "${per_commit}/mail.message" "${per_commit}/mail.patch" < "${patch}" > "${per_commit}/mailinfo.stdout" 2> "${per_commit}/mailinfo.stderr"
  git -C "${tree}" show -s --format=%B "${commit}" > "${per_commit}/commit.message"
  cmp "${per_commit}/mail.message" "${per_commit}/commit.message"

  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
    "${order}" "${commit}" "${parent}" "${author}" "${author_email}" \
    "${committer}" "${committer_email}" "${commit_patch_id}" >> "${out}/commits.tsv"
  previous="${commit}"
done

git -C "${tree}" diff --check "${baseline}..HEAD"
test -z "$(git -C "${tree}" status --short)"
(cd "${workspace}" && sha256sum -c progress/R69/patches/SHA256SUMS) > "${out}/patch_identity.log"

git -C "${tree}" diff --binary "${baseline}..HEAD" > "${out}/series.diff"
git -C "${check_tree}" diff --cached --binary > "${out}/apply_check.diff"
cmp "${out}/series.diff" "${out}/apply_check.diff"

printf 'R76_SERIES_VALIDATION=PASS\n' > "${out}/STATUS"
