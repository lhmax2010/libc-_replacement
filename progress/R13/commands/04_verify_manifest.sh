#!/bin/bash
set -uo pipefail
verify_pwd=/home/toolchain/development/libc++_replacement
cd "$verify_pwd"
printf 'PWD=%s\n' "$PWD"
printf '%s\n' 'COMMAND=sha256sum -c progress/R13/MANIFEST.sha256'
sha256sum -c progress/R13/MANIFEST.sha256
verify_exit=$?
printf 'EXIT_CODE=%s\n' "$verify_exit"
exit "$verify_exit"
