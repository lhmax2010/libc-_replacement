#!/bin/bash
set -u

label=$1
shift
sample_log=/home/toolchain/development/libc++_replacement/progress/R42/resources/${label}_samples.log
printf 'BUILD_COMMAND='
printf '%q ' "$@"
printf '\n'
env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1 \
  nice -n 15 ionice -c 3 "$@" -j2 &
build_pid=$!
printf 'BUILD_PID=%d\n' "$build_pid"
while kill -0 "$build_pid" 2>/dev/null; do
  {
    date --iso-8601=seconds
    free -h
    uptime
  } >>"$sample_log"
  available_kib=$(awk '/MemAvailable:/ {print $2}' /proc/meminfo)
  if [ "$available_kib" -lt $((2 * 1024 * 1024)) ]; then
    printf 'RED_STOP_RESOURCE MEM_AVAILABLE_KIB=%s\n' "$available_kib" | tee -a "$sample_log"
    kill -TERM "$build_pid"
    wait "$build_pid"
    printf 'BUILD_EXIT_CODE=%d\n' "$?"
    exit 22
  fi
  for _ in $(seq 1 300); do
    kill -0 "$build_pid" 2>/dev/null || break
    sleep 1
  done
done
wait "$build_pid"
rc=$?
printf 'BUILD_EXIT_CODE=%d\n' "$rc"
exit "$rc"
