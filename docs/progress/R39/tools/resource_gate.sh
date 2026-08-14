#!/bin/bash
set -u

label=${1:-UNSPECIFIED}
printf 'RESOURCE_GATE_LABEL=%s\n' "$label"
printf 'UTC=%s\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
nproc
free -h
uptime
df -h /home/toolchain/development/libc++_replacement/tmp

cpus=$(nproc)
available_kib=$(awk '/^MemAvailable:/ {print $2}' /proc/meminfo)
load1=$(awk '{print $1}' /proc/loadavg)
limit=$(awk -v n="$cpus" 'BEGIN {printf "%.3f", n/2}')
printf 'NPROC_NUMERIC=%s\nMEM_AVAILABLE_KIB=%s\nLOAD1=%s\nLOAD_LIMIT=%s\n' \
  "$cpus" "$available_kib" "$load1" "$limit"

if [ "$available_kib" -lt 8388608 ]; then
  printf 'RESOURCE_GATE_RESULT=RED_STOP_RESOURCE_MEMORY_LT_8GB\n'
  exit 10
fi
if ! awk -v load="$load1" -v limit="$limit" 'BEGIN {exit !(load <= limit)}'; then
  printf 'RESOURCE_GATE_RESULT=RED_STOP_RESOURCE_LOAD_GT_HALF_CPU\n'
  exit 11
fi
printf 'RESOURCE_GATE_RESULT=PASS\n'
exit 0
