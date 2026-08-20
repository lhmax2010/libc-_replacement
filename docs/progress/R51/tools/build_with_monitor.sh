#!/usr/bin/env bash
set -euo pipefail

build_dir=${1:?build directory required}
log_dir=${2:?monitor log directory required}
mkdir -p "$log_dir"

monitor() {
  local sample=0 available_kib
  while :; do
    sample=$((sample + 1))
    {
      printf 'SAMPLE=%d DATE=' "$sample"
      date --iso-8601=seconds
      free -h
      uptime
    } >>"$log_dir/resource_samples.log" 2>&1
    available_kib=$(awk '/^MemAvailable:/ {print $2}' /proc/meminfo)
    if (( available_kib < 1048576 )); then
      printf 'RED_STOP_RESOURCE MemAvailable_KiB=%d threshold_KiB=1048576\n' "$available_kib" >>"$log_dir/resource_samples.log"
      kill -TERM "$build_pid" 2>/dev/null || true
      return 90
    fi
    sleep 300
  done
}

cmd=(env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1
  nice -n 15 ionice -c 3 cmake --build "$build_dir" --target cxxabi_shared -- -j2)
printf 'BUILD_COMMAND='
printf '%q ' "${cmd[@]}"
printf '\n'
"${cmd[@]}" &
build_pid=$!
monitor &
monitor_pid=$!
set +e
wait "$build_pid"
build_rc=$?
kill "$monitor_pid" 2>/dev/null
wait "$monitor_pid" 2>/dev/null
monitor_rc=$?
set -e
printf 'BUILD_EXIT_CODE=%d\nMONITOR_EXIT_CODE=%d\n' "$build_rc" "$monitor_rc"
if (( monitor_rc == 90 )); then
  exit 90
fi
exit "$build_rc"
