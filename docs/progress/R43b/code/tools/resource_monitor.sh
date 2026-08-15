#!/bin/bash
set -u

label=$1
available_kib=$(awk '/MemAvailable:/ {print $2}' /proc/meminfo)
printf 'LABEL=%s\nCOMMAND=free -h\n' "$label"
free -h
printf 'FREE_EXIT_CODE=%d\nCOMMAND=uptime\n' "$?"
uptime
printf 'UPTIME_EXIT_CODE=%d\nMEM_AVAILABLE_KIB=%s\n' "$?" "$available_kib"
if [ "$available_kib" -lt $((1024 * 1024)) ]; then
  printf 'MONITOR_RESULT=RED_STOP_RESOURCE\n'
  exit 20
fi
printf 'MONITOR_RESULT=PASS\n'
exit 0
