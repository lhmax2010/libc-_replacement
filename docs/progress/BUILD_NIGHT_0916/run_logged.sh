#!/usr/bin/env bash
set -u
root=/home/toolchain/development/libc++_replacement/progress/BUILD_NIGHT_0916
label=$1
shift
mkdir -p "$root/raw"
prefix="$root/raw/$label"
test ! -e "$prefix.command.txt" || exit 90
printf '%q ' "$@" > "$prefix.command.txt"
printf '\n' >> "$prefix.command.txt"
date --iso-8601=seconds > "$prefix.started.txt"
cap_kib=$(awk '/^MemTotal:/ {print int($2/2)}' /proc/meminfo)
ulimit -v "$cap_kib" || exit 91
printf 'method=ulimit-v\nlimit_kib=%s\nactual_limit_kib=%s\n' "$cap_kib" "$(ulimit -v)" > "$prefix.resources.txt"
nice -n 19 ionice -c 3 "$@" > "$prefix.stdout.txt" 2> "$prefix.stderr.txt"
rc=$?
printf '%s\n' "$rc" > "$prefix.exitcode"
date --iso-8601=seconds > "$prefix.finished.txt"
printf '%s exitcode=%s\n' "$label" "$rc"
exit "$rc"
