#!/bin/bash
set -u

label=${1:-R43b}
printf 'LABEL=%s\n' "$label"
for command_name in nproc 'free -h' uptime 'df -h /home/toolchain/development/libc++_replacement/tmp' 'top -bn1 | head -20'; do
  printf 'COMMAND=%s\n' "$command_name"
  bash -o pipefail -c "$command_name"
  printf 'EXIT_CODE=%d\n' "$?"
done

nproc_value=$(nproc)
available_kib=$(awk '/MemAvailable:/ {print $2}' /proc/meminfo)
load_one=$(awk '{print $1}' /proc/loadavg)
min_kib=$((2 * 1024 * 1024))
load_limit=$(awk -v n="$nproc_value" 'BEGIN { printf "%.2f", n * 1.5 }')
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
