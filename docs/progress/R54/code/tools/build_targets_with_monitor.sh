#!/usr/bin/env bash
set -euo pipefail
build_dir=${1:?build directory required}
log_dir=${2:?monitor directory required}
shift 2
(( $# > 0 )) || { printf 'ERROR: at least one target required\n' >&2; exit 2; }
mkdir -p "$log_dir"

monitor() {
  local sample=0 available_kib elapsed=300
  while :; do
    sample=$((sample + 1))
    {
      printf 'SAMPLE=%d DATE=' "$sample"
      date --iso-8601=seconds
      free -h
      uptime
    } >> "$log_dir/resource_samples.log" 2>&1
    available_kib=$(awk '/^MemAvailable:/ {print $2}' /proc/meminfo)
    if (( available_kib < 1048576 )); then
      printf 'RED_STOP_RESOURCE MemAvailable_KiB=%d threshold_KiB=1048576\n' "$available_kib" >> "$log_dir/resource_samples.log"
      kill -TERM "$build_pid" 2>/dev/null || true
      return 90
    fi
    while (( elapsed > 0 )); do
      kill -0 "$build_pid" 2>/dev/null || return 0
      sleep 5
      elapsed=$((elapsed - 5))
    done
    elapsed=300
  done
}

overall=0
for target in "$@"; do
  cmd=(env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1
    nice -n 15 ionice -c 3 cmake --build "$build_dir" --target "$target" -- -j2)
  printf 'BUILD_COMMAND='; printf '%q ' "${cmd[@]}"; printf '\n'
  "${cmd[@]}" &
  build_pid=$!
  monitor & monitor_pid=$!
  set +e
  wait "$build_pid"; build_rc=$?
  kill "$monitor_pid" 2>/dev/null
  wait "$monitor_pid"; monitor_rc=$?
  set -e
  printf 'TARGET=%s BUILD_EXIT_CODE=%d MONITOR_EXIT_CODE=%d\n' "$target" "$build_rc" "$monitor_rc"
  if (( monitor_rc == 90 )); then exit 90; fi
  if (( build_rc != 0 )); then overall=$build_rc; fi
done
exit "$overall"
