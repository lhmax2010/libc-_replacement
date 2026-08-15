#!/bin/bash
set -u

label=${1:-R42}
printf 'LABEL=%s\n' "$label"
printf 'COMMAND=nproc\n'
nproc
printf 'NPROC_EXIT_CODE=%d\n' "$?"
printf 'COMMAND=free -h\n'
free -h
printf 'FREE_H_EXIT_CODE=%d\n' "$?"
printf 'COMMAND=uptime\n'
uptime
printf 'UPTIME_EXIT_CODE=%d\n' "$?"
printf 'COMMAND=df -h /home/toolchain/development/libc++_replacement/tmp\n'
df -h /home/toolchain/development/libc++_replacement/tmp
printf 'DF_EXIT_CODE=%d\n' "$?"
printf 'COMMAND=top -bn1 | head -20\n'
top -bn1 | head -20
printf 'TOP_EXIT_CODE=%d\n' "${PIPESTATUS[0]}"

nproc_value=$(nproc)
available_kib=$(awk '/MemAvailable:/ {print $2}' /proc/meminfo)
load_one=$(awk '{print $1}' /proc/loadavg)
min_kib=$((8 * 1024 * 1024))
load_limit=$(awk -v n="$nproc_value" 'BEGIN { printf "%.2f", n * 0.8 }')
printf 'NPROC_VALUE=%s\nMEM_AVAILABLE_KIB=%s\nLOAD_ONE=%s\nLOAD_LIMIT=%s\n' \
  "$nproc_value" "$available_kib" "$load_one" "$load_limit"

if [ "$available_kib" -lt "$min_kib" ]; then
  printf 'GATE_RESULT=RED_STOP_RESOURCE_MEMORY\n'
  exit 20
fi
if awk -v load="$load_one" -v limit="$load_limit" 'BEGIN { exit !(load > limit) }'; then
  printf 'GATE_RESULT=RETRYABLE_HIGH_LOAD\n'
  exit 21
fi
printf 'GATE_RESULT=PASS\n'
exit 0
