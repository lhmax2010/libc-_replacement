#!/bin/bash
set -u

account=lhmax2025
host=review.tizen.org:29418
while IFS=$'\t' read -r name repo; do
  url="ssh://${account}@${host}/${repo}"
  printf '===== %s\t%s =====\n' "$name" "$url"
  git ls-remote --heads "$url" \
    'refs/heads/tizen_base' \
    'refs/heads/tizen_base_toolchain' \
    'refs/heads/accepted/tizen_base' \
    'refs/heads/accepted/tizen_base_toolchain' \
    'refs/heads/sandbox/lhmax2025/*'
done <<'EOF'
abseil-cpp	platform/upstream/abseil-cpp
bcc-tools	platform/upstream/bcc
boost	platform/upstream/boost
bpftrace	platform/upstream/bpftrace
icu	platform/upstream/icu
jsoncpp	platform/upstream/jsoncpp
libsigc++	platform/upstream/libsigc++
llvm	platform/upstream/llvm
pcre	platform/upstream/pcre
taglib	platform/upstream/taglib
tensorflow2	platform/upstream/tensorflow2
EOF
