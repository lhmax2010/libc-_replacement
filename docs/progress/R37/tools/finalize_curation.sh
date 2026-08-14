#!/bin/bash
set -euo pipefail

root=/home/toolchain/development/libc++_replacement
cd "$root"

oversized=$(find progress/R37 -type f -size +5M -print)
if [ -n "$oversized" ]; then
    printf 'CURATION_RED_STOP_OVERSIZED\n%s\n' "$oversized" >&2
    exit 90
fi

(
    cd progress/R37
    find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256
)

mkdir -p docs/progress/R37
cp -a progress/R37/. docs/progress/R37/
(
    cd docs/progress/R37
    find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256
    sha256sum -c MANIFEST.sha256
)

printf 'PROGRESS_FILES=%s\n' "$(find progress/R37 -type f | wc -l)"
printf 'CURATED_FILES=%s\n' "$(find docs/progress/R37 -type f | wc -l)"
printf 'MAX_PROGRESS_FILE_BYTES=%s\n' "$(find progress/R37 -type f -printf '%s\n' | sort -nr | head -n 1)"
