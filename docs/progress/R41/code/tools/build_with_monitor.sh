#!/bin/bash
set -u

if [ "$#" -lt 3 ]; then
  printf 'usage: %s SAMPLE_LOG BUILD_LOG COMMAND [ARG ...]\n' "$0" >&2
  exit 2
fi
sample_log=$1
build_log=$2
shift 2
mkdir -p "$(dirname "$sample_log")" "$(dirname "$build_log")"
: >"$sample_log"
printf 'COMMAND=' >"$build_log"; printf '%q ' "$@" >>"$build_log"; printf '\n' >>"$build_log"
"$@" >>"$build_log" 2>&1 &
build_pid=$!
printf 'BUILD_PID=%d\n' "$build_pid" >>"$sample_log"
while kill -0 "$build_pid" 2>/dev/null; do
  {
    date -Ins
    free -h
    uptime
    mem_available_kib=$(awk '/^MemAvailable:/ {print $2}' /proc/meminfo)
    printf 'MEM_AVAILABLE_KIB=%s\n' "$mem_available_kib"
  } >>"$sample_log" 2>&1
  if [ "$mem_available_kib" -lt 2097152 ]; then
    printf 'RED_STOP_RESOURCE_MEMORY_BELOW_2_GIB=1\n' >>"$sample_log"
    kill -TERM "$build_pid" 2>/dev/null
    wait "$build_pid"
    printf 'EXIT_CODE=20\n' >>"$build_log"
    exit 20
  fi
  for _ in $(seq 1 300); do
    kill -0 "$build_pid" 2>/dev/null || break
    sleep 1
  done
done
wait "$build_pid"
rc=$?
printf 'EXIT_CODE=%d\n' "$rc" >>"$build_log"
exit "$rc"

