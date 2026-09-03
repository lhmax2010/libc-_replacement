#!/bin/bash
set -euo pipefail

if [ "$#" -ne 1 ]; then
  echo "usage: $0 PACKAGE" >&2
  exit 2
fi

package=$1
workspace=/home/toolchain/development/libc++_replacement
repo="$workspace/codes/R104/$package"
target=refs/heads/sandbox/lhmax2025/libcxx-migration
out_dir="$workspace/progress/R104/branch_ops/post_implementation"
mkdir -p "$out_dir"
all_heads="$out_dir/${package}.heads"
existing="$out_dir/${package}.existing-only.heads"
before="$workspace/progress/R104/branch_ops/after/${package}.existing-only.heads"

git -C "$repo" ls-remote --heads origin | sort -k2,2 >"$all_heads"
local_sha=$(git -C "$repo" rev-parse HEAD)
remote_sha=$(rg "$target$" "$all_heads" | cut -f1)
printf 'local=%s\nremote=%s\n' "$local_sha" "$remote_sha"
test "$local_sha" = "$remote_sha"
grep -v "$target$" "$all_heads" >"$existing"
sort -k2,2 "$before" >"$out_dir/${package}.before.by-ref"
cmp "$out_dir/${package}.before.by-ref" "$existing"
git -C "$repo" status --short --branch
echo EXISTING_BRANCHES_UNCHANGED
