#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../.." && pwd)
dest_root="$root/codes/R104"
mkdir -p "$dest_root"

while IFS=$'\t' read -r name repo branch; do
  dest="$dest_root/$name"
  url="ssh://lhmax2025@review.tizen.org:29418/$repo"
  if [ -e "$dest" ]; then
    echo "STOP target already exists: $dest" >&2
    exit 2
  fi
  printf 'CLONE\t%s\t%s\t%s\t%s\n' "$name" "$url" "$branch" "$dest"
  git clone --depth 3 --single-branch --no-tags --branch "$branch" "$url" "$dest"
done <<'EOF'
abseil-cpp	platform/upstream/abseil-cpp	tizen_base
bcc-tools	platform/upstream/bcc	tizen_base
boost	platform/upstream/boost	tizen_base
bpftrace	platform/upstream/bpftrace	tizen_base
icu	platform/upstream/icu	tizen_base
jsoncpp	platform/upstream/jsoncpp	tizen_base
libsigc++	platform/upstream/libsigc++	tizen_base
llvm	platform/upstream/llvm	tizen_base
pcre	platform/upstream/pcre	tizen_base
taglib	platform/upstream/taglib	tizen_base
tensorflow2	platform/upstream/tensorflow2	accepted/tizen_base_toolchain
EOF
