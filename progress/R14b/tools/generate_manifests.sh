#!/usr/bin/env bash
set -euo pipefail

cd /home/toolchain/development/libc++_replacement
log=progress/R14b/L2/commands/99_generate_manifests.log

generate() {
  local target=$1
  find "$target" -type f ! -name MANIFEST.sha256 -print0 \
    | LC_ALL=C sort -z \
    | xargs -0 sha256sum >"$target/MANIFEST.sha256"
}

printf 'PWD=%s\nCOMMAND=%s\nACTIONS=%s\nEXIT_CODE=PENDING\n' \
  "$PWD" 'progress/R14b/tools/generate_manifests.sh' \
  'generate progress/R14b/MANIFEST.sha256; generate docs/progress/R14b/MANIFEST.sha256' \
  >"$log"
generate progress/R14b
generate docs/progress/R14b
printf 'PWD=%s\nCOMMAND=%s\nACTIONS=%s\nEXIT_CODE=0\n' \
  "$PWD" 'progress/R14b/tools/generate_manifests.sh' \
  'generate progress/R14b/MANIFEST.sha256; generate docs/progress/R14b/MANIFEST.sha256' \
  >"$log"
generate progress/R14b
generate docs/progress/R14b
