#!/bin/bash
set -u

build=/home/toolchain/development/libc++_replacement/tmp/R36/build-armv7l
export CMAKE_BUILD_PARALLEL_LEVEL=2
export LLVM_PARALLEL_LINK_JOBS=1

printf 'BUILD_COMMAND=env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1 nice -n 15 ionice -c 3 cmake --build %q --target cxxabi -- -j2\n' "$build"
printf 'MONITOR_INTERVAL_SECONDS=60\n'
printf 'MONITOR_STOP_THRESHOLD_MEMAVAILABLE_KB=%d\n' "$((2 * 1024 * 1024))"

nice -n 15 ionice -c 3 cmake --build "$build" --target cxxabi -- -j2 &
build_pid=$!
resource_stop=0

while kill -0 "$build_pid" 2>/dev/null; do
  printf '=== RESOURCE_SAMPLE %s ===\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
  free -h
  uptime
  mem_kb=$(awk '/MemAvailable:/ {print $2}' /proc/meminfo)
  printf 'MEM_AVAILABLE_KB=%s\n' "$mem_kb"
  if [ "$mem_kb" -lt $((2 * 1024 * 1024)) ]; then
    printf 'RED_STOP_RESOURCE=YES\n'
    kill -TERM "$build_pid"
    resource_stop=1
    break
  fi
  for _ in 1 2 3 4 5 6; do
    kill -0 "$build_pid" 2>/dev/null || break
    sleep 10
  done
done

wait "$build_pid"
build_rc=$?
printf 'BUILD_EXIT_CODE=%d\n' "$build_rc"
printf 'RESOURCE_STOP=%d\n' "$resource_stop"
if [ "$resource_stop" -ne 0 ]; then
  exit 86
fi
exit "$build_rc"
