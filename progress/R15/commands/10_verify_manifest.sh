#!/usr/bin/env bash
set -uo pipefail

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"
printf 'PWD=%s\n' "$PWD"
printf '%s\n' 'COMMAND=sha256sum -c progress/R15/MANIFEST.sha256'
sha256sum -c progress/R15/MANIFEST.sha256
rc=$?
printf 'EXIT_CODE=%d\n' "$rc"
exit "$rc"
