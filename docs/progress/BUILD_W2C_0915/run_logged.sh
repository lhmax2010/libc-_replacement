#!/usr/bin/env bash
set -u
root=/home/toolchain/development/libc++_replacement/progress/BUILD_W2C_0915
label=$1
shift
mkdir -p "$root/raw"
prefix="$root/raw/$label"
printf '%q ' "$@" > "$prefix.command.txt"
printf '\n' >> "$prefix.command.txt"
date --iso-8601=seconds > "$prefix.started.txt"
"$@" > "$prefix.stdout.txt" 2> "$prefix.stderr.txt"
rc=$?
printf '%s\n' "$rc" > "$prefix.exitcode"
date --iso-8601=seconds > "$prefix.finished.txt"
printf '%s exitcode=%s\n' "$label" "$rc"
exit "$rc"
