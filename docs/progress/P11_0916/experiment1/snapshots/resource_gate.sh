#!/usr/bin/env bash
# Fixed resource gate for build and analysis tasks.
# Exit codes:
#   0  gate passed
#   10 script/usage/dependency error
#   20 insufficient available memory
#   21 excessive one-minute load average

set -u
set -o pipefail

level='NOT_AVAILABLE'
cpu_count='NOT_AVAILABLE'
mem_available_kib='NOT_AVAILABLE'
mem_min_kib='NOT_AVAILABLE'
load_one='NOT_AVAILABLE'
load_multiplier='NOT_AVAILABLE'
load_limit='NOT_AVAILABLE'
disk_path=${RESOURCE_GATE_DISK_PATH:-.}
disk_available_kib='NOT_AVAILABLE'

usage() {
  printf 'Usage: %s --level {light|medium|heavy}\n' "$0"
}

self_error() {
  printf 'level=%s\n' "$level"
  printf 'nproc=%s\n' "$cpu_count"
  printf 'mem_available_kib=%s\n' "$mem_available_kib"
  printf 'mem_min_kib=%s\n' "$mem_min_kib"
  printf 'load1=%s\n' "$load_one"
  printf 'load_multiplier=%s\n' "$load_multiplier"
  printf 'load_limit=%s\n' "$load_limit"
  printf 'disk_path=%s\n' "$disk_path"
  printf 'disk_available_kib=%s\n' "$disk_available_kib"
  printf 'gate_result=SELF_ERROR\n'
  printf 'error=%s\n' "$1"
  exit 10
}

if (( $# != 2 )); then
  usage
  self_error 'expected exactly: --level LEVEL'
fi
if [[ ${1:-} != '--level' ]]; then
  usage
  self_error 'first argument must be --level'
fi
level=${2:-}

case "$level" in
  light)
    default_mem_min_kib=2097152
    default_load_multiplier=1.5
    ;;
  medium)
    default_mem_min_kib=4194304
    default_load_multiplier=1.0
    ;;
  heavy)
    default_mem_min_kib=8388608
    default_load_multiplier=0.8
    ;;
  *)
    usage
    self_error "invalid level: $level"
    ;;
esac

mem_min_kib=${RESOURCE_GATE_MEM_MIN_KIB:-$default_mem_min_kib}
load_multiplier=${RESOURCE_GATE_LOAD_MULTIPLIER:-$default_load_multiplier}

for required_command in nproc awk df; do
  if ! command -v "$required_command" >/dev/null 2>&1; then
    self_error "required command not found: $required_command"
  fi
done

if ! [[ $mem_min_kib =~ ^[0-9]+$ ]]; then
  self_error 'RESOURCE_GATE_MEM_MIN_KIB must be a non-negative integer'
fi
if ! [[ $load_multiplier =~ ^[0-9]+([.][0-9]+)?$ ]]; then
  self_error 'RESOURCE_GATE_LOAD_MULTIPLIER must be a non-negative number'
fi
if [[ ! -r /proc/meminfo ]]; then
  self_error '/proc/meminfo is not readable'
fi
if [[ ! -r /proc/loadavg ]]; then
  self_error '/proc/loadavg is not readable'
fi
if [[ ! -e $disk_path ]]; then
  self_error "disk path does not exist: $disk_path"
fi

cpu_count=$(nproc) || self_error 'nproc failed'
mem_available_kib=$(awk '/^MemAvailable:/ {print $2; found=1; exit} END {if (!found) exit 1}' /proc/meminfo) ||
  self_error 'failed to read MemAvailable from /proc/meminfo'
load_one=$(awk '{print $1; exit}' /proc/loadavg) || self_error 'failed to read load1 from /proc/loadavg'
load_limit=$(awk -v cpus="$cpu_count" -v multiplier="$load_multiplier" \
  'BEGIN {printf "%.6f", cpus * multiplier}') || self_error 'failed to calculate load limit'
disk_available_kib=$(df -Pk "$disk_path" | awk 'NR == 2 {print $4; found=1} END {if (!found) exit 1}') ||
  self_error "failed to read disk availability for: $disk_path"

if ! [[ $cpu_count =~ ^[1-9][0-9]*$ ]]; then
  self_error "invalid nproc value: $cpu_count"
fi
if ! [[ $mem_available_kib =~ ^[0-9]+$ ]]; then
  self_error "invalid MemAvailable value: $mem_available_kib"
fi
if ! [[ $load_one =~ ^[0-9]+([.][0-9]+)?$ ]]; then
  self_error "invalid load1 value: $load_one"
fi
if ! [[ $disk_available_kib =~ ^[0-9]+$ ]]; then
  self_error "invalid disk availability value: $disk_available_kib"
fi

printf 'level=%s\n' "$level"
printf 'nproc=%s\n' "$cpu_count"
printf 'mem_available_kib=%s\n' "$mem_available_kib"
printf 'mem_min_kib=%s\n' "$mem_min_kib"
printf 'load1=%s\n' "$load_one"
printf 'load_multiplier=%s\n' "$load_multiplier"
printf 'load_limit=%s\n' "$load_limit"
printf 'disk_path=%s\n' "$disk_path"
printf 'disk_available_kib=%s\n' "$disk_available_kib"

if (( mem_available_kib < mem_min_kib )); then
  printf 'gate_result=INSUFFICIENT_MEMORY\n'
  exit 20
fi

if awk -v actual="$load_one" -v limit="$load_limit" 'BEGIN {exit !(actual > limit)}'; then
  printf 'gate_result=EXCESSIVE_LOAD\n'
  exit 21
fi

printf 'gate_result=PASS\n'
exit 0
