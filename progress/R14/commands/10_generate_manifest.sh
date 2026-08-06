#!/usr/bin/env bash
set -uo pipefail

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"
printf 'PWD=%s\n' "$PWD"
printf '%s\n' 'COMMAND=find progress/R14 -type f ! -path progress/R14/MANIFEST.sha256 ! -path progress/R14/commands/10_generate_manifest.log ! -path progress/R14/commands/11_verify_manifest.log -print0 | sort -z | xargs -0 sha256sum > progress/R14/MANIFEST.sha256'
find progress/R14 -type f \
  ! -path progress/R14/MANIFEST.sha256 \
  ! -path progress/R14/commands/10_generate_manifest.log \
  ! -path progress/R14/commands/11_verify_manifest.log \
  -print0 | sort -z | xargs -0 sha256sum > progress/R14/MANIFEST.sha256
rc=$?
printf 'EXIT_CODE=%d\n' "$rc"
exit "$rc"
