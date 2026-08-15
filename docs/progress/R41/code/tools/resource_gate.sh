#!/bin/bash
set -u

build_path=${1:-.}
label=${2:-R41_RESOURCE_GATE}

printf 'RESOURCE_GATE_LABEL=%s\n' "$label"
printf 'COMMAND=nproc\n'; nproc
printf 'COMMAND=free -h\n'; free -h
printf 'COMMAND=uptime\n'; uptime
printf 'COMMAND=df -h %q\n' "$build_path"; df -h "$build_path"
printf 'COMMAND=bash -c %q\n' 'top -bn1 | head -20'
top -bn1 | head -20

cpu_count=$(nproc)
mem_available_kib=$(awk '/^MemAvailable:/ {print $2; found=1} END {if (!found) exit 1}' /proc/meminfo) || {
  printf 'RESOURCE_GATE_RESULT=FAIL_MEM_PARSE\n'; exit 10;
}
load_one=$(awk '{print $1}' /proc/loadavg) || {
  printf 'RESOURCE_GATE_RESULT=FAIL_LOAD_PARSE\n'; exit 11;
}
load_limit=$(awk -v cpu="$cpu_count" 'BEGIN {printf "%.3f", cpu * 0.8}')
printf 'CPU_COUNT=%s\nMEM_AVAILABLE_KIB=%s\nMEM_MINIMUM_KIB=8388608\nLOAD_ONE=%s\nLOAD_LIMIT=%s\n' \
  "$cpu_count" "$mem_available_kib" "$load_one" "$load_limit"
if [ "$mem_available_kib" -lt 8388608 ]; then
  printf 'RESOURCE_GATE_RESULT=RED_STOP_RESOURCE_MEMORY_BELOW_8_GIB\n'; exit 20
fi
if awk -v load="$load_one" -v limit="$load_limit" 'BEGIN {exit !(load > limit)}'; then
  printf 'RESOURCE_GATE_RESULT=RED_STOP_RESOURCE_LOAD_ABOVE_0_8_CPU\n'; exit 21
fi
printf 'RESOURCE_GATE_RESULT=PASS\n'

