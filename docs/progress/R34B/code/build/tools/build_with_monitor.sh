#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
build=$workspace/tmp/R34B/build-s1
monitor_log=$workspace/progress/R34B/results/build_resource_monitor.log

sample() {
  local label=$1
  {
    printf 'SAMPLE=%s TIME=%s\n' "$label" "$(date --iso-8601=seconds)"
    free -h
    uptime
    awk '/MemAvailable:/{print "MEM_AVAILABLE_KB=" $2}' /proc/meminfo
  } >>"$monitor_log"
}

: >"$monitor_log"
sample START

export CMAKE_BUILD_PARALLEL_LEVEL=2
export LLVM_PARALLEL_LINK_JOBS=1
nice -n 15 ionice -c 3 cmake --build "$build" --target cxxabi -- -j2 &
build_pid=$!
start_seconds=$(date +%s)
next_sample=300
resource_stop=0

while kill -0 "$build_pid" 2>/dev/null; do
  sleep 5
  elapsed=$(( $(date +%s) - start_seconds ))
  if (( elapsed >= next_sample )); then
    sample "PERIODIC_${elapsed}s"
    available_kb=$(awk '/MemAvailable:/{print $2}' /proc/meminfo)
    if (( available_kb < 2 * 1024 * 1024 )); then
      printf 'RED_STOP_RESOURCE MEM_AVAILABLE_KB=%s\n' "$available_kb" >>"$monitor_log"
      kill "$build_pid" 2>/dev/null || true
      resource_stop=1
      break
    fi
    next_sample=$((next_sample + 300))
  fi
done

set +e
wait "$build_pid"
build_rc=$?
set -e
sample END

if (( resource_stop )); then
  exit 99
fi
exit "$build_rc"
