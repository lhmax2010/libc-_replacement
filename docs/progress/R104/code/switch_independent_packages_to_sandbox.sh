#!/bin/bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"

for package in abseil-cpp boost icu jsoncpp libsigc++ pcre taglib tensorflow2; do
  repo="codes/R104/$package"
  remote_sha=$(git -C "$repo" ls-remote --heads origin \
    refs/heads/sandbox/lhmax2025/libcxx-migration | cut -f1)
  local_sha=$(git -C "$repo" rev-parse sandbox/lhmax2025/libcxx-migration)
  current_branch=$(git -C "$repo" branch --show-current)
  printf '%s remote=%s local_branch=%s current=%s\n' \
    "$package" "$remote_sha" "$local_sha" "$current_branch"
  test -n "$remote_sha"
  test "$remote_sha" = "$local_sha"
  git -C "$repo" switch sandbox/lhmax2025/libcxx-migration
  git -C "$repo" status --short --branch
done
