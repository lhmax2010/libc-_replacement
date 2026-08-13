#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
progress=$workspace/progress/R33
curated=$workspace/docs/progress/R33
(
  cd "$progress"
  find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256
)
mkdir -p "$curated"
cp -a "$progress/." "$curated/"
diff -qr "$progress" "$curated"
printf 'progress_files='; find "$progress" -type f | wc -l
printf 'curated_files='; find "$curated" -type f | wc -l
printf 'oversize_files='; find "$curated" -type f -size +5M | wc -l
