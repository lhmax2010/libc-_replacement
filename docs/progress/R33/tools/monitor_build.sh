#!/usr/bin/env bash
set -uo pipefail
label=$1
shift
log="progress/R33/results/${label}_resource_monitor.log"
mkdir -p "$(dirname "$log")"
export CMAKE_BUILD_PARALLEL_LEVEL=4
export LLVM_PARALLEL_LINK_JOBS=1
{
  printf 'PWD=%q\n' "$PWD"
  printf 'CMAKE_BUILD_PARALLEL_LEVEL=%s\n' "$CMAKE_BUILD_PARALLEL_LEVEL"
  printf 'LLVM_PARALLEL_LINK_JOBS=%s\n' "$LLVM_PARALLEL_LINK_JOBS"
  printf 'COMMAND=nice -n 15 ionice -c 3 '; printf '%q ' "$@"; printf '\n'
  date --iso-8601=seconds
  free -h
  uptime
} >"$log"
nice -n 15 ionice -c 3 "$@" &
pid=$!
printf 'BUILD_PID=%s\n' "$pid" >>"$log"
while kill -0 "$pid" 2>/dev/null; do
  sleep 300 &
  sleeper=$!
  while kill -0 "$pid" 2>/dev/null && kill -0 "$sleeper" 2>/dev/null; do
    sleep 2
  done
  kill "$sleeper" 2>/dev/null || true
  wait "$sleeper" 2>/dev/null || true
  {
    date --iso-8601=seconds
    free -h
    uptime
  } >>"$log"
  available_kb=$(awk '/MemAvailable:/{print $2}' /proc/meminfo)
  if (( available_kb < 2 * 1024 * 1024 )); then
    printf 'RED_STOP_RESOURCE available memory below 2 GiB; terminating PID %s\n' "$pid" >>"$log"
    kill -TERM "$pid" 2>/dev/null || true
    wait "$pid" 2>/dev/null || true
    printf 'EXIT_CODE=20\n' >>"$log"
    exit 20
  fi
done
wait "$pid"
rc=$?
{
  date --iso-8601=seconds
  free -h
  uptime
  printf 'EXIT_CODE=%s\n' "$rc"
} >>"$log"
exit "$rc"
