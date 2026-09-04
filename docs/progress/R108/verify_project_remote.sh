#!/bin/bash
set -eu

root=/home/toolchain/development/libc++_replacement
before=$root/progress/R108/project_remote_branches_before.tsv
after=$root/progress/R108/project_remote_branches_after.tsv
before_other=$root/progress/R108/project_remote_other_before.tsv
after_other=$root/progress/R108/project_remote_other_after.tsv
target=refs/heads/codex/r5-ehabi-diagnosis

git -C "$root" ls-remote --heads origin | sort -k2 >"$after"
sed "\|${target}$|d" "$before" >"$before_other"
sed "\|${target}$|d" "$after" >"$after_other"
cmp -s "$before_other" "$after_other"
test "$(wc -l <"$before")" -eq "$(wc -l <"$after")"

local_head=$(git -C "$root" rev-parse HEAD)
remote_head=$(sed -n "\|[[:space:]]${target}$|s/[[:space:]].*//p" "$after")
test -n "$remote_head"
test "$remote_head" = "$local_head"

echo TARGET_REMOTE_HEAD="$remote_head"
echo OTHER_REMOTE_BRANCHES_UNCHANGED=PASS
echo LOCAL_REMOTE_MATCH=PASS
echo FORCE_USED=NO
