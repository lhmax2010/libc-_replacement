#!/usr/bin/env bash
set -u
set -o pipefail

sample_log=${1:?sample log required}
shift

printf 'COMMAND='
printf '%q ' nice -n 15 ionice -c 3 "$@"
printf '\n'
nice -n 15 ionice -c 3 "$@" &
watched_pid=$!
printf 'WATCHED_PID=%s\n' "$watched_pid"

while kill -0 "$watched_pid" 2>/dev/null; do
  {
    date --iso-8601=seconds
    printf '%s\n' '$ free -h'
    free -h
    printf 'EXIT_CODE=%s\n' "$?"
    printf '%s\n' '$ uptime'
    uptime
    printf 'EXIT_CODE=%s\n' "$?"
  } >>"$sample_log" 2>&1
  mem_available_kib=$(awk '/^MemAvailable:/ {print $2; exit}' /proc/meminfo)
  if (( mem_available_kib < 1048576 )); then
    printf 'RED_STOP_RESOURCE MEM_AVAILABLE_KIB=%s\n' "$mem_available_kib" | tee -a "$sample_log"
    kill -TERM "$watched_pid" 2>/dev/null || true
    wait "$watched_pid" 2>/dev/null || true
    printf 'EXIT_CODE=30\n'
    exit 30
  fi
  elapsed=0
  while (( elapsed < 300 )) && kill -0 "$watched_pid" 2>/dev/null; do
    sleep 1
    elapsed=$((elapsed + 1))
  done
done

wait "$watched_pid"
rc=$?
printf 'EXIT_CODE=%s\n' "$rc"
exit "$rc"
