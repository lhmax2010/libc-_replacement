#!/bin/bash
set -u

if [ "$#" -ne 3 ]; then
  printf 'usage: %s BUILD_PATH LABEL LOG_DIR\n' "$0" >&2
  exit 2
fi
build_path=$1
label=$2
log_dir=$3
mkdir -p "$log_dir"

for attempt in 1 2 3; do
  log="$log_dir/${label}_attempt${attempt}.log"
  "$(dirname "$0")/run_logged.sh" "$log" "$(dirname "$0")/resource_gate.sh" "$build_path" "${label}_ATTEMPT_${attempt}"
  rc=$?
  printf 'RESOURCE_GATE_ATTEMPT=%d EXIT_CODE=%d LOG=%s\n' "$attempt" "$rc" "$log"
  if [ "$rc" -eq 0 ]; then
    printf 'RESOURCE_GATE_RETRY_RESULT=PASS ATTEMPTS=%d\n' "$attempt"
    exit 0
  fi
  if [ "$rc" -ne 21 ]; then
    printf 'RESOURCE_GATE_RETRY_RESULT=RED_STOP_RESOURCE_NON_LOAD_FAILURE EXIT_CODE=%d\n' "$rc"
    exit "$rc"
  fi
  if [ "$attempt" -eq 3 ]; then
    printf 'RESOURCE_GATE_RETRY_RESULT=RED_STOP_RESOURCE_LOAD_RETRIES_EXHAUSTED\n'
    exit 21
  fi
  printf 'COMMAND=sleep 600\n'
  sleep 600
  printf 'SLEEP_EXIT_CODE=%d\n' "$?"
done
