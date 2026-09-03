#!/bin/bash
set -u

root=$(cd "$(dirname "$0")/../../.." && pwd)
repo="$root/codes/R104/tensorflow2"
out="$root/progress/R104/branch_ops/tensorflow2"
table="$root/progress/R104/tables/tensorflow_branch_result.tsv"
base_ref="refs/heads/accepted/tizen_base_toolchain"
base_sha="1cdba73549f741720f11dd53da1bb516b03763a2"
target_branch="sandbox/lhmax2025/libcxx-migration"

mkdir -p "$out"

run() {
  printf 'COMMAND'
  printf ' %q' "$@"
  printf '\n'
  "$@"
  rc=$?
  printf 'EXIT_CODE %s\n' "$rc"
  return "$rc"
}

capture_heads() {
  output=$1
  printf 'COMMAND'
  printf ' %q' timeout 90 git -C "$repo" ls-remote --heads origin
  printf '\n'
  set -o pipefail
  timeout 90 git -C "$repo" ls-remote --heads origin | tee "$output"
  rc=${PIPESTATUS[0]}
  set +o pipefail
  printf 'EXIT_CODE %s\n' "$rc"
  return "$rc"
}

target_ref="refs/heads/$target_branch"
test -d "$repo/.git" || { echo 'STOP missing repository' >&2; exit 1; }
test -z "$(git -C "$repo" status --porcelain)" || { echo 'STOP dirty worktree' >&2; exit 1; }
test "$(git -C "$repo" rev-parse HEAD)" = "$base_sha" || {
  echo 'STOP local HEAD does not equal authorized base' >&2
  exit 1
}
test "$(git -C "$repo" branch --show-current)" = accepted/tizen_base_toolchain || {
  echo 'STOP unexpected local source branch' >&2
  exit 1
}
if git -C "$repo" show-ref --verify --quiet "$target_ref"; then
  echo 'STOP local target branch already exists' >&2
  exit 1
fi

git -C "$repo" for-each-ref --format='%(refname)%09%(objectname)' refs/heads \
  >"$out/local_before.tsv"
capture_heads "$out/remote_before.heads" || exit $?

remote_base=$(awk -v ref="$base_ref" '$2==ref{print $1}' "$out/remote_before.heads")
test "$remote_base" = "$base_sha" || {
  printf 'STOP remote base mismatch expected=%s actual=%s\n' "$base_sha" "$remote_base" >&2
  exit 1
}
if awk -v ref="$target_ref" '$2==ref{found=1} END{exit found?0:1}' "$out/remote_before.heads"; then
  echo 'STOP remote target branch already exists' >&2
  exit 1
fi

run git -C "$repo" branch "$target_branch" "$base_sha" || exit $?
run git -C "$repo" push origin "$base_sha:$target_ref" || exit $?

capture_heads "$out/remote_after.heads" || exit $?
git -C "$repo" for-each-ref --format='%(refname)%09%(objectname)' refs/heads \
  >"$out/local_after.tsv"

remote_target=$(awk -v ref="$target_ref" '$2==ref{print $1}' "$out/remote_after.heads")
test "$remote_target" = "$base_sha" || {
  printf 'STOP remote target mismatch expected=%s actual=%s\n' "$base_sha" "$remote_target" >&2
  exit 1
}
grep -v $'\t'"$target_ref"'$' "$out/remote_after.heads" \
  >"$out/remote_after_existing_only.heads"
run cmp -s "$out/remote_before.heads" "$out/remote_after_existing_only.heads" || {
  echo 'STOP an existing remote ref changed' >&2
  diff -u "$out/remote_before.heads" "$out/remote_after_existing_only.heads" >&2 || true
  exit 1
}

grep -v $'^'"$target_ref"$'\t' "$out/local_after.tsv" \
  >"$out/local_after_existing_only.tsv"
run cmp -s "$out/local_before.tsv" "$out/local_after_existing_only.tsv" || {
  echo 'STOP an existing local ref changed' >&2
  diff -u "$out/local_before.tsv" "$out/local_after_existing_only.tsv" >&2 || true
  exit 1
}

printf 'package\tbase_ref\tbase_sha\ttarget_ref\ttarget_sha\texisting_remote_refs\texisting_local_refs\n' >"$table"
printf 'tensorflow2\t%s\t%s\t%s\t%s\tUNCHANGED\tUNCHANGED\n' \
  "$base_ref" "$base_sha" "$target_ref" "$remote_target" >>"$table"
printf 'RESULT TENSORFLOW_BRANCH_CREATED_AND_VERIFIED base=%s target=%s\n' \
  "$base_sha" "$remote_target"
