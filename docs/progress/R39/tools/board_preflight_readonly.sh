#!/bin/sh
set -u

printf 'BOARD_PREFLIGHT_BEGIN\n'
date -u +UTC=%Y-%m-%dT%H:%M:%SZ
uname -a
printf 'TARGET_PIDS_BEGIN\n'
for pid in 9448 9450; do
  if kill -0 "$pid" 2>/dev/null; then
    printf 'PID_PRESENT=%s\n' "$pid"
  else
    printf 'PID_ABSENT=%s\n' "$pid"
  fi
done
printf 'TARGET_PIDS_END\n'
printf 'PROJECT_PROCESS_SCAN_BEGIN\n'
ps -ef | awk 'NR == 1 || /r39|r38_s1|r36_s1|s1_libcxx|s1_libstdcxx/ { print }'
printf 'PROJECT_PROCESS_SCAN_END\n'
printf 'BOARD_PREFLIGHT_END\n'
