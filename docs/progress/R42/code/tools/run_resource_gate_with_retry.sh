#!/bin/bash
set -u

label=$1
out_dir=/home/toolchain/development/libc++_replacement/progress/R42/resources
mkdir -p "$out_dir"
attempt=1
while [ "$attempt" -le 3 ]; do
  log="$out_dir/${label}_attempt${attempt}.log"
  /home/toolchain/development/libc++_replacement/progress/R42/tools/resource_gate.sh \
    "$label-attempt-$attempt" >"$log" 2>&1
  rc=$?
  printf 'ATTEMPT=%d LOG=%s EXIT_CODE=%d\n' "$attempt" "$log" "$rc"
  if [ "$rc" -eq 0 ]; then
    exit 0
  fi
  if [ "$rc" -eq 20 ]; then
    printf 'RED_STOP_RESOURCE\n'
    exit "$rc"
  fi
  if [ "$rc" -ne 21 ]; then
    printf 'RED_STOP_RESOURCE_GATE_ERROR\n'
    exit "$rc"
  fi
  if [ "$attempt" -lt 3 ]; then
    printf 'WAIT_SECONDS=600\n'
    sleep 600
  fi
  attempt=$((attempt + 1))
done
printf 'RED_STOP_RESOURCE_HIGH_LOAD_AFTER_RETRIES\n'
exit 21
