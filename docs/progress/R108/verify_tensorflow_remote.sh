#!/bin/bash
set -eu

root=/home/toolchain/development/libc++_replacement
repo=$root/codes/R104/tensorflow2
before=$root/progress/R108/tensorflow_remote_branches_before.tsv
after=$root/progress/R108/tensorflow_remote_branches_after.tsv
before_other=$root/progress/R108/tensorflow_remote_other_before.tsv
after_other=$root/progress/R108/tensorflow_remote_other_after.tsv
target=refs/heads/sandbox/lhmax2025/libcxx-migration
base=1cdba73549f741720f11dd53da1bb516b03763a2

git -C "$repo" ls-remote --heads origin | sort -k2 >"$after"
sed "\|${target}$|d" "$before" >"$before_other"
sed "\|${target}$|d" "$after" >"$after_other"
cmp -s "$before_other" "$after_other"
test "$(wc -l <"$before")" -eq "$(wc -l <"$after")"

local_head=$(git -C "$repo" rev-parse HEAD)
remote_head=$(sed -n "\|[[:space:]]${target}$|s/[[:space:]].*//p" "$after")
test -n "$remote_head"
test "$remote_head" = "$local_head"
git -C "$repo" merge-base --is-ancestor "$base" "$local_head"
test "$(git -C "$repo" rev-list --count "$base..$local_head")" -eq 1

echo TARGET_REMOTE_HEAD="$remote_head"
echo EXISTING_BASE_RETAINED=PASS
echo OTHER_REMOTE_BRANCHES_UNCHANGED=PASS
echo FORCE_USED=NO
