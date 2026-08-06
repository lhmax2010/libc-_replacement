#!/bin/bash
set -uo pipefail
manifest_pwd=/home/toolchain/development/libc++_replacement
manifest_path=progress/R13/MANIFEST.sha256
cd "$manifest_pwd"
printf 'PWD=%s\n' "$PWD"
printf '%s\n' "COMMAND=find progress/R13 -type f ! -path progress/R13/MANIFEST.sha256 ! -path progress/R13/commands/03_generate_manifest.log ! -path progress/R13/commands/04_verify_manifest.log -print0 | sort -z | xargs -0 sha256sum > progress/R13/MANIFEST.sha256"
find progress/R13 -type f ! -path progress/R13/MANIFEST.sha256 ! -path progress/R13/commands/03_generate_manifest.log ! -path progress/R13/commands/04_verify_manifest.log -print0 | sort -z | xargs -0 sha256sum > "$manifest_path"
manifest_exit=$?
printf 'EXIT_CODE=%s\n' "$manifest_exit"
exit "$manifest_exit"
