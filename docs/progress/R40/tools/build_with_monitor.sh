#!/bin/bash
set -u

if [ "$#" -ne 2 ]; then
  printf 'usage: %s BUILD_DIR TARGET\n' "$0" >&2
  exit 2
fi
build_dir=$1
target=$2
printf 'CMAKE_BUILD_PARALLEL_LEVEL=2\nLLVM_PARALLEL_LINK_JOBS=1\n'
printf 'BUILD_COMMAND=env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1 nice -n 15 ionice -c 3 cmake --build %q --target %q -- -j2\n' "$build_dir" "$target"
printf 'BUILD_START_UTC=%s\n' "$(date -u '+%Y-%m-%dT%H:%M:%SZ')"
printf 'START_SAMPLE_FREE_COMMAND=free -h\n'; free -h
printf 'START_SAMPLE_UPTIME_COMMAND=uptime\n'; uptime

setsid env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1 nice -n 15 ionice -c 3 \
  cmake --build "$build_dir" --target "$target" -- -j2 &
build_pid=$!
printf 'BUILD_PID=%s\n' "$build_pid"
last_sample=$(date +%s)
resource_stop=0
while kill -0 "$build_pid" 2>/dev/null; do
  sleep 10
  now=$(date +%s)
  mem_available_kib=$(awk '/^MemAvailable:/ {print $2; found=1} END {if (!found) exit 1}' /proc/meminfo) || mem_available_kib=0
  if [ "$mem_available_kib" -lt 2097152 ]; then
    printf 'RED_STOP_RESOURCE=MEMORY_BELOW_2_GIB\nMEM_AVAILABLE_KIB=%s\n' "$mem_available_kib"
    printf 'STOP_SAMPLE_FREE_COMMAND=free -h\n'; free -h
    printf 'STOP_SAMPLE_UPTIME_COMMAND=uptime\n'; uptime
    kill -TERM -- "-$build_pid" 2>/dev/null || true
    resource_stop=1
    break
  fi
  if [ $((now - last_sample)) -ge 300 ]; then
    printf 'PERIODIC_SAMPLE_UTC=%s\n' "$(date -u '+%Y-%m-%dT%H:%M:%SZ')"
    printf 'PERIODIC_SAMPLE_FREE_COMMAND=free -h\n'; free -h
    printf 'PERIODIC_SAMPLE_UPTIME_COMMAND=uptime\n'; uptime
    last_sample=$now
  fi
done
wait "$build_pid"
build_rc=$?
printf 'BUILD_EXIT_CODE=%d\n' "$build_rc"
printf 'BUILD_END_UTC=%s\n' "$(date -u '+%Y-%m-%dT%H:%M:%SZ')"
printf 'END_SAMPLE_FREE_COMMAND=free -h\n'; free -h
printf 'END_SAMPLE_UPTIME_COMMAND=uptime\n'; uptime
if [ "$resource_stop" -ne 0 ]; then exit 90; fi
exit "$build_rc"
