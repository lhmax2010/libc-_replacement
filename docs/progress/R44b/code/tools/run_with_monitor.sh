#!/usr/bin/env bash
set -u
set -o pipefail

sample_log=${1:?sample log required}
shift

nice -n 15 ionice -c 3 "$@" &
watched_pid=$!
printf 'watched_pid=%s\n' "$watched_pid" >"$sample_log"

while kill -0 "$watched_pid" 2>/dev/null; do
  {
    date --iso-8601=seconds
    printf '%s\n' '$ free -h'
    free -h
    printf '[exit_code=%s]\n' "$?"
    printf '%s\n' '$ uptime'
    uptime
    printf '[exit_code=%s]\n' "$?"
  } >>"$sample_log" 2>&1
  mem_available_kib=$(awk '/^MemAvailable:/ {print $2; exit}' /proc/meminfo)
  if (( mem_available_kib < 1048576 )); then
    printf 'RED_STOP_RESOURCE\tmem_available_kib=%s\n' "$mem_available_kib" >>"$sample_log"
    kill -TERM "$watched_pid" 2>/dev/null || true
    wait "$watched_pid" 2>/dev/null || true
    exit 30
  fi
  elapsed=0
  while (( elapsed < 300 )) && kill -0 "$watched_pid" 2>/dev/null; do
    sleep 1
    elapsed=$((elapsed + 1))
  done
done

wait "$watched_pid"
exit $?
