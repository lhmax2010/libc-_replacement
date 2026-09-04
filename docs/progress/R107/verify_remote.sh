#!/bin/bash
set -eu

before=/home/toolchain/development/libc++_replacement/progress/R107/remote_branches_before.tsv
after=/home/toolchain/development/libc++_replacement/progress/R107/remote_branches_after.tsv
existing_after=/home/toolchain/development/libc++_replacement/progress/R107/remote_existing_after.tsv
target=refs/heads/codex/runtime-validation

git ls-remote --heads origin | sort -k2 >"$after"
sed "\|${target}$|d" "$after" >"$existing_after"
cmp -s "$before" "$existing_after"
test "$(wc -l <"$after")" -eq 4
test "$(wc -l <"$existing_after")" -eq 3

local_head=$(git rev-parse HEAD)
remote_head=$(sed -n "\|[[:space:]]${target}$|s/[[:space:]].*//p" "$after")
test -n "$remote_head"
test "$remote_head" = "$local_head"

echo EXISTING_BRANCHES_UNCHANGED=PASS
echo ADDED_BRANCH_COUNT=1
echo NEW_BRANCH="$target"
echo NEW_BRANCH_HEAD="$remote_head"
