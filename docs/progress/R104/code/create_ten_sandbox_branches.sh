#!/bin/bash
set -u

root=$(cd "$(dirname "$0")/../../.." && pwd)
work_root="$root/codes/R104"
out_root="$root/progress/R104/branch_ops"
table_root="$root/progress/R104/tables"
new_branch="sandbox/lhmax2025/libcxx-migration"
packages=(abseil-cpp bcc-tools boost bpftrace icu jsoncpp 'libsigc++' llvm pcre taglib)

mkdir -p "$out_root/before" "$out_root/after" "$table_root"
: >"$table_root/ten_remote_heads_pre_push.tsv"
: >"$table_root/ten_remote_heads_post_push.tsv"
: >"$table_root/ten_local_heads_pre_push.tsv"
: >"$table_root/ten_local_heads_post_push.tsv"
printf 'package\tref\tsha\n' >>"$table_root/ten_remote_heads_pre_push.tsv"
printf 'package\tref\tsha\n' >>"$table_root/ten_remote_heads_post_push.tsv"
printf 'package\tref\tsha\n' >>"$table_root/ten_local_heads_pre_push.tsv"
printf 'package\tref\tsha\n' >>"$table_root/ten_local_heads_post_push.tsv"

run() {
  printf 'COMMAND'
  printf ' %q' "$@"
  printf '\n'
  "$@"
  rc=$?
  printf 'EXIT_CODE %s\n' "$rc"
  return "$rc"
}

capture_remote() {
  phase=$1
  package=$2
  repo="$work_root/$package"
  output="$out_root/$phase/$package.heads"
  if [ "$phase" = before ]; then
    table_phase=pre
  else
    table_phase=post
  fi
  printf 'COMMAND'
  printf ' %q' timeout 90 git -C "$repo" ls-remote --heads origin
  printf '\n'
  set -o pipefail
  timeout 90 git -C "$repo" ls-remote --heads origin | tee "$output"
  rc=${PIPESTATUS[0]}
  set +o pipefail
  printf 'EXIT_CODE %s\n' "$rc"
  if [ "$rc" -ne 0 ]; then
    printf 'STOP remote-list package=%s phase=%s rc=%s\n' "$package" "$phase" "$rc" >&2
    return "$rc"
  fi
  awk -v package="$package" '{print package "\t" $2 "\t" $1}' "$output" \
    >>"$table_root/ten_remote_heads_${table_phase}_push.tsv"
}

printf 'PHASE preflight-all-remotes\n'
for package in "${packages[@]}"; do
  repo="$work_root/$package"
  test -d "$repo/.git" || { printf 'STOP missing-repo %s\n' "$package" >&2; exit 1; }
  test -z "$(git -C "$repo" status --porcelain)" || {
    printf 'STOP dirty-worktree %s\n' "$package" >&2
    exit 1
  }
  head=$(git -C "$repo" rev-parse HEAD)
  base=$(git -C "$repo" branch --show-current)
  test "$base" = tizen_base || {
    printf 'STOP unexpected-base package=%s branch=%s\n' "$package" "$base" >&2
    exit 1
  }
  git -C "$repo" for-each-ref --format="${package}\t%(refname)\t%(objectname)" refs/heads \
    >>"$table_root/ten_local_heads_pre_push.tsv"
  capture_remote before "$package" || exit $?
  remote_base=$(awk -v ref="refs/heads/tizen_base" '$2==ref{print $1}' "$out_root/before/$package.heads")
  test -n "$remote_base" || {
    printf 'STOP missing-remote-base %s\n' "$package" >&2
    exit 1
  }
  test "$head" = "$remote_base" || {
    printf 'STOP stale-local-head package=%s local=%s remote=%s\n' "$package" "$head" "$remote_base" >&2
    exit 1
  }
  if awk -v ref="refs/heads/$new_branch" '$2==ref{found=1} END{exit found?0:1}' \
      "$out_root/before/$package.heads"; then
    printf 'STOP target-remote-branch-exists %s\n' "$package" >&2
    exit 1
  fi
  if git -C "$repo" show-ref --verify --quiet "refs/heads/$new_branch"; then
    printf 'STOP target-local-branch-exists %s\n' "$package" >&2
    exit 1
  fi
  printf 'PREFLIGHT_OK package=%s base=%s head=%s\n' "$package" "$base" "$head"
done

printf 'PHASE create-and-push\n'
for package in "${packages[@]}"; do
  repo="$work_root/$package"
  head=$(git -C "$repo" rev-parse HEAD)
  run git -C "$repo" branch "$new_branch" "$head" || exit $?
  run git -C "$repo" push origin "$head:refs/heads/$new_branch" || exit $?
done

printf 'PHASE postflight-all-remotes\n'
for package in "${packages[@]}"; do
  repo="$work_root/$package"
  head=$(git -C "$repo" rev-parse HEAD)
  capture_remote after "$package" || exit $?
  git -C "$repo" for-each-ref --format="${package}\t%(refname)\t%(objectname)" refs/heads \
    >>"$table_root/ten_local_heads_post_push.tsv"
  remote_target=$(awk -v ref="refs/heads/$new_branch" '$2==ref{print $1}' "$out_root/after/$package.heads")
  test "$remote_target" = "$head" || {
    printf 'STOP target-head-mismatch package=%s local=%s remote=%s\n' "$package" "$head" "$remote_target" >&2
    exit 1
  }
  grep -v $'\trefs/heads/'"$new_branch"'$' "$out_root/after/$package.heads" \
    >"$out_root/after/$package.existing-only.heads"
  run cmp -s "$out_root/before/$package.heads" "$out_root/after/$package.existing-only.heads" || {
    printf 'STOP existing-remote-ref-changed %s\n' "$package" >&2
    diff -u "$out_root/before/$package.heads" "$out_root/after/$package.existing-only.heads" >&2 || true
    exit 1
  }
  printf 'POSTFLIGHT_OK package=%s target=%s head=%s existing_refs=UNCHANGED\n' \
    "$package" "$new_branch" "$head"
done

printf 'RESULT TEN_BRANCHES_CREATED_AND_VERIFIED\n'
