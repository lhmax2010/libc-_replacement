#!/usr/bin/env bash
set -u
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/toolchain/.local/bin:/home/toolchain/.vscode/extensions/openai.chatgpt-26.908.40401-linux-x64/bin/linux-x86_64
root=/home/toolchain/development/libc++_replacement/progress/BUILD_STATIC_0917B/resume_0917
label=$1
shift
mkdir -p "$root/raw"
prefix="$root/raw/$label"
test ! -e "$prefix.command.txt" || exit 90
printf '%q ' "$@" > "$prefix.command.txt"
printf '\n' >> "$prefix.command.txt"
date --iso-8601=seconds > "$prefix.started.txt"
"$@" > "$prefix.stdout.txt" 2> "$prefix.stderr.txt"
rc=$?
printf '%s\n' "$rc" > "$prefix.exitcode"
date --iso-8601=seconds > "$prefix.finished.txt"
printf '%s exitcode=%s\n' "$label" "$rc"
exit "$rc"
