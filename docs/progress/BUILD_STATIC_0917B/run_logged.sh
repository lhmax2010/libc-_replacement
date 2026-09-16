#!/usr/bin/env bash
set -u
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/toolchain/.vscode/extensions/openai.chatgpt-26.908.40401-linux-x64/bin/linux-x86_64
task_root=/home/toolchain/development/libc++_replacement/progress/BUILD_STATIC_0917B
label=$1
shift
mkdir -p "$task_root/raw"
prefix="$task_root/raw/$label"
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
